#ifndef SUPERFLAPPY_OPERATORS_HPP
#define SUPERFLAPPY_OPERATORS_HPP

#include "properties.hpp"


bool operator==(const Vec2& a, const Vec2& b);
bool operator==(const Vector2& a, const Vector2& b);
Vec2 operator-(const Vec2& minuend, const Vec2& subraend);
Rectangle operator-(const Rectangle& minuend, const Vec2& subraend);





#endif // SUPERFLAPPY_OPERATORS_HPP