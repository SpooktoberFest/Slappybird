
#include "utils.hpp"

#include "raylib.h"


bool operator==(const Vec2& a, const Vec2& b) {
    return a.x == b.x && a.y == b.y;
}

bool operator==(const Vector2& a, const Vector2& b) {
    return a.x == b.x && a.y == b.y;
}

Vec2 operator-(const Vec2& minuend, const Vec2& subraend) {
    return {
        minuend.x - subraend.x,
        minuend.y - subraend.y
    };
}

Rectangle operator-(const Rectangle& minuend, const Vec2& subraend) {
    return {
        minuend.x - subraend.x,
        minuend.y - subraend.y,
        minuend.width,
        minuend.height
    };
}

GameState& operator^=(GameState& lhs, const GameState& rhs) {
    return (lhs = GameState(uint8_t(lhs) ^ uint8_t(rhs)));
}
bool operator&(const GameState& lhs, const GameState& rhs) {
    return (bool(uint8_t(lhs) & uint8_t(rhs)));
}
GameState operator|=(GameState& lhs, const GameState& rhs) {
    return (lhs = GameState(uint8_t(lhs) | uint8_t(rhs)));
}


