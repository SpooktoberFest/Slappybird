#ifndef SUPERFLAPPY_ENTITIES_HPP
#define SUPERFLAPPY_ENTITIES_HPP

#include <string>
#include <vector>
#include <optional>

#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/optional.hpp>

#include "forward_decl.hpp"
#include "properties.hpp"


struct Chararacter {
    Vec2 pos;
    Vec2 vel;

    Rectangle rect(float w=1_b, float h=1_b) const;

    // (De)Serialization
    Chararacter& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(vel, pos); }
};

struct Pipe {
    Vec2 pos;
    bool passed;

    std::array<Rectangle, 2> rect(const float w, const float h) const;

    // (De)Serialization
    Pipe& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(pos);};
};

struct Platform {
    Vec2 pos;
    Vec2 size {1_b, 1_b};

    Rectangle rect() const;

    // (De)Serialization
    Platform& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(pos, size); }
};

struct Button {
    Vec2 pos;
    Vec2 size;
    std::string text;
    Action action;
    int parameter;

    Rectangle rect(const Vector2& res) const;

    // (De)Serialization
    Button& load(const JsonRef jf);
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
    Biome& load(const JsonRef jf);
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
    World& load(const JsonRef jf);
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

    bool check_predicate(const Game& context) const;

    // (De)Serialization
    Spawner& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(vel, pos, predicate, use_index, spawn_in, spawn); }
};




#endif // SUPERFLAPPY_ENTITIES_HPP