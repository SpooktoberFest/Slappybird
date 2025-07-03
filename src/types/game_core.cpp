#include "game.hpp"

#include <cmath>

#include "utils.hpp"
#include "debug.hpp"

#include "raylib.h"

const static auto src = "GameCore";


Game::Game() {
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE); // FLAG_FULLSCREEN_MODE
    InitWindow(_res.x, _res.y, "Super Flappy Kendoka Person!");
    // InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    _gradient_shader = LoadShader(0, "resources/shaders/gradient.fs");

    update_resolution(true);
    set_background({{0.4f, 0.4f, 0.8f}}, {{0.2f, 0.2f, 0.4f}});

    LOG_INFO(src, "Initialized Raylib Components");

    _serializer.loadScene("profile_select.scene");
    {
        Menu menu;
        _serializer.loadMenu("pause.menu", &menu);
        _menus.push(std::move(menu));
    }
    reset_scene();

    LOG_INFO(src, "Loaded Resources");
};

Game::~Game() {
    // CloseAudioDevice();
    CloseWindow();
    LOG_INFO(src, "Closed Raylib Components");
};


void Game::simulate() {
    handle_input();
    if (_gamestate & GameState::PAUSED) return;
    handle_entitysim();
    if (_gamestate & GameState::GAMEOVER) return;
    handle_spawning();
    handle_collision();
    _scene._actions.clear();
};

void Game::render() {
    update_resolution();
    BeginDrawing();

    // Background
    BeginShaderMode(_gradient_shader);
    DrawRectangle(0, 0, _res.x, _res.y, WHITE);
    EndShaderMode();

    const bool paused = _gamestate & GameState::PAUSED;
    Biome& b = _scene._world.biomes[0];

    // Pipes
    for (const auto& pipe : _scene._world.pipes) {
        auto hitboxes = pipe.rect(b.pipe_width, b.gap_height);
        DrawRectangleRec(std::move(hitboxes[0]) - _scene._cam_pos, GREEN);
        DrawRectangleRec(std::move(hitboxes[1]) - _scene._cam_pos, GREEN);
    }

    // Platforms
    for (const auto& platform : _scene._world.platforms) {
        DrawRectangleRec(platform.rect() - _scene._cam_pos, BROWN);
    }

    // Scene Buttons
    if (_scene._world.menu && !_scene._world.menu->buttons.empty()) {
        const Menu& menu = *_scene._world.menu;
        std::size_t i, j;
        for (i=0 ; i < menu.buttons.size() ; ++i) {
            const ButtonList& button_list = menu.buttons[i];
            if (button_list.buttons.empty()) continue;
            const auto hitboxes = button_list.rects(_res);
            for (j=0 ; j < hitboxes.size() ; ++j) {
                const Rectangle hitbox = hitboxes[j];
                DrawRectangleRec(hitbox, (paused ? GRAY : WHITE));
                DrawText(button_list.buttons[j].text.c_str(), hitbox.x + 5, hitbox.y + 5, 20, DARKGRAY);
            }

            if (menu.index == i && !paused)
                DrawRectangleLinesEx(hitboxes[button_list.index], 5.0f, YELLOW);
        }
    }

    // Menu Buttons
    if (paused && !_menus.empty() && !_menus.top().buttons.empty()) {
        const Menu& menu = _menus.top();
        std::size_t i, j;
        for (i=0 ; i < menu.buttons.size() ; ++i) {
            const ButtonList& button_list = menu.buttons[i];
            if (button_list.buttons.empty()) continue; 
            const auto hitboxes = button_list.rects(_res);
            for (j=0 ; j < hitboxes.size() ; ++j) {
                const Rectangle hitbox = hitboxes[j];
                DrawRectangleRec(hitbox, WHITE);
                DrawText(button_list.buttons[j].text.c_str(), hitbox.x + 5, hitbox.y + 5, 20, DARKGRAY);
            }
            if (menu.index == i)
                DrawRectangleLinesEx(hitboxes[button_list.index], 5.0f, YELLOW);
        }
    }

    // Player
    DrawRectangleRec(_scene._world.player->rect(1_b, 2_b) - _scene._cam_pos, WHITE);

    // Text
    DrawText(TextFormat("Score: %d", _scene._score), _res.x - 150, 10, 20, BLUE);
    DrawText("Slappy Bird (Raylib)", 10, 10, 20, DARKGRAY);
    if (_gamestate & GameState::GAMEOVER) {
        DrawText("Game Over! Press R to Restart", _res.x /2 - 160, _res.y /2 - 10, 20, MAROON);
    }

    EndDrawing();
}


void Game::update_resolution(const bool override) {
    Vector2 res = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    if (override || !(res == _res)) {
        _res = {res.x, res.y};
        SetShaderValue(
            _gradient_shader,
            GetShaderLocation(_gradient_shader, "resolution"),
            &res,
            SHADER_UNIFORM_VEC2
        );
    }
}

void Game::set_background(OptColor color1, OptColor color2) {
    if (color1) {
    SetShaderValue(
        _gradient_shader,
        GetShaderLocation(_gradient_shader, "topColor"),
        (float[4]){color1->at(0), color1->at(1), color1->at(2), 1.0f},
        SHADER_UNIFORM_VEC4);
    }
    if (color2) {
    SetShaderValue(
        _gradient_shader,
        GetShaderLocation(_gradient_shader, "bottomColor"),
        (float[4]){color2->at(0), color2->at(1), color2->at(2), 1.0f},
        SHADER_UNIFORM_VEC4);
    }
}

void Game::reset_scene(const Scene* scene) {
    if (scene) _serializer.loaded_scene = *scene;
    _scene = _serializer.loaded_scene;
    handle_action({ActionType::SHOW_PROFILES});
}

bool Game::is_quit() const {
    return _gamestate & GameState::QUIT;
}


