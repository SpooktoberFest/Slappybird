#ifndef SUPERFLAPPY_PROPERTIES_HPP
#define SUPERFLAPPY_PROPERTIES_HPP

#include <array>
#include <limits>

#include <cereal/types/array.hpp>

#include "forward_decl.hpp"
#include "unlock_map.hpp"
#include "enums.hpp"

#define QNAN std::numeric_limits<float>::quiet_NaN()
#define BLOCK 32.0f


// Literal for block unit
constexpr float operator"" _b(unsigned long long value) {
    return static_cast<float>(value) * BLOCK;
}

struct Vec2 {
    float x;
    float y;

    Vec2& emplace(const Vec2& other) {
        if (x == QNAN) x = other.x;
        if (y == QNAN) y = other.y;
        return *this;
    }
    operator Vector2() const;

    // (De)Serialization
    Vec2& load(const JsonRef jf, const std::string field, const float def=0);
    template <class Archive>
    void serialize(Archive& ar) { ar(x, y); };
};

struct Loadout {
    Equipment head;
    Equipment torso;
    Equipment hands;
    Equipment legs;
    Equipment weapon;
    std::array<Technique, 4> moveset;

    // (De)Serialization
    Loadout& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(head, torso, hands, legs, weapon, moveset); };
};

struct ControlScheme {
    ControlScheme();
    std::array<KeyboardKey_, 4> move;
    std::array<KeyboardKey_, 4> nav;
    KeyboardKey_ jump;
    KeyboardKey_ select;
    KeyboardKey_ pause;
    KeyboardKey_ reset;

    // (De)Serialization
    ControlScheme& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(move, nav, jump, select, pause, reset); };
};

struct Profile {
    unlock_map<Equipment> equipment;
    unlock_map<Technique> moves;

    // (De)Serialization
    Profile& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(equipment, moves); };
};

struct Action {
    ActionType type;
    u_int8_t index;

    // (De)Serialization
    Action& load(const JsonRef jf);
    template <class Archive>
    void serialize(Archive& ar) { ar(type, index); };
};


#endif // SUPERFLAPPY_PROPERTIES_HPP
