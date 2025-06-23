#include "game.hpp"
#include "debug.hpp"
#include "operators.hpp"

const static auto src = "Game";

Game::Game() {
    InitWindow(_resoulution.x, _resoulution.y, "Super Flappy Kendoka Person!");
    // InitAudioDevice();
    SetTargetFPS(60);

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

    BeginShaderMode(_gradient_shader);
    DrawRectangle(0, 0, _resoulution.x, _resoulution.y, WHITE);
    EndShaderMode();

    DrawText("Slappy Bird (Raylib)", 10, 10, 20, DARKGRAY);
    DrawRectangleRec(_scene.player.hitbox - _scene._camera_position, WHITE);

    // Draw Pipes
    for (const auto& pipe : _scene._pipes) {
        DrawRectangleRec(pipe.topRect - _scene._camera_position, GREEN);
        DrawRectangleRec(pipe.bottomRect - _scene._camera_position, GREEN);
    }

    // Draw Buttons
    for (const auto& button : _scene._buttons) {
        DrawRectangleRec(button.rect, WHITE);
        DrawText(button.text.c_str(), button.rect.x + 5, button.rect.y + 5, 20, DARKGRAY);
    }

    DrawText(TextFormat("Score: %d", _scene._score), _resoulution.x - 150, 10, 20, BLUE);

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

void Game::simulate() {

    handle_input();

    // Update Bird
    {
        Player& p = _scene.player;
        p.velocity.y += _scene._gravity;
        p.hitbox.x += p.velocity.x;
        p.hitbox.y += p.velocity.y;
    }

    if (_gamestate != GameState::RUNNING) return;
    
    // Update Camera
    {
        Vector2& cam_vel = _scene._camera_velocity;
        Vector2& cam_pos = _scene._camera_position;
        if (std::isnan(cam_vel.x))
            cam_pos.x = _scene.player.hitbox.x - (_resoulution.x / 2);
        else cam_pos.x += cam_vel.x;
        if (std::isnan(cam_vel.y))
            cam_pos.y = _scene.player.hitbox.y - (_resoulution.y / 2);
        else cam_pos.y += cam_vel.y;
    }

    handle_collision();
};

void Game::load_scene(const Scene* const scene) {
    _scene_template = scene;
    _scene = *_scene_template;
}

void Game::handle_input() {

    if (IsKeyPressed(_controls.reset)) {
        _scene = *_scene_template;
        _gamestate = GameState::RUNNING;
    }
    // if (IsKeyPressed(_controls.pause)) {
    //     _gamestate =  (_gamestate == GameState::PAUSED) ? GameState::RUNNING : GameState::PAUSED;
    // }

    // Mouse & Buttons
    if (_scene._buttons.size() > 0) {
        Vector2 mouse_pos = GetMousePosition();
        for (short i = 0; i < _scene._buttons.size(); ++i) {
            if (CheckCollisionPointRec(mouse_pos, _scene._buttons[i].rect)) {
                _scene._selected = i;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    handle_action(_scene._buttons[_scene._selected].action);
                }
                break;
            }
        }
        if (IsKeyPressed(_controls.select)) {
            handle_action(_scene._buttons[_scene._selected].action);
        }
    }

    if (_gamestate != GameState::RUNNING) return;

    // Move player
    if (IsKeyPressed(_controls.jump)) {
        _scene.player.velocity.y = _scene._jump_strength;
    }
    if (IsKeyDown(_controls.move_left)) {
        _scene.player.hitbox.x -= _scene.player.move_speed;
    }
    if (IsKeyDown(_controls.move_right)) {
        _scene.player.hitbox.x += _scene.player.move_speed;
    }
    if (IsKeyDown(_controls.move_up)) {
        _scene.player.hitbox.y -= _scene.player.move_speed;
    }
    if (IsKeyDown(_controls.move_down)) {
        _scene.player.hitbox.y += _scene.player.move_speed;
    }
}

void Game::handle_collision() {
    Player& p = _scene.player;
    Vector2& cam_pos{_scene._camera_position};
    const Rectangle screen_border = {cam_pos.x, cam_pos.y, _resoulution.x, _resoulution.y};

    // Check player-border collisions
    if (!CheckCollisionRecs(p.hitbox, screen_border)) {
        _gamestate = GameState::GAMEOVER;
    }

    // Update pipes
    for (int i = 0; i < _scene._pipes.size(); i++) {
        Pipe& pipe = _scene._pipes[i];

        // Reset when the left of screen
        if (pipe.x < cam_pos.x - _scene._pipe_width) {
            pipe.x = _resoulution.x + 250 + cam_pos.x;
            pipe.gapY = 100 + rand() % 200;
            pipe.passed = false;
        }

        // Update data
        pipe.topRect = (Rectangle){pipe.x, 0, _scene._pipe_width, pipe.gapY};
        pipe.bottomRect = (Rectangle){pipe.x, pipe.gapY + _scene._gap_height, _scene._pipe_width, _resoulution.y};

        // Check player-pipe collisions
        if (CheckCollisionRecs(p.hitbox, pipe.topRect) ||
            CheckCollisionRecs(p.hitbox, pipe.bottomRect)) {
            _gamestate = GameState::GAMEOVER;
        }

        // Scoring
        if (!pipe.passed && pipe.x + _scene._pipe_width < p.hitbox.x) {
            pipe.passed = true;
            _scene._score++;
        }
    }
}


void Game::handle_action(const Action& action) {
    
    // Load World Action
    if (action < NORMAL_ACTION) {
        int16_t index;
        if (action < LOAD_CANNON_WORLD) {
            // Load custom world
            index = action + LOAD_CUSTOM_WORLD;
            // load_scene();
        } else {
            // Load cannon world
            index = action + LOAD_CANNON_WORLD;
            // load_scene();
        }
        return;
    }

    // Other Actions
    switch (action)
    {
    default:
        LOG_ERROR(src, "Unknown Action: " + std::to_string(action));
        break;
    }

}