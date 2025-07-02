#ifndef SUPERFLAPPY_OPERATORS_HPP
#define SUPERFLAPPY_OPERATORS_HPP

#include <limits>

#include "properties.hpp"
#include "forward_decl.hpp"

#define branchless_ternary(pred, a, b) (a & -pred) | (b & ~-pred)

#define QNAN std::numeric_limits<float>::quiet_NaN()
#define BLOCK 32.0f


// Literal for block unit
constexpr float operator"" _b(unsigned long long value) {
    return static_cast<float>(value) * BLOCK;
}
bool operator==(const Vec2& a, const Vec2& b);
bool operator==(const Vector2& a, const Vector2& b);
Vec2 operator-(const Vec2& minuend, const Vec2& subraend);
Rectangle operator-(const Rectangle& minuend, const Vec2& subraend);





#endif // SUPERFLAPPY_OPERATORS_HPP