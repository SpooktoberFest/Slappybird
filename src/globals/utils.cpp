
#include "utils.hpp"

#include "raylib.h"

// float BLOCK = 32.0f;

// Comparison Operators
bool operator==(const Vec2& a, const Vec2& b)       { return a.x == b.x && a.y == b.y; }
bool operator==(const Vector2& a, const Vector2& b) { return a.x == b.x && a.y == b.y; }

// Aritmetic Operators
Vec2 operator-(const Vec2& lhs, const Vec2& rhs)    { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
Vec2 operator*(const Vec2& lhs, const float rhs)    { return { lhs.x * rhs, lhs.y * rhs}; }
Vector2 operator*(const Vector2& lhs, const float rhs)    { return { lhs.x * rhs, lhs.y * rhs}; }
Rectangle operator-(const Rectangle& lhs, const Vec2& rhs)
    { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.width, lhs.height}; }
Rectangle operator*(const Rectangle& lhs, const float rhs)
    { return { lhs.x * rhs, lhs.y * rhs, lhs.width * rhs, lhs.height * rhs }; }

// Assignment Operators
Vec2& operator+=(Vec2& lhs, const Vec2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
Rectangle& operator*=(Rectangle& lhs, const float rhs) {
    lhs.x *= rhs;       lhs.y *= rhs;
    lhs.width *= rhs;   lhs.height *= rhs;
    return lhs;
}

// Enum Operators
bool check_flag(const GameState& lhs, const GameState& rhs)  { return bool(uint8_t(lhs) & uint8_t(rhs)); }
GameState& flip_flag(GameState& lhs, const GameState& rhs) { return (lhs = GameState(uint8_t(lhs) ^ uint8_t(rhs))); }
GameState& set_flag(GameState& lhs, const GameState& rhs)  { return (lhs = GameState(uint8_t(lhs) | uint8_t(rhs))); }


