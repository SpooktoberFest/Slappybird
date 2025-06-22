#include "game.hpp"
#include "debug.hpp"

const static auto src = "Scene";

Scene::Scene() {
    InitWindow(_screen_width_px, _screen_height_px, "Super Flappy Kendoka Person!");
    // InitAudioDevice();
    SetTargetFPS(60);
    srand(time(0));

    _gradientShader = LoadShader(0, "resources/shaders/gradient.fs");
    set_background(true, {{0.4f, 0.4f, 0.8f}}, {{0.2f, 0.2f, 0.4f}});

    LOG_INFO(src, "Initialized Raylib Components");

    reset_level();
};

Scene::~Scene() {
    // CloseAudioDevice();
    CloseWindow();
    LOG_INFO(src, "Closed Raylib Components");
};

void Scene::render() {
    BeginDrawing();

    BeginShaderMode(_gradientShader);
    DrawRectangle(0, 0, _screen_width_px, _screen_height_px, WHITE);
    EndShaderMode();

    DrawText("Slappy Bird (Raylib)", 10, 10, 20, DARKGRAY);
    DrawRectangleRec(_bird.rect, WHITE);

    for (const auto& pipe : _pipes) {
        DrawRectangleRec(pipe.topRect, GREEN);
        DrawRectangleRec(pipe.bottomRect, GREEN);
    }

    DrawText(TextFormat("Score: %d", _score), _screen_width_px - 150, 10, 20, BLUE);

    if (_gamestate == GameState::QUIT) {
        DrawText("Game Over! Press R to Restart", _screen_width_px/2 - 160, _screen_height_px/2 - 10, 20, MAROON);
    }

    EndDrawing();
}

void Scene::set_background(const bool update_resolution, OptColor color1, OptColor color2) {

    if (update_resolution) {
        Vector2 res = {(float)GetScreenWidth(), (float)GetScreenHeight()};
        _screen_width_px = res.x;
        _screen_height_px = res.y;
        SetShaderValue(
            _gradientShader,
            GetShaderLocation(_gradientShader, "resolution"),
            &res,
            SHADER_UNIFORM_VEC2
        );
    }

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

void Scene::simulate() {

    const bool simulate = _gamestate == GameState::RUNNING;

    // Reset level
    if (IsKeyPressed(KEY_BACKSPACE)) reset_level();
    
    // Update bird
    if (IsKeyPressed(KEY_SPACE) && simulate) _bird.velocity = _jump_strength;

    _bird.velocity += _gravity;
    _bird.y += _bird.velocity;
    _bird.rect.y = _bird.y;


    if (!simulate) return;

    // Update pipes
    for (int i = 0; i < _pipes.size(); i++) {
        _pipes[i].x -= _pipe_speed;

        if (_pipes[i].x < -_pipe_width) {
            _pipes[i].x = _screen_width_px + 250;
            _pipes[i].gapY = 100 + rand() % 200;
            _pipes[i].passed = false;
        }

        _pipes[i].topRect = (Rectangle){_pipes[i].x, 0, _pipe_width, _pipes[i].gapY};
        _pipes[i].bottomRect = (Rectangle){_pipes[i].x, _pipes[i].gapY + _gap_height, _pipe_width, _screen_height_px};


        if (CheckCollisionRecs(_bird.rect, _pipes[i].topRect) ||
            CheckCollisionRecs(_bird.rect, _pipes[i].bottomRect) ||
            _bird.y < 0 || _bird.y + _bird.rect.height > _screen_height_px) {
            _gamestate = GameState::GAMEOVER;
        }

        // Scoring
        if (!_pipes[i].passed && _pipes[i].x + _pipe_width < _bird.rect.x) {
            _pipes[i].passed = true;
            _score++;
        }
    }
};


void Scene::handle_input() {}
void Scene::handle_collision() {}
void Scene::reset_level() {

    const int number_of_pipes = 5;

    _bird = { _screen_height_px/2, 0, {100, _screen_height_px/2, 34, 24} };

    _pipes.resize(number_of_pipes);
    for (std::size_t i = 0; i < number_of_pipes; i++) {
        float x = _screen_height_px + i * 250;
        float gapY = 100 + rand() % 200;
        _pipes[i].x = x;
        _pipes[i].gapY = gapY;
        _pipes[i].passed = false;
    }

    _bird.y = _screen_height_px / 2;
    _bird.velocity = 0;
    _score = 0;
    _gamestate = GameState::RUNNING;
};