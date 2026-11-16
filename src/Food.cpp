#include "Food.h"
#include <random>
#include <ctime>

Food::Food() {
    pos = { 15, 10 }; // Default initial position
}

void Food::Spawn(int cols, int rows, const Snake& snake) {
    // Static random engine seeded with system time
    static std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> disX(0, cols - 1);
    std::uniform_int_distribution<int> disY(0, rows - 1);

    bool collision = true;
    int safetyCounter = 0;
    int maxTries = cols * rows;

    while (collision && safetyCounter < maxTries) {
        pos.x = disX(gen);
        pos.y = disY(gen);
        collision = false;

        // Check if generated coordinate overlaps with snake body
        for (const auto& segment : snake.body) {
            if (pos == segment) {
                collision = true;
                break;
            }
        }
        safetyCounter++;
    }
}
