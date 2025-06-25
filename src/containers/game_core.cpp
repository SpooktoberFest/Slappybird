#include "game.hpp"

#include <cmath>

#include "operators.hpp"
#include "debug.hpp"

const static auto src = "Game";

Game::Game() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); // FLAG_FULLSCREEN_MODE
    InitWindow(_resoulution.x, _resoulution.y, "Super Flappy Kendoka Person!");
    // InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);


    _gradient_shader = LoadShader(0, "resources/shaders/gradient.fs");
    update_resolution(true);
    set_background({{0.4f, 0.4f, 0.8f}}, {{0.2f, 0.2f, 0.4f}});

    LOG_INFO(src, "Initialized Raylib Components");
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
    DrawRectangle(0, 0, _resoulution.x, _resoulution.y, WHITE);
    EndShaderMode();

    // Pipes
    for (const auto& pipe : _scene._pipes) {
        auto hboxes = pipe.box(_scene._pipe_width, _scene._gap_height);
        DrawRectangleRec(std::move(hboxes[0]) - _scene._camera_position, GREEN);
        DrawRectangleRec(std::move(hboxes[1]) - _scene._camera_position, GREEN);
    }

    // Platforms
    for (const auto& platform : _scene._platforms) {
        DrawRectangleRec(platform.rect - _scene._camera_position, BROWN);
    }

    // Buttons
    for (const auto& button : _scene._buttons) {
        DrawRectangleRec(button.rect, WHITE);
        DrawText(button.text.c_str(), button.rect.x + 5, button.rect.y + 5, 20, DARKGRAY);
    }
    if (_scene._selected < _scene._buttons.size())
        DrawRectangleLinesEx(_scene._buttons[_scene._selected].rect, 5.0f, YELLOW);

    // Player
    DrawRectangleRec(_scene._player.box(1_b, 2_b) - _scene._camera_position, WHITE);

    // Text
    DrawText(TextFormat("Score: %d", _scene._score), _resoulution.x - 150, 10, 20, BLUE);
    DrawText("Slappy Bird (Raylib)", 10, 10, 20, DARKGRAY);
    if (_gamestate == GameState::QUIT) {
        DrawText("Game Over! Press R to Restart", _resoulution.x /2 - 160, _resoulution.y /2 - 10, 20, MAROON);
    }

    EndDrawing();
}

void Game::update_resolution(const bool override) {
    Vector2 res = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    if (override || !(res == _resoulution)) {
        _resoulution = res;
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

void Game::load_scene(const Scene* const scene) {
    _scene_template = scene;
    _scene = *_scene_template;
}

