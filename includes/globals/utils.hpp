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
bool operator==(const Vec2& lhs, const Vec2& rhs);
bool operator==(const Vector2& lhs, const Vector2& rhs);

// Aritmetic Operators
Vec2 operator-(const Vec2& lhs, const Vec2& rhs);
Vec2 operator*(const Vec2& lhs, const float rhs);
Vector2 operator*(const Vector2& lhs, const float rhs);
Rectangle operator-(const Rectangle& lhs, const Vec2& rhs);
Rectangle operator*(const Rectangle& lhs, const float rhs);

// Assignment Operators
Vec2& operator+=(Vec2& lhs, const Vec2& rhs);
Rectangle& operator*=(Rectangle& lhs, const float rhs);

// Enum Operators
bool check_flag(const GameState& lhs, const GameState& rhs);
GameState& flip_flag(GameState& lhs, const GameState& rhs);
GameState& set_flag(GameState& lhs, const GameState& rhs);


