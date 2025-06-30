#ifndef SUPERFLAPPY_ENTITIES_HPP
#define SUPERFLAPPY_ENTITIES_HPP

#include "raylib.h"
#include <string>

#include "properties.hpp"
#include "algorithm"
#include "functional"

#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/optional.hpp>
#include "nlohmann/json.hpp"


struct Spawner; // Forward declaration


struct Chararacter {
    Vec2 pos;
    Vec2 vel;

    constexpr Rectangle rect(float w=1_b, float h=1_b) const {
         return Rectangle{pos.x, pos.y, std::move(w), std::move(h)};
    };

    // (De)Serialization
    Chararacter& load(const nlohmann::json& j);
    template <class Archive>
    void serialize(Archive& ar) { ar(vel, pos); }
};

struct Pipe {
    Vec2 pos;
    bool passed;

    constexpr std::array<Rectangle, 2> rect(const float w, const float h) const
        { return {Rectangle{pos.x, 0, w, pos.y}, Rectangle{pos.x, pos.y+h, w, 12_b}}; };

    // (De)Serialization
    Pipe& load(const nlohmann::json& j);
    template <class Archive>
    void serialize(Archive& ar) { ar(pos);};
};

struct Platform {
    Vec2 pos;
    Vec2 size {1_b, 1_b};

    constexpr Rectangle rect() const
        { return Rectangle{pos.x, pos.y, size.x, size.y}; };

    // (De)Serialization
    Platform& load(const nlohmann::json& j);
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

    // (De)Serialization
    Button& load(const nlohmann::json& j);
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

    // (De)Serialization
    Biome& load(const nlohmann::json& j);
    template <class Archive>
    void serialize(Archive& ar) {
        ar( pos, pipe_width, gap_height, pipe_speed,
            move_speed, gravity, jump_strength);
    }
};

struct World {
    std::optional<Chararacter> player;
    std::vector<Chararacter> enemies;
    std::vector<Spawner> spawners;
    std::vector<Pipe> pipes;
    std::vector<Button> buttons;
    std::vector<Platform> platforms;
    std::vector<Biome> biomes;

    // (De)Serialization
    World& load(const nlohmann::json& j);
    template <class Archive>
    void serialize(Archive& ar) {
        ar(player, enemies, spawners, pipes, buttons, platforms, biomes);
    }
};

struct Spawner {
    Vec2 pos;
    Vec2 vel;
    Action predicate;
    bool use_index;
    World spawn_in;

    template<typename Game_>
    bool check_predicate(const Game_& context) const {
        bool do_spawn = false;
        const std::vector<Action>& actions = context.get_scene()._actions;
        return (
            actions.end() !=
            std::find_if(actions.begin(), actions.end(),
                [&](const Action& elem) {
                    if (elem.type != predicate.type) return false;
                    if (!use_index) return true;
                    return (elem.index != predicate.index);
                }
            )
        );
    }

    // (De)Serialization
    Spawner& load(const nlohmann::json& j);
    template <class Archive>
    void serialize(Archive& ar) { ar(vel, pos, predicate, use_index, spawn_in); }
};




#endif // SUPERFLAPPY_ENTITIES_HPP