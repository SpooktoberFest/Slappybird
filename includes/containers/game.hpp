#ifndef SUPERFLAPPY_GAME_HPP
#define SUPERFLAPPY_GAME_HPP

#include <vector>
#include <optional>
#include <array>

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
    void load_scene(const Scene* const scene);


    Serializer _serializer;

private:

    void handle_collision();
    void handle_input();
    void handle_action(const Action& action);
    void set_background(OptColor color1, OptColor color2);
    void update_resolution(const bool override=false);

    // Game State
    Scene _scene;
    const Scene* _scene_template;
    GameState _gamestate = GameState::INIT;

    // Window Properties
    Vector2 _resoulution = {800, 450};

    // Misc
    Shader _gradient_shader;
    ControlScheme _controls;

};




#endif // SUPERFLAPPY_GAME_HPP