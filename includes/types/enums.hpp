#pragma once

#include <limits.h>
#include <cstdint>

// Unscoped enums

enum Diretion : int8_t { UP, DOWN, LEFT, RIGHT };

enum GameState : uint8_t {
    INIT        = 0,
    GAMEOVER    = 1,
    PAUSED      = 2,
    QUIT        = 4
};


// Scoped enums


enum class ActionType : uint8_t {
    // Actions that can be called from any world
    NOP,
    TOGGLE_GROUP,
    RESET_WORLD,
    LOAD_WORLD,
    SHOW_WORLDS,
    LOAD_MENU,
    

    // Actions that cannot be called from custom worlds
    SPECIAL_ACTION = UCHAR_MAX/2,
    EXIT_GAME,
    SHOW_PROFILES,
    LOAD_PROFILE,
    NEW_PROFILE,
    DELETE_PROFILE,
};

enum class Equipment : uint8_t {
    DEFAULT_M, DEFAULT_F,
    WHITE_M, WHITE_F,
    BLACK_M, BLACK_F,
    YELLOW_M, YELLOW_F,
    RED_M, RED_F,
    GREEN_M, GREEN_F,
    BLUE_M, BLUE_F,
    GREY_M, GREY_F,
    RAINBOW_L, RAINBOW_G, RAINBOW_T,
};

enum class Technique : uint8_t {
    NONE,
    MEN, DO, KOTE
};

enum class Type : uint8_t {
    NONE,

    PROPERTY,
    VECTOR,
    LOADOUT,
    CONTROLSHEME,
    PROFILE,
    ACTION,


    ENTITY,
    CHARACTER,
    PLAYER,
    ENEMY,
    PIPE,
    PLATFORM,
    BIOME,
    BUTTON,
    BUTTONLIST,
    MENU,
    WORLD,
    SPAWNER,
    SCENE,

    GAME
};




