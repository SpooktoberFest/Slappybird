#ifndef SUPERFLAPPY_ENUMS_HPP
#define SUPERFLAPPY_ENUMS_HPP

#include <limits.h>

enum Action : int16_t {
    // Load world
    LOAD_CUSTOM_WORLD = SHRT_MIN,
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

enum class Technique : int16_t {
    NONE,
    MEN, DO, KOTE
};


#endif // SUPERFLAPPY_ENUMS_HPP