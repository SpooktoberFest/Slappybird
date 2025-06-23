#ifndef SUPERFLAPPY_ENTITIES_HPP
#define SUPERFLAPPY_ENTITIES_HPP

#include "raylib.h"
#include <vector>


struct Player {
    Rectangle hitbox = {100, 450/2, 34, 24};
    Vector2 velocity = {0.0f, 0.0f};
    float move_speed;
};

struct Pipe {
    float x;
    float gapY;
    Rectangle topRect;
    Rectangle bottomRect;
    bool passed;
};

struct Button {
    Rectangle bottomRect;
};




#endif // SUPERFLAPPY_ENTITIES_HPP