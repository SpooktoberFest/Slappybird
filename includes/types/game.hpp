#pragma once

#include <optional>
#include <array>
#include <stack>

#include "utils.hpp"
#include "enums.hpp"
#include "forward_decl.hpp"
#include "properties.hpp"
#include "entities.hpp"
#include "serializer.hpp"


class Game {
    using OptColor = std::optional<std::array<float, 3>>;

public:
    Game();
    virtual ~Game();

    // Getters for Spawners
    const Scene& get_scene() const { return _scene; };
    // const GameState get_gamestate() const { return _gamestate; };
    // const Vector2 get_res() const { return _res; };
    // const Serializer& get_serializer() const { return _serializer; };
    // const Menu& get_menu() const { return _menus.top(); };
    // const ControlScheme& get_controls() const { return _controls; };

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
    Vec2 _res = {25_b, 15_b}; // Window resolution
    Serializer _serializer;
    Shader_ _gradient_shader;
    ControlScheme _controls;
};



