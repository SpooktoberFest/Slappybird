#ifndef SUPERFLAPPY_OPERATORS_HPP
#define SUPERFLAPPY_OPERATORS_HPP

#include "raylib.h"
#include <vector>

bool operator==(const Vector2& a, const Vector2& b);
Vector2 operator-(const Vector2& minuend, const Vector2& subraend);
Rectangle operator-(const Rectangle& minuend, const Vector2& subraend);
Rectangle operator+(const Rectangle& addend1, const Vector2& addend2);




#endif // SUPERFLAPPY_OPERATORS_HPP