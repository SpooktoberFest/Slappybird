#ifndef SUPERFLAPPY_ENTITIES_HPP
#define SUPERFLAPPY_ENTITIES_HPP

#include "raylib.h"
#include <vector>
#include <string>
#include <limits.h>

struct Player {
    Rectangle hitbox = {100, 450/2, 34, 24};
    Vector2 velocity = {0.0f, 0.0f};
    float move_speed;
};

struct Pipe {
    float x;
    float gapY;
    Rectangle topRect;
    Rectangle bottomRect;
    bool passed;
};


enum Action : int16_t {
    // Load world
    LOAD_CUSTOM_WORLD = SHRT_MIN,
    LOAD_CANNON_WORLD = SHRT_MIN/2,

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

struct Button {
    Rectangle rect;
    std::string text;
    Action action;
};


struct ControlScheme {
    //*
    KeyboardKey move_up     = KEY_UP;
    KeyboardKey move_down   = KEY_DOWN;
    KeyboardKey move_left   = KEY_LEFT;
    KeyboardKey move_right  = KEY_RIGHT;
    // KeyboardKey nav_up      = KEY_W;
    // KeyboardKey nav_down    = KEY_S;
    // KeyboardKey nav_left    = KEY_A;
    // KeyboardKey nav_right   = KEY_UP;
    /*/
    KeyboardKey move_up     = KEY_W;
    KeyboardKey move_down   = KEY_S;
    KeyboardKey move_left   = KEY_A;
    KeyboardKey move_right  = KEY_D;
    // KeyboardKey nav_up      = KEY_UP;
    // KeyboardKey nav_down    = KEY_DOWN;
    // KeyboardKey nav_left    = KEY_LEFT;
    // KeyboardKey nav_right   = KEY_RIGHT;
    //*/
    KeyboardKey jump        = KEY_SPACE;
    KeyboardKey select      = KEY_ENTER;
    KeyboardKey pause       = KEY_ESCAPE;
    KeyboardKey reset       = KEY_BACKSPACE;
};





#endif // SUPERFLAPPY_ENTITIES_HPP