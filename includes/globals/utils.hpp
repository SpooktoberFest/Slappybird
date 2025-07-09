#pragma once

#include <limits>

#include "enums.hpp"
#include "properties.hpp"
#include "forward_decl.hpp"

#define branchless_ternary(pred, a, b) (a & -pred) | (b & ~-pred)

#define QNAN std::numeric_limits<float>::quiet_NaN()

#define BLOCK 32.0f
// extern float BLOCK;

// Literal for block unit
constexpr float operator"" _b(unsigned long long value) {
    return static_cast<float>(value) * BLOCK;
}

// Comparison Operators
constexpr bool operator==(const Vec2& a, const Vec2& b)     { return a.x == b.x && a.y == b.y; }
bool operator==(const Vector2& lhs, const Vector2& rhs);

// Aritmetic Operators
constexpr Vec2 operator-(const Vec2& lhs, const Vec2& rhs)  { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
constexpr Vec2 operator*(const Vec2& lhs, const float rhs)  { return { lhs.x * rhs, lhs.y * rhs}; }
Vector2 operator*(const Vector2& lhs, const float rhs);
Rectangle operator-(const Rectangle& lhs, const Vec2& rhs);
Rectangle operator*(const Rectangle& lhs, const float rhs);

// Assignment Operators
constexpr Vec2& operator+=(Vec2& lhs, const Vec2& rhs)      { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
Rectangle& operator*=(Rectangle& lhs, const float rhs);

// Enum Operators
constexpr bool check_flag(const GameState& lhs, const GameState& rhs)   { return bool(uint8_t(lhs) & uint8_t(rhs)); }
constexpr GameState& flip_flag(GameState& lhs, const GameState& rhs)    { return (lhs = GameState(uint8_t(lhs) ^ uint8_t(rhs))); }
constexpr GameState& set_flag(GameState& lhs, const GameState& rhs)     { return (lhs = GameState(uint8_t(lhs) | uint8_t(rhs))); }
