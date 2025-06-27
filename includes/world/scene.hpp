#ifndef SUPERFLAPPY_SCENE_HPP
#define SUPERFLAPPY_SCENE_HPP

#include <stdlib.h>
#include <limits>
#include <vector>
#include <optional>
#include <array>

#include <cereal/types/vector.hpp>

#include "entities.hpp"

#define QNAN std::numeric_limits<float>::quiet_NaN()

class Serializer; // Forward declaration


class Scene {
    friend class Serializer;
public:
    Scene();
    virtual ~Scene() {};

    // Loaded Entities
    Chararacter _player;
    std::vector<Pipe> _pipes;
    std::vector<Button> _buttons;
    std::vector<Platform> _platforms;

    unsigned short _selected;

    // Level Parameters
    float _pipe_width = 2_b;
    float _gap_height = 150.0f;
    float _gravity = 0.5f;
    float _jump_strength = -8.0f;
    float _pipe_speed = 3.0f;
    float _move_speed = 5.0f;
    Vec2 _cam_vel = {QNAN, QNAN}; // Where QNAN means follow player
    Vec2 _cam_pos;
    unsigned short _score = 0;

    // Serialization function for cereal
    template <class Archive>
    void serialize(Archive& ar) {
        ar(
            _player, _pipes, _buttons, _platforms,
           _pipe_width, _gap_height, _gravity, _jump_strength, _pipe_speed,
           _cam_vel, _cam_pos, _score
        );
    }
};



#endif // SUPERFLAPPY_SCENE_HPP