#ifndef SUPERFLAPPY_PROPERTIES_HPP
#define SUPERFLAPPY_PROPERTIES_HPP

#include <array>
#include <vector>

#include "raylib.h"
#include <cereal/types/array.hpp>

#include "unlock_map.hpp"
#include "enums.hpp"

class Serializer; // Forward declaration

// Literal for block unit
constexpr float operator"" _b(unsigned long long value) {
    return static_cast<float>(value) * 32.0f;
}

struct Vec2 {
    float x;
    float y;

    template <class Archive>
    void serialize(Archive& ar) { ar(x, y); };
    operator Vector2() const { return {x, y}; };
};

struct Rect {
    float x;
    float y;
    float w = 1_b;
    float h = 1_b;

    template <class Archive>
    void serialize(Archive& ar) { ar(x, y, w, h); };
    operator Rectangle() const { return {x, y, w, h}; };
};

struct Loadout {
    Equipment _men;
    Equipment _do;
    Equipment _kote;
    Equipment _gi;
    Equipment _hakama;
    Equipment _shinai;
    std::array<Technique, 4> _moveset;

    template <class Archive>
    void serialize(Archive& ar) { ar(_men, _do, _kote, _gi, _hakama, _shinai, _moveset); };
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
    unlock_map<Equipment> _sets;
    unlock_map<Technique> _movesets;

    template <class Archive>
    void serialize(Archive& ar) { ar(_sets, _movesets); };
};




#endif // SUPERFLAPPY_PROPERTIES_HPP