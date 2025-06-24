#include "operators.hpp"


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

Rect operator-(const Rect& minuend, const Vec2& subraend) {
    return {
        minuend.x - subraend.x,
        minuend.y - subraend.y,
        minuend.w,
        minuend.h
    };
}

Rect operator+(const Rect& addend1, const Vec2& addend2) {
    return {
        addend1.x - addend2.x,
        addend1.y - addend2.y,
        addend1.w,
        addend1.h
    };
}
