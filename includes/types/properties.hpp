#pragma once

#include <array>
#include <cereal/types/array.hpp>

#include "enums.hpp"
#include "forward_decl.hpp"
#include "unlock_map.hpp"


struct Vec2 {
    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {};
    // Requires json array to have length of at least 2, if the field exists.
    Vec2(const JsonRef jr, const std::string field, const float def=0);
    ~Vec2() = default;

    float x, y;

    Vec2& emplace(const Vec2& other);
    operator Vector2() const;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(x, y); };
};

struct Loadout {
    Loadout(const JsonRef jr);
    Loadout() = default;
    ~Loadout() = default;

    std::array<Technique, 4> moveset;
    Equipment head , torso, hands, legs, weapon;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(head, torso, hands, legs, weapon, moveset); };
};

struct ControlScheme {
    ControlScheme(const JsonRef jr);
    ControlScheme();
    ~ControlScheme() = default;

    std::array<KeyboardKey_, 4> move;
    std::array<KeyboardKey_, 4> nav;
    KeyboardKey_ jump, select, pause, reset;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(move, nav, jump, select, pause, reset); };
};


struct Profile {
    Profile(const JsonRef jr);
    Profile() = default;
    ~Profile() = default;

    unlock_map<Equipment> equipment;
    unlock_map<Technique> moves;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(equipment, moves); };
};

struct Action {
    Action(const JsonRef jr);
    Action(const ActionType type, const uint8_t index) : type(type), index(index) {};
    Action() = default;
    ~Action() = default;

    ActionType type;
    uint8_t index;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(type, index); };
};

struct Trigger {
    Trigger(const JsonRef jr);
    Trigger() = default;
    ~Trigger() = default;

    TriggerType type;
    uint8_t parameter, state;

    // (De)Serialization
    template <class Archive>
    void serialize(Archive& ar) { ar(type, parameter); };
};
