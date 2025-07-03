#include "properties.hpp"

#include "raylib.h"

#include "utils.hpp"

#define K(key) static_cast<KeyboardKey_>(key)

Vec2::operator Vector2() const { return {x, y}; };


Vec2& Vec2::emplace(const Vec2& other) {
    if (x == QNAN) x = other.x;
    if (y == QNAN) y = other.y;
    return *this;
}


ControlScheme::ControlScheme() :
    move({K(KEY_UP), K(KEY_DOWN), K(KEY_LEFT), K(KEY_RIGHT)}),
    nav({K(KEY_W), K(KEY_S), K(KEY_A), K(KEY_D)}),
    jump(K(KEY_SPACE)),
    select(K(KEY_ENTER)),
    pause(K(KEY_ESCAPE)),
    reset(K(KEY_BACKSPACE)) {};

