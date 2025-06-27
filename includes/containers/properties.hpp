#ifndef SUPERFLAPPY_PROPERTIES_HPP
#define SUPERFLAPPY_PROPERTIES_HPP

#include <array>
#include <vector>

#include "raylib.h"
#include <cereal/types/array.hpp>

#include "unlock_map.hpp"
#include "enums.hpp"

#define BLOCK 32.0f

// Literal for block unit
constexpr float operator"" _b(unsigned long long value) {
    return static_cast<float>(value) * BLOCK;
}

struct Vec2 {
    float x;
    float y;

    operator Vector2() const { return {x, y}; };
    template <class Archive>
    void serialize(Archive& ar) { ar(x, y); };
};

struct Loadout {
    Equipment head;
    Equipment torso;
    Equipment hands;
    Equipment legs;
    Equipment weapon;
    std::array<Technique, 4> moveset;

    template <class Archive>
    void serialize(Archive& ar) { ar(head, torso, hands, legs, weapon, moveset); };
};

struct ControlScheme {
    std::array<KeyboardKey, 4> move = {
        KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT 
    };
    std::array<KeyboardKey, 4> nav = {
        KEY_W, KEY_S, KEY_A, KEY_UP 
    };
    KeyboardKey jump        = KEY_SPACE;
    KeyboardKey select      = KEY_ENTER;
    KeyboardKey pause       = KEY_ESCAPE;
    KeyboardKey reset       = KEY_BACKSPACE;

    template <class Archive>
    void serialize(Archive& ar) { ar(move, nav, jump, select, pause, reset); };
};

struct Profile {
    unlock_map<Equipment> equipment;
    unlock_map<Technique> moves;

    template <class Archive>
    void serialize(Archive& ar) { ar(equipment, moves); };
};




#endif // SUPERFLAPPY_PROPERTIES_HPP