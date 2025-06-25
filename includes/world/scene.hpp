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

class Scene {
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
    Vec2 _camera_velocity = {QNAN, QNAN}; // Where QNAN means follow player

    // Level State
    unsigned short _score = 0;
    Vec2 _camera_position;

    // Serialization function for cereal
    template <class Archive>
    void serialize(Archive& ar) {
        ar(_player, _pipes, _buttons, _platforms, _selected,
           _pipe_width, _gap_height, _gravity, _jump_strength, _pipe_speed,
           _camera_velocity, _score, _camera_position);
    }
};



#endif // SUPERFLAPPY_SCENE_HPP