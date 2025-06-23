#ifndef SUPERFLAPPY_SCENE_HPP
#define SUPERFLAPPY_SCENE_HPP

#include <stdlib.h>
#include <time.h>
#include <cmath>

#include <vector>
#include <optional>
#include <array>

#include "entities.hpp"

#define NANF std::nanf("")

class Scene {
public:
    Scene();
    virtual ~Scene() {};

    // Loaded Entities
    Player player;
    std::vector<Pipe> _pipes;
    std::vector<Button> _buttons;

    // Level Parameters
    float _pipe_width = 60.0f;
    float _gap_height = 150.0f;
    float _gravity = 0.5f;
    float _jump_strength = -8.0f;
    float _pipe_speed = 3.0f;
    Vector2 _camera_velocity = {NANF, 0.0f}; // Where NANF means follow player

    // Level State
    unsigned short _score = 0;
    Vector2 _camera_position;
};


#endif // SUPERFLAPPY_SCENE_HPP