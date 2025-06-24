#include "game.hpp"

#include <cmath>

#include "operators.hpp"
#include "debug.hpp"

const static auto src = "Game";

void Game::simulate() {

    handle_input();

    if (_gamestate > GameState::PAUSED) return;


    if (_gamestate > GameState::GAMEOVER) return;

    // Update Bird
    {
        Player& p = _scene.player;
        p.velocity.y += _scene._gravity;
        p.hitbox.x += p.velocity.x;
        p.hitbox.y += p.velocity.y;
    }


    if (_gamestate > GameState::RUNNING) return;
    
    // Update Camera
    {
        Vec2& cam_vel = _scene._camera_velocity;
        Vec2& cam_pos = _scene._camera_position;
        if (std::isnan(cam_vel.x))
            cam_pos.x = _scene.player.hitbox.x - (_resoulution.x / 2);
        else cam_pos.x += cam_vel.x;
        if (std::isnan(cam_vel.y))
            cam_pos.y = _scene.player.hitbox.y - (_resoulution.y / 2);
        else cam_pos.y += cam_vel.y;
    }

    handle_collision();
};

void Game::handle_input() {
    /*
    for (int keycode = GetKeyPressed() ; keycode != 0 ; keycode = GetKeyPressed()) {
        switch (keycode)
        {
        case _controls.jump:
            break;
        default:
            break;
        }
    }
    */

    if (IsKeyPressed(_controls.reset)) {
        _scene = *_scene_template;
        _gamestate = GameState::RUNNING;
    }
    if (IsKeyPressed(_controls.pause)) {
        switch (_gamestate) {
        case GameState::PAUSED:
            _gamestate = GameState::RUNNING;
            break;
        case GameState::RUNNING:
            _gamestate = GameState::PAUSED;
            break;
        case GameState::GAMEOVER:
            _gamestate = GameState::PAUSED_GAMEOVER;
            break;
        case GameState::PAUSED_GAMEOVER:
            _gamestate = GameState::GAMEOVER;
            break;
        default:
            break;
        }
    }

    // Mouse & Buttons
    if (_scene._buttons.size() > 0) {
        _scene._selected %= _scene._buttons.size();
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

    if (_gamestate > GameState::RUNNING) return;

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
    Vec2& cam_pos{_scene._camera_position};
    const Rectangle screen_border = {cam_pos.x, cam_pos.y, _resoulution.x, _resoulution.y};

    // Check player-border collisions
    if (!CheckCollisionRecs(p.hitbox, screen_border)) {
        _gamestate = GameState::GAMEOVER;
    }

    // Check player-platforms collisions
    for (const auto& pf : _scene._platforms) {
        if (CheckCollisionRecs(p.hitbox, pf.rect)) {
            float arr[4] = {
                pf.rect.x + pf.rect.w - p.hitbox.x,
                p.hitbox.x + p.hitbox.w - pf.rect.x,
                pf.rect.y + pf.rect.h - p.hitbox.y,
                p.hitbox.y + p.hitbox.h - pf.rect.y,
            };

            int index = 0;
            for (int i = 1; i < 4; ++i) {
                if (arr[i] < arr[index]) {
                    index = i;
                }
            }

            switch (index) {
                case 0: p.hitbox.x += arr[index]; p.velocity.x = 0; break;
                case 1: p.hitbox.x -= arr[index]; p.velocity.x = 0; break;
                case 2: p.hitbox.y += arr[index]; p.velocity.y = 0; break;
                case 3: p.hitbox.y -= arr[index]; p.velocity.y = 0; break;
            }
        }
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
        pipe.topRect = (Rect){pipe.x, 0, _scene._pipe_width, pipe.gapY};
        pipe.bottomRect = (Rect){pipe.x, pipe.gapY + _scene._gap_height, _scene._pipe_width, _resoulution.y};

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
            LOG_INFO(src, "Loading Custom World: " + std::to_string(index));
            // load_scene();
        } else {
            // Load cannon world
            index = action + LOAD_CANNON_WORLD;
            LOG_INFO(src, "Loading Cannon World: " + std::to_string(index));
            // load_scene();
        }
        return;
    }

    // Other Actions
    switch (action)
    {
    default:
        LOG_WARN(src, "Unknown Action: " + std::to_string(action));
        break;
    }

}




