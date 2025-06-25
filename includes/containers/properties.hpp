#ifndef SUPERFLAPPY_PROPERTIES_HPP
#define SUPERFLAPPY_PROPERTIES_HPP

#include <limits.h>
#include <stdint.h>

#include "raylib.h"

#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

// Literal for block unit
constexpr float operator"" _b(unsigned long long value) {
    return static_cast<float>(value) * 32.0f;
}


struct Vec2 {
    float x;
    float y;

    template <class Archive>
    void serialize(Archive& ar) { ar(x, y); }
    operator Vector2() const { return {x, y}; };
};

struct Rect {
    float x;
    float y;
    float w = 1_b;
    float h = 1_b;

    template <class Archive>
    void serialize(Archive& ar) { ar(x, y, w, h); }
    operator Rectangle() const { return {x, y, w, h}; };
};

enum Action : int16_t {
    // Load world
    LOAD_CUSTOM_WORLD = SHRT_MIN, // TODO()
    LOAD_CANNON_WORLD = SHRT_MIN*2/3,
    LOAD_UI = SHRT_MIN/3,

    // Actions that can be called from any world
    NORMAL_ACTION = 0,
    TOGGLE_GROUP_1,
    TOGGLE_GROUP_2,
    TOGGLE_GROUP_3,
    TOGGLE_GROUP_4,

    // Actions that cannot be called from custom worlds
    SPECIAL_ACTION = SHRT_MAX/2,
    NEW_SAVE,
    LOAD_SAVE,
    DELETE_SAVE,
};

struct Profile {
    
};


struct ControlScheme {
    //*
    KeyboardKey move_up     = KEY_UP;
    KeyboardKey move_down   = KEY_DOWN;
    KeyboardKey move_left   = KEY_LEFT;
    KeyboardKey move_right  = KEY_RIGHT;
    KeyboardKey nav_up      = KEY_W;
    KeyboardKey nav_down    = KEY_S;
    KeyboardKey nav_left    = KEY_A;
    KeyboardKey nav_right   = KEY_UP;
    /*/
    KeyboardKey move_up     = KEY_W;
    KeyboardKey move_down   = KEY_S;
    KeyboardKey move_left   = KEY_A;
    KeyboardKey move_right  = KEY_D;
    KeyboardKey nav_up      = KEY_UP;
    KeyboardKey nav_down    = KEY_DOWN;
    KeyboardKey nav_left    = KEY_LEFT;
    KeyboardKey nav_right   = KEY_RIGHT;
    //*/
    KeyboardKey jump        = KEY_SPACE;
    KeyboardKey select      = KEY_ENTER;
    KeyboardKey pause       = KEY_ESCAPE;
    KeyboardKey reset       = KEY_BACKSPACE;
};





#endif // SUPERFLAPPY_PROPERTIES_HPP