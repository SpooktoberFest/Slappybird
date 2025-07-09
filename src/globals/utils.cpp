
#include "utils.hpp"

#include "raylib.h"

// Comparison Operators
bool operator==(const Vector2& a, const Vector2& b) { return a.x == b.x && a.y == b.y; }

// Aritmetic Operators
Vector2 operator*(const Vector2& lhs, const float rhs)    { return { lhs.x * rhs, lhs.y * rhs}; }
Rectangle operator-(const Rectangle& lhs, const Vec2& rhs)
    { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.width, lhs.height}; }
Rectangle operator*(const Rectangle& lhs, const float rhs)
    { return { lhs.x * rhs, lhs.y * rhs, lhs.width * rhs, lhs.height * rhs }; }

// Assignment Operators
Rectangle& operator*=(Rectangle& lhs, const float rhs) {
    lhs.x *= rhs;       lhs.y *= rhs;
    lhs.width *= rhs;   lhs.height *= rhs;
    return lhs;
}
