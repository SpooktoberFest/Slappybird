#ifndef SUPERFLAPPY_ENTITIES_HPP
#define SUPERFLAPPY_ENTITIES_HPP

#include "raylib.h"
#include <vector>
#include <string>

#include "properties.hpp"

#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

struct Player {
    Rect hitbox = {100, 450/2, 34, 24};
    Vec2 velocity;
    float move_speed = 5.0f;
};

struct Pipe {
    float x;
    float gapY;
    Rect topRect;
    Rect bottomRect;
    bool passed;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(x, gapY, topRect, bottomRect, passed);
    }
};

struct Platform {
    Rect rect;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(rect);
    }
};

struct Button {
    Rect rect;
    std::string text;
    Action action;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(rect, text, action);
    }
};





#endif // SUPERFLAPPY_ENTITIES_HPP