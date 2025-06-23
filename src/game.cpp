#include "game.hpp"
#include "debug.hpp"
#include "operators.hpp"

const static auto src = "Game";

Game::Game() {
    InitWindow(resoulution.x, resoulution.y, "Super Flappy Kendoka Person!");
    // InitAudioDevice();
    SetTargetFPS(60);

    _gradientShader = LoadShader(0, "resources/shaders/gradient.fs");
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

    BeginShaderMode(_gradientShader);
    DrawRectangle(0, 0, resoulution.x, resoulution.y, WHITE);
    EndShaderMode();

    DrawText("Slappy Bird (Raylib)", 10, 10, 20, DARKGRAY);
    DrawRectangleRec(_scene.player.hitbox - _scene._camera_position, WHITE);

    for (const auto& pipe : _scene._pipes) {
        DrawRectangleRec(pipe.topRect - _scene._camera_position, GREEN);
        DrawRectangleRec(pipe.bottomRect - _scene._camera_position, GREEN);
    }

    // LOG_DEBUG(src, 
    //     "cam_vel: " << std::to_string(_scene._camera_velocity.x) << \
    //     ", _camera_position: " << std::to_string(_camera_position.x) << \
    //     ", player: " << std::to_string(_scene.player.hitbox.x) << \
    //     ",\n player render: " << std::to_string(_scene.player.hitbox.x - _camera_position.x) << \
    //     ", pipe render: " << std::to_string(_scene._pipes[0].bottomRect.x - _camera_position.x)
    // );

    DrawText(TextFormat("Score: %d", _scene._score), resoulution.x - 150, 10, 20, BLUE);

    if (_gamestate == GameState::QUIT) {
        DrawText("Game Over! Press R to Restart", resoulution.x /2 - 160, resoulution.y /2 - 10, 20, MAROON);
    }

    EndDrawing();
}

void Game::update_resolution(const bool override) {
    Vector2 res = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    if (override || !(res == resoulution)) {
        resoulution = res;
        SetShaderValue(
            _gradientShader,
            GetShaderLocation(_gradientShader, "resolution"),
            &res,
            SHADER_UNIFORM_VEC2
        );
    }
}

void Game::set_background(OptColor color1, OptColor color2) {
    if (color1) {
    SetShaderValue(
        _gradientShader,
        GetShaderLocation(_gradientShader, "topColor"),
        (float[4]){color1->at(0), color1->at(1), color1->at(2), 1.0f},
        SHADER_UNIFORM_VEC4);
    }
    if (color2) {
    SetShaderValue(
        _gradientShader,
        GetShaderLocation(_gradientShader, "bottomColor"),
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

    // Update Camera
    {
        Vector2& cam_vel = _scene._camera_velocity;
        Vector2& cam_pos = _scene._camera_position;
        if (std::isnan(cam_vel.x))
            cam_pos.x = _scene.player.hitbox.x - (resoulution.x / 2);
        else cam_pos.x += cam_vel.x;
        if (std::isnan(cam_vel.y))
            cam_pos.y = _scene.player.hitbox.y - (resoulution.y / 2);
        else cam_pos.y += cam_vel.y;
    }

    if (_gamestate == GameState::RUNNING) handle_collision();
};

void Game::load_scene(const Scene* const scene) {
    _scene_template = scene;
    _scene = *_scene_template;
}

void Game::handle_input() {

    switch (_gamestate)
    {
    case GameState::RUNNING:
        // Jump
        if (IsKeyPressed(KEY_SPACE)) {
            _scene.player.velocity.y = _scene._jump_strength;
        }
        if (IsKeyDown(KEY_LEFT)) {
            _scene.player.hitbox.x -= _scene.player.move_speed;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            _scene.player.hitbox.x += _scene.player.move_speed;
        }
        if (IsKeyDown(KEY_UP)) {
            _scene.player.hitbox.y -= _scene.player.move_speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            _scene.player.hitbox.y += _scene.player.move_speed;
        }
        [[fallthrough]];


    default:
        // Reset level
        if (IsKeyPressed(KEY_BACKSPACE)) {
            _scene = *_scene_template;
            _gamestate = GameState::RUNNING;
        }        
        break;
    }
}

void Game::handle_collision() {
    Player& p = _scene.player;
    Vector2& cam_pos{_scene._camera_position};

    // Update pipes
    for (int i = 0; i < _scene._pipes.size(); i++) {
        Pipe& pipe = _scene._pipes[i];

        // Reset when the left of screen
        if (pipe.x < cam_pos.x - _scene._pipe_width) {
            pipe.x = resoulution.x + 250 + cam_pos.x;
            pipe.gapY = 100 + rand() % 200;
            pipe.passed = false;
        }

        // Update data
        pipe.topRect = (Rectangle){pipe.x, 0, _scene._pipe_width, pipe.gapY};
        pipe.bottomRect = (Rectangle){pipe.x, pipe.gapY + _scene._gap_height, _scene._pipe_width, resoulution.y};

        // Check pipe-bird and bird-border collisions
        if (CheckCollisionRecs(p.hitbox, pipe.topRect) ||
            CheckCollisionRecs(p.hitbox, pipe.bottomRect) ||
            p.hitbox.y < 0 || p.hitbox.y + p.hitbox.height > resoulution.y) {
            _gamestate = GameState::GAMEOVER;
        }

        // Scoring
        if (!pipe.passed && pipe.x + _scene._pipe_width < p.hitbox.x) {
            pipe.passed = true;
            _scene._score++;
        }
    }
}
