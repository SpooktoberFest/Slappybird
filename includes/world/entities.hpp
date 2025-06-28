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
    Vec2 size {1_b, 1_b};

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
    int parameter;

    constexpr Rectangle rect(const Vector2& res) const {
        return Rectangle{
            (pos.x < 0.0f ? res.x + pos.x : pos.x),
            (pos.y < 0.0f ? res.y + pos.y : pos.y),
            size.x, size.y};
        };

    template <class Archive>
    void serialize(Archive& ar) { ar(pos, size, text, action, parameter); }
};

struct Spawner {
    Vec2 pos;
    Vec2 vel;



    template <class Archive>
    void serialize(Archive& ar) { ar(vel, pos); }
};


#endif // SUPERFLAPPY_ENTITIES_HPP