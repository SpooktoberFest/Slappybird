#ifndef SUPERFLAPPY_OPERATORS_HPP
#define SUPERFLAPPY_OPERATORS_HPP

#include <vector>

#include "enums.hpp"
#include "properties.hpp"


constexpr bool operator==(const Vec2& a, const Vec2& b) {
    return a.x == b.x && a.y == b.y;
}

constexpr bool operator==(const Vector2& a, const Vector2& b) {
    return a.x == b.x && a.y == b.y;
}

constexpr Vec2 operator-(const Vec2& minuend, const Vec2& subraend) {
    return {
        minuend.x - subraend.x,
        minuend.y - subraend.y
    };
}

constexpr Rect operator-(const Rect& minuend, const Vec2& subraend) {
    return {
        minuend.x - subraend.x,
        minuend.y - subraend.y,
        minuend.w,
        minuend.h
    };
}





#endif // SUPERFLAPPY_OPERATORS_HPP