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
    Chararacter(const JsonRef jr);
    Chararacter() = default;
    ~Chararacter() = default;

    Vec2 pos;
    Vec2 vel;

    Rectangle get_hitbox(float w=1, float h=1) const;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(vel, pos); }
};


struct Pipe {
    Pipe(const JsonRef jr);
    Pipe() = default;
    ~Pipe() = default;

    Vec2 pos;
    bool passed;

    std::array<Rectangle, 2> get_hitbox(const float w, const float h) const;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(pos);};
};

struct Platform {
    Platform(const JsonRef jr);
    Platform() = default;
    ~Platform() = default;

    Vec2 pos;
    Vec2 size {1, 1};

    Rectangle get_hitbox() const;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(pos, size); }
};

struct Biome {
    Biome(const JsonRef jr);
    Biome() = default;
    ~Biome() = default;

    Vec2 pos;

    float pipe_width = 2;
    float gap_height = 5;
    float pipe_speed = 3.0f;

    float move_speed = 5.0f;
    float gravity = 0.5f;
    float jump_strength = -8.0f;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) {
        ar( pos, pipe_width, gap_height, pipe_speed,
            move_speed, gravity, jump_strength);
    }
};


struct Button {
    Button(const JsonRef jr);
    Button(const std::string text, Action action) : text(text), action(action) {};
    Button() = default;
    ~Button() = default;

    std::string text;
    Action action;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(text, action); }
};

struct ButtonList {
    ButtonList(const JsonRef jr);
    ButtonList() = default;
    ~ButtonList() = default;

    std::vector<Button> buttons;
    float pos, spacing, begin, end;
    Vec2 button_dims = {1, 1};
    uint8_t index = 0;
    Type special_content = Type::NONE;
    bool horizontal = false;

    void clamp_index();
    void load_buttons(const std::vector<std::string>& str_vec, const ActionType type);
    std::vector<Rectangle> get_hitboxes(const Vector2& res) const;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) {
        ar(buttons, horizontal, pos, spacing, begin, end, button_dims, special_content);
    }
};

struct Menu {
    Menu(const JsonRef jr);
    Menu() = default;
    ~Menu() = default;

    std::vector<ButtonList> buttons;
    uint8_t index;
    bool fixed = true;

    void clamp_index();

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) {
        ar(buttons, fixed);
    }
};

struct World {
    World(const JsonRef jr);
    World() = default;
    ~World() = default;

    void emplace(World& world, const Vec2& pos);

    std::vector<Chararacter> enemies;
    std::vector<Spawner> spawners;
    std::vector<Pipe> pipes;
    std::vector<Platform> platforms;
    std::vector<Biome> biomes;

    std::optional<Menu> menu;
    std::optional<Chararacter> player;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) {
        ar(player, menu, enemies, spawners, pipes, platforms, biomes);
    }
};


struct Spawner {
    Spawner(const JsonRef jr);
    Spawner() = default;
    ~Spawner() = default;

    Vec2 pos;
    Vec2 vel;
    bool fixed = true;

    Trigger predicate;
    World spawn_in;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(fixed, pos, vel, predicate, spawn_in); }
};

class Scene {
public:
    Scene();
    ~Scene() {};

    std::vector<Trigger> _events;
    Vec2 _cam_vel; // Where QNAN means follow player
    Vec2 _cam_pos;
    World _world;
    uint8_t _score = 0;

    // (De)Serialization
    Scene& load(const JsonRef jr);
    template <class Archive>
    void serialize(Archive& ar) {
        ar(_world, _cam_vel, _cam_pos, _score);
    }
};
