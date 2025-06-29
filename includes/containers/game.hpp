#ifndef SUPERFLAPPY_GAME_HPP
#define SUPERFLAPPY_GAME_HPP

#include <vector>
#include <optional>
#include <array>
#include <stack>

#include "entities.hpp"
#include "scene.hpp"
#include "serializer.hpp"


class Game {
    using OptColor = std::optional<std::array<float, 3>>;

    enum GameState : u_int8_t {
        RUNNING, GAMEOVER, PAUSED, PAUSED_GAMEOVER, INIT, QUIT
    };

public:
    Game();
    virtual ~Game();

    GameState get_gamestate() { return _gamestate; };

    void simulate();
    void render();
    void reset_scene(const Scene* scene=nullptr);
    bool is_quit() const;

    Serializer _serializer;

private:

    void handle_collision();
    void handle_input();
    void handle_spawning();
    void handle_action(const Action& action);
    void set_background(OptColor color1, OptColor color2);
    void update_resolution(const bool override=false);

    // Game State
    Scene _scene;
    std::stack<Menu> _menus;
    GameState _gamestate = GameState::INIT;

    // Window Properties
    Vector2 _res = {800, 480}; // Window resolution {25_b, 15_b}

    // Misc
    Shader _gradient_shader;
    ControlScheme _controls;

};




#endif // SUPERFLAPPY_GAME_HPP