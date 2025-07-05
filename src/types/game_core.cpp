#include "game.hpp"

#include <cmath>

#include "utils.hpp"
#include "debug.hpp"

#include "raylib.h"


#define R(rectangle) ((rectangle - _scene._cam_pos) * _block)

const static auto src = "GameCore";


Game::Game() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); // FLAG_FULLSCREEN_MODE
    InitWindow(_res.x * _block, _res.y * _block, "Super Flappy Kendoka Person!");
    // InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    _gradient_shader = LoadShader(0, "resources/shaders/gradient.fs");

    update_resolution(true);
    set_background({{0.4f, 0.4f, 0.8f}}, {{0.2f, 0.2f, 0.4f}});

    LOG_INFO("Initialized Raylib Components");

    _serializer.loadScene("profile_select.scene");
    {
        Menu menu;
        _serializer.loadMenu("pause.menu", &menu);
        _menus.push(std::move(menu));
    }
    reset_scene();

    LOG_INFO("Loaded Resources");
};

Game::~Game() {
    // CloseAudioDevice();
    CloseWindow();
    LOG_INFO("Closed Raylib Components");
};


void Game::simulate() {
    handle_input();
    if (check_flag(_gamestate, GameState::PAUSED)) return;
    handle_entitysim();
    if (check_flag(_gamestate, GameState::GAMEOVER)) return;
    handle_spawning();
    handle_collision();
    _scene._actions.clear();
};

void Game::render() {
    update_resolution();
    BeginDrawing();

    // Background
    BeginShaderMode(_gradient_shader);
    DrawRectangle(0, 0, _res.x * _block, _res.y * _block, WHITE);
    EndShaderMode();

    const bool paused = check_flag(_gamestate, GameState::PAUSED);
    Biome& b = _scene._world.biomes[0];

    // Pipes
    for (const auto& pipe : _scene._world.pipes) {
        auto hitboxes = pipe.get_hitbox(b.pipe_width, b.gap_height);
        DrawRectangleRec(R(std::move(hitboxes[0])), GREEN);
        DrawRectangleRec(R(std::move(hitboxes[1])), GREEN);
    }

    // Platforms
    for (const auto& platform : _scene._world.platforms) {
        DrawRectangleRec(R(platform.get_hitbox()), BROWN);
    }

    // Scene Buttons
    if (_scene._world.menu && !_scene._world.menu->buttons.empty()) {
        const Menu& menu = *_scene._world.menu;
        std::size_t i, j;
        for (i=0 ; i < menu.buttons.size() ; ++i) {
            const ButtonList& button_list = menu.buttons[i];
            if (button_list.buttons.empty()) continue;
            auto hitboxes = button_list.get_hitboxes(_res);
            for (j=0 ; j < hitboxes.size() ; ++j) {
                Rectangle& hitbox = hitboxes[j];
                hitbox = hitbox * _block;
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
            auto hitboxes = button_list.get_hitboxes(_res);
            for (j=0 ; j < hitboxes.size() ; ++j) {
                Rectangle& hitbox = hitboxes[j];
                hitbox = hitbox * _block;
                DrawRectangleRec(hitbox, WHITE);
                DrawText(button_list.buttons[j].text.c_str(), hitbox.x + 5, hitbox.y + 5, 20, DARKGRAY);
            }
            if (menu.index == i)
                DrawRectangleLinesEx(hitboxes[button_list.index], 5.0f, YELLOW);
        }
    }

    // Player
    DrawRectangleRec(R(_scene._world.player->get_hitbox(1, 2)), WHITE);

    // Text
    DrawText(TextFormat("Score: %d", _scene._score), _res.x * _block - 150, 10, 20, BLUE);
    DrawText("Slappy Bird (Raylib)", 10, 10, 20, DARKGRAY);
    if (check_flag(_gamestate, GameState::GAMEOVER)) {
        DrawText(
            "Game Over! Press Backspace to Restart",
            _res.x * _block /2 - 160,
            _res.y * _block /2 - 10,
            20, MAROON);
    }

    EndDrawing();
}


void Game::update_resolution(const bool override) {
    Vector2 res = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    if (override || !(res == (_res * _block))) {
        _block = res.x / 25;
        _per_block = 1 / _block;
        _res.y = res.x * _per_block;
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
    return check_flag(_gamestate, GameState::QUIT);
}


