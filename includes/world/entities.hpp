#ifndef SUPERFLAPPY_ENTITIES_HPP
#define SUPERFLAPPY_ENTITIES_HPP

#include "raylib.h"
#include <string>

#include "properties.hpp"

#include <cereal/types/string.hpp>



struct Chararacter {
    Vec2 pos;
    Vec2 vel;

    template <class Archive>
    void serialize(Archive& ar) { ar(vel, pos); }
    constexpr Rect box(float w, float h) const
        { return Rect{pos.x, pos.y, std::move(w), std::move(h)}; };
};

struct Pipe {
    Vec2 pos;
    bool passed;

    template <class Archive>
    void serialize(Archive& ar) { ar(pos);};

    constexpr std::array<Rect, 2> box(const float w, const float h) const
        { return {Rect{pos.x, 0, w, pos.y}, Rect{pos.x, pos.y+h, w, 12_b}}; };
};

struct Platform {
    Rect rect;

    template <class Archive>
    void serialize(Archive& ar) { ar(rect); }
};

struct Button {
    Rect rect;
    std::string text;
    Action action;

    template <class Archive>
    void serialize(Archive& ar) { ar(rect, text, action); }
};





#endif // SUPERFLAPPY_ENTITIES_HPP