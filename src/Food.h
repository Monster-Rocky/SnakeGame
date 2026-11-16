#pragma once
#include "Snake.h"

class Food {
public:
    Point pos;

    Food();

    // Place food at random coordinates avoiding overlay with snake body
    void Spawn(int cols, int rows, const Snake& snake);
};
