#include "game.hpp"

#include <cmath>

#include "operators.hpp"
#include "debug.hpp"

const static auto src = "Game";

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

    {
        Menu menu;
        _serializer.loadMenu("pause.menu", &menu);
        _menus.push(std::move(menu));
    }

    reset_scene();

    LOG_INFO(src, "Loaded Resources");
    _gamestate = GameState::RUNNING;
};

Game::~Game() {
    // CloseAudioDevice();
    CloseWindow();
    LOG_INFO(src, "Closed Raylib Components");
};

void Game::render() {
    update_resolution();
    BeginDrawing();

    // Background
    BeginShaderMode(_gradient_shader);
    DrawRectangle(0, 0, _res.x, _res.y, WHITE);
    EndShaderMode();

    // Pipes
    for (const auto& pipe : _scene._pipes) {
        auto hitboxes = pipe.rect(_scene._pipe_width, _scene._gap_height);
        DrawRectangleRec(std::move(hitboxes[0]) - _scene._cam_pos, GREEN);
        DrawRectangleRec(std::move(hitboxes[1]) - _scene._cam_pos, GREEN);
    }

    // Platforms
    for (const auto& platform : _scene._platforms) {
        DrawRectangleRec(platform.rect() - _scene._cam_pos, BROWN);
    }

    // Scene Buttons
    const bool is_running = _gamestate == GameState::RUNNING;
    for (const auto& button : _scene._buttons) {
        DrawRectangleRec(button.rect(_res), (is_running ? WHITE : GRAY));
        DrawText(button.text.c_str(), button.pos.x + 5, button.pos.y + 5, 20, DARKGRAY);
    } if (is_running) {
        if (_scene._selected < _scene._buttons.size())
            DrawRectangleLinesEx(_scene._buttons[_scene._selected].rect(_res), 5.0f, YELLOW);
    }
    // Menu Buttons
    else {
        const Menu& menu = _menus.top();
        for (const auto& button : menu._buttons) {
            const auto hitbox = button.rect(_res);
            DrawRectangleRec(hitbox, WHITE);
            DrawText(button.text.c_str(), hitbox.x + 5, hitbox.y + 5, 20, DARKGRAY);
        }
        if (menu._selected < menu._buttons.size())
            DrawRectangleLinesEx(menu._buttons[menu._selected].rect(_res), 5.0f, YELLOW);
    }

    // Player
    DrawRectangleRec(_scene._player.rect(1_b, 2_b) - _scene._cam_pos, WHITE);

    // Text
    DrawText(TextFormat("Score: %d", _scene._score), _res.x - 150, 10, 20, BLUE);
    DrawText("Slappy Bird (Raylib)", 10, 10, 20, DARKGRAY);
    if (_gamestate == GameState::QUIT) {
        DrawText("Game Over! Press R to Restart", _res.x /2 - 160, _res.y /2 - 10, 20, MAROON);
    }

    EndDrawing();
}

void Game::update_resolution(const bool override) {
    Vector2 res = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    if (override || !(res == _res)) {
        _res = res;
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
}

