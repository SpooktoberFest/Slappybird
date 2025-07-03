#pragma once

#include <string>
#include <vector>
#include <optional>

#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/optional.hpp>

#include "utils.hpp"
#include "forward_decl.hpp"
#include "properties.hpp"



struct Chararacter {
    Vec2 pos;
    Vec2 vel;

    Rectangle rect(float w=1, float h=1) const;

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
    Vec2 size {1, 1};

    Rectangle rect() const;

    // (De)Serialization
    Platform& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(pos, size); }
};

struct Biome {
    Vec2 pos;

    float pipe_width = 2;
    float gap_height = 5;
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


struct Button {
    std::string text;
    Action action;

    // (De)Serialization
    Button& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(text, action); }
};

struct ButtonList {
    std::vector<Button> buttons;
    uint8_t index = 0;
    bool horizontal = false;
    float pos, spacing, begin, end;
    Vec2 button_dims = {1, 1};
    Type special_content = Type::NONE;

    void clamp();
    void load_buttons(const std::vector<std::string>& str_vec, const ActionType type);
    std::vector<Rectangle> rects(const Vector2& res) const;

    // (De)Serialization
    ButtonList& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) {
        ar(buttons, horizontal, pos, spacing, begin, end, button_dims, special_content);
    }
};

struct Menu {
    std::vector<ButtonList> buttons;
    uint8_t index;
    bool fixed = true;

    void clamp();

    // (De)Serialization
    Menu& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) {
        ar(buttons, fixed);
    }
};

struct World {
    std::optional<Chararacter> player;
    std::optional<Menu> menu;
    std::vector<Chararacter> enemies;
    std::vector<Spawner> spawners;
    std::vector<Pipe> pipes;
    std::vector<Platform> platforms;
    std::vector<Biome> biomes;

    // (De)Serialization
    World& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) {
        ar(player, menu, enemies, spawners, pipes, platforms, biomes);
    }
};


struct Spawner {
    Vec2 pos;
    Vec2 vel;
    bool fixed = true;

    Action predicate;
    bool use_index;
    World spawn_in;

    bool check_predicate(const Game& context) const;

    // (De)Serialization
    Spawner& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(fixed, pos, vel, predicate, use_index, spawn_in); }
};

class Scene {
public:
    Scene();
    virtual ~Scene() {};

    World _world;
    Vec2 _cam_vel; // Where QNAN means follow player
    Vec2 _cam_pos;
    uint8_t _score = 0;
    std::vector<Action> _actions = {{ActionType::LOAD_WORLD}};

    // (De)Serialization
    Scene& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) {
        ar(_world, _cam_vel, _cam_pos, _score);
    }
};




