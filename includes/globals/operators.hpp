#ifndef SUPERFLAPPY_OPERATORS_HPP
#define SUPERFLAPPY_OPERATORS_HPP

#include <vector>
#include "properties.hpp"

bool operator==(const Vec2& a, const Vec2& b);
bool operator==(const Vector2& a, const Vector2& b);
Vec2 operator-(const Vec2& minuend, const Vec2& subraend);
Rect operator-(const Rect& minuend, const Vec2& subraend);
Rect operator+(const Rect& addend1, const Vec2& addend2);




#endif // SUPERFLAPPY_OPERATORS_HPP