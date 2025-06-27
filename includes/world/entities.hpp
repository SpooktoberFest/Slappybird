#ifndef SUPERFLAPPY_ENTITIES_HPP
#define SUPERFLAPPY_ENTITIES_HPP

#include "raylib.h"
#include <string>

#include "properties.hpp"

#include <cereal/types/string.hpp>



struct Chararacter {
    Vec2 pos;
    Vec2 vel;

    constexpr Rectangle rect(float w, float h) const
        { return Rectangle{pos.x, pos.y, std::move(w), std::move(h)}; };

    template <class Archive>
    void serialize(Archive& ar) { ar(vel, pos); }
};

struct Pipe {
    Vec2 pos;
    bool passed;

    constexpr std::array<Rectangle, 2> rect(const float w, const float h) const
        { return {Rectangle{pos.x, 0, w, pos.y}, Rectangle{pos.x, pos.y+h, w, 12_b}}; };

    template <class Archive>
    void serialize(Archive& ar) { ar(pos);};
};

struct Platform {
    Vec2 pos;
    Vec2 size;

    constexpr Rectangle rect() const
        { return Rectangle{pos.x, pos.y, size.x, size.y}; };

    template <class Archive>
    void serialize(Archive& ar) { ar(pos, size); }
};

struct Button {
    Vec2 pos;
    Vec2 size;
    std::string text;
    Action action;

    constexpr Rectangle rect() const
        { return Rectangle{pos.x, pos.y, size.x, size.y}; };    

    template <class Archive>
    void serialize(Archive& ar) { ar(pos, size, text, action); }
};





#endif // SUPERFLAPPY_ENTITIES_HPP