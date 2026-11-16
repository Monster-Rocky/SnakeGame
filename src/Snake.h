#pragma once
#include <deque>

struct Point {
    int x;
    int y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

class Snake {
public:
    std::deque<Point> body;
    Direction dir;
    bool growPending;

    Snake();

    // Reset snake to starting position and standard length
    void Reset(int startX, int startY);

    // Update body segments based on direction
    void Move();

    // Change direction while preventing direct reversals
    void ChangeDirection(Direction newDir);

    // Increment length on next move tick
    void Grow();

    // Check if the snake head collided with its body
    bool CheckSelfCollision() const;

    // Check if the snake head collided with grid boundaries
    bool CheckWallCollision(int cols, int rows) const;
};
