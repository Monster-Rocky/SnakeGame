#include "Snake.h"

Snake::Snake() {
    Reset(10, 10);
}

void Snake::Reset(int startX, int startY) {
    body.clear();
    dir = Direction::Right;
    growPending = false;

    // Start with a length of 3 segments
    body.push_back({ startX, startY });
    body.push_back({ startX - 1, startY });
    body.push_back({ startX - 2, startY });
}

void Snake::Move() {
    if (body.empty()) return;

    Point head = body.front();
    Point newHead = head;

    switch (dir) {
        case Direction::Up:    newHead.y--; break;
        case Direction::Down:  newHead.y++; break;
        case Direction::Left:  newHead.x--; break;
        case Direction::Right: newHead.x++; break;
    }

    // Insert new head at the front
    body.push_front(newHead);

    // If growing, keep the tail and reset the flag. Otherwise, pop the tail.
    if (growPending) {
        growPending = false;
    } else {
        body.pop_back();
    }
}

void Snake::ChangeDirection(Direction newDir) {
    // Prevent standard reversing into itself
    if (newDir == Direction::Up && dir != Direction::Down)    dir = newDir;
    if (newDir == Direction::Down && dir != Direction::Up)    dir = newDir;
    if (newDir == Direction::Left && dir != Direction::Right)  dir = newDir;
    if (newDir == Direction::Right && dir != Direction::Left)  dir = newDir;
}

void Snake::Grow() {
    growPending = true;
}

bool Snake::CheckSelfCollision() const {
    if (body.size() < 4) return false; // Self-collision impossible with < 4 segments

    const Point& head = body.front();
    for (size_t i = 1; i < body.size(); ++i) {
        if (head == body[i]) {
            return true;
        }
    }
    return false;
}

bool Snake::CheckWallCollision(int cols, int rows) const {
    if (body.empty()) return false;
    const Point& head = body.front();
    return (head.x < 0 || head.x >= cols || head.y < 0 || head.y >= rows);
}
