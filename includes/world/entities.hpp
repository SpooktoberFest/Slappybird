#ifndef SUPERFLAPPY_ENTITIES_HPP
#define SUPERFLAPPY_ENTITIES_HPP

#include "raylib.h"
#include <string>

#include "properties.hpp"

#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/optional.hpp>



struct Chararacter {
    Vec2 pos;
    Vec2 vel;

    constexpr Rectangle rect(float w=1_b, float h=1_b) const
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

struct Biome {
    Vec2 pos;

    float pipe_width = 2_b;
    float gap_height = 150.0f;
    float pipe_speed = 3.0f;

    float move_speed = 5.0f;
    float gravity = 0.5f;
    float jump_strength = -8.0f;

    template <class Archive>
    void serialize(Archive& ar) {
        ar( pos, pipe_width, gap_height, pipe_speed,
            move_speed, gravity, jump_strength);
    }
};

struct World {
    std::optional<Chararacter> player;
    std::vector<Chararacter> enemies;
    std::vector<Pipe> pipes;
    std::vector<Button> buttons;
    std::vector<Platform> platforms;
    std::vector<Biome> biomes;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(player, enemies, pipes, buttons, platforms, biomes);
    }
};

struct Spawner {
    Vec2 pos;
    Vec2 vel;
    Action predicate;
    Entity type;

    // std::optional<World> check_and_spawn(Context context) {
    //     if (!predicate) return std::nullopt;
    //         World spawn_in;
    //         return spawn_in;
    // }

    template <class Archive>
    void serialize(Archive& ar) { ar(vel, pos, predicate); }
};





#endif // SUPERFLAPPY_ENTITIES_HPP