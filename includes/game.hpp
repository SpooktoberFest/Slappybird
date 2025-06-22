#ifndef SUPERFLAPPY_GAME_HPP
#define SUPERFLAPPY_GAME_HPP

#include <stdlib.h>
#include <time.h>

#include <vector>
#include <optional>
#include <array>

#include "entities.hpp"

enum class GameState : u_int8_t {
    QUIT, INIT, RUNNING, PAUSED, GAMEOVER
};

class Scene {
    using OptColor = std::optional<std::array<float, 3>>;

public:
    Scene();
    virtual ~Scene();

    GameState get_gamestate() { return _gamestate; };

    void simulate();
    void render();

private:

    void handle_input();
    void reset_level();
    void handle_collision();
    void set_background(const bool update_resolution, OptColor color1, OptColor color2);

    // Loaded Entities
    Bird _bird;
    std::vector<Pipe> _pipes;
    std::vector<Button> _buttons;

    // Game State
    int _score = 0;
    GameState _gamestate = GameState::INIT;

    // Level Parameters
    float _pipe_width = 60.0f;
    float _gap_height = 150.0f;
    float _gravity = 0.5f;
    float _jump_strength = -8.0f;
    float _pipe_speed = 3.0f;

    // Window Properties
    float _screen_width_px = 800;
    float _screen_height_px = 450;
    float _px_per_m = 20;
    Shader _gradientShader;
};


#endif // SUPERFLAPPY_GAME_HPP