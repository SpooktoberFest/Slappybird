#include "operators.hpp"


bool operator==(const Vector2& a, const Vector2& b) {
    return a.x == b.x && a.y == b.y;
}

Vector2 operator-(const Vector2& minuend, const Vector2& subraend) {
    return {
        minuend.x - subraend.x,
        minuend.y - subraend.y
    };
}

Rectangle operator-(const Rectangle& minuend, const Vector2& subraend) {
    return {
        minuend.x - subraend.x,
        minuend.y - subraend.y,
        minuend.width,
        minuend.height
    };
}

Rectangle operator+(const Rectangle& addend1, const Vector2& addend2) {
    return {
        addend1.x - addend2.x,
        addend1.y - addend2.y,
        addend1.width,
        addend1.height
    };
}
