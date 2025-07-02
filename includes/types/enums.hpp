#ifndef SUPERFLAPPY_ENUMS_HPP
#define SUPERFLAPPY_ENUMS_HPP

#include <limits.h>

enum ActionType : u_int8_t {
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

enum Diretion : int8_t { UP, DOWN, LEFT, RIGHT };

enum class Equipment : u_int16_t {
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

enum class Technique : u_int16_t {
    NONE,
    MEN, DO, KOTE
};

enum class Entity : u_int8_t {
    PLAYER,
    ENEMY,
    PIPE,
    PLATFORM,
    BUTTON,
    SPAWNER
};


#endif // SUPERFLAPPY_ENUMS_HPP