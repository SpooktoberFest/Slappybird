#pragma once

#include <optional>
#include <array>
#include <stack>

#include "enums.hpp"
#include "forward_decl.hpp"
#include "properties.hpp"
#include "entities.hpp"
#include "serializer.hpp"


class Game {
    using OptColor = std::optional<std::array<float, 3>>;

public:
    Game();
    ~Game();

    // For main
    void simulate();
    void render();
    bool is_quit() const;

private:

    //
    void reset_scene(const Scene* scene=nullptr);
    void set_background(OptColor color1, OptColor color2);
    void update_resolution(const bool override=false);

    // Handlers
    void handle_collision();
    void handle_input();
    void handle_spawning();
    void handle_entitysim();
    void handle_action(const Action& action);

    // Game State
    Scene _scene;
    Profile _profile;
    std::stack<Menu> _menus;
    GameState _gamestate = GameState::INIT;

    // Misc
    Vec2 _res = {25, 15};           // Window resolution in blocks
    float _block = 32.0f;           // Block length (fixed to 25th of window width)
    float _per_block = 1/_block;    // Reciprocal of _block
    Serializer _serializer;
    Shader_ _gradient_shader;
    ControlScheme _controls;
};
