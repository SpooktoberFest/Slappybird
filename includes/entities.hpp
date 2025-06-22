#ifndef SUPERFLAPPY_ENTITIES_HPP
#define SUPERFLAPPY_ENTITIES_HPP

#include "raylib.h"
#include <vector>


struct Bird {
    float y;
    float velocity;
    Rectangle rect;
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