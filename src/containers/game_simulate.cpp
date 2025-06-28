#include "game.hpp"

#include <cmath>

#include "operators.hpp"
#include "debug.hpp"

const static auto src = "Game";

void Game::simulate() {

    handle_input();

    if (_gamestate >= GameState::PAUSED) return;

    // Update Bird
    {
        Chararacter& p = _scene._player;
        p.vel.y += _scene._gravity;
        p.pos.x += p.vel.x;
        p.pos.y += p.vel.y;
    }


    if (_gamestate >= GameState::GAMEOVER) return;
    
    // Update Camera
    {
        const Vec2& cam_vel = _scene._cam_vel;
        Vec2& cam_pos = _scene._cam_pos;
        if (std::isnan(cam_vel.x))
            cam_pos.x = _scene._player.pos.x - (_res.x / 2);
        else cam_pos.x += cam_vel.x;
        if (std::isnan(cam_vel.y))
            cam_pos.y = _scene._player.pos.y - (_res.y / 2);
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
        reset_scene();
        _gamestate = GameState::RUNNING;
    }
    if (IsKeyPressed(_controls.pause)) {
        switch (_gamestate) {
        case GameState::PAUSED:
            _gamestate = GameState::RUNNING; break;
        case GameState::RUNNING:
            _gamestate = GameState::PAUSED; break;
        case GameState::GAMEOVER:
            _gamestate = GameState::PAUSED_GAMEOVER; break;
        case GameState::PAUSED_GAMEOVER:
            _gamestate = GameState::GAMEOVER; break;
        default: break;
        }
    }

    // Mouse & Buttons
    if (_scene._buttons.size() > 0 || _gamestate == GameState::PAUSED) {
        const bool paused = bool(_gamestate == GameState::PAUSED);
        auto& index =   paused ? _menus.top()._selected : _scene._selected;
        auto& buttons = paused ? _menus.top()._buttons : _scene._buttons;

        // Navigation keys
        if (IsKeyPressed(_controls.nav[LEFT]))  index += 8;
        if (IsKeyPressed(_controls.nav[RIGHT])) index -= 8;
        if (IsKeyPressed(_controls.nav[UP]))    --index;
        if (IsKeyPressed(_controls.nav[DOWN]))  ++index;
        index %= buttons.size();

        // Mouse hover
        Vector2 mouse_pos = GetMousePosition();
        for (short i = 0; i < buttons.size(); ++i) {
            if (CheckCollisionPointRec(mouse_pos, buttons[i].rect(_res))) {
                index = i;
                // Mouse click
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    handle_action(buttons[index].action);
                break;
            }
        }
        // Select key
        if (IsKeyPressed(_controls.select)) {
            handle_action(buttons[index].action);
        }
    }

    if (_gamestate > GameState::RUNNING) return;

    // Move player
    if (IsKeyPressed(_controls.jump))       _scene._player.vel.y = _scene._jump_strength;
    const Vec2 vel = {
        (IsKeyDown(_controls.move[RIGHT]) - IsKeyDown(_controls.move[LEFT])) * _scene._move_speed,
        (IsKeyDown(_controls.move[DOWN]) - IsKeyDown(_controls.move[UP])) * _scene._move_speed
    };
    if (vel.x && vel.y) {
        _scene._player.pos.x += vel.x * 0.75;
        _scene._player.pos.y += vel.y * 0.75;
    } else {
        _scene._player.pos.x += vel.x;
        _scene._player.pos.y += vel.y;
    }
}

void Game::handle_collision() {
    Chararacter& p1 = _scene._player;
    const Rectangle p1_rect = p1.rect(1_b, 2_b);

    // Check player-border collisions
    if (!CheckCollisionRecs(
        p1_rect, {_scene._cam_pos.x, _scene._cam_pos.y, _res.x, _res.y}
    )) {
        _gamestate = GameState::GAMEOVER;
    }

    // Check player-platforms collisions
    for (const auto& pf : _scene._platforms) {
        if (CheckCollisionRecs(p1_rect, pf.rect())) {
            float arr[4] = {
                pf.pos.x + pf.size.x - p1.pos.x,
                p1.pos.x + p1_rect.width - pf.pos.x,
                pf.pos.y + pf.size.y - p1.pos.y,
                p1.pos.y + p1_rect.height - pf.pos.y,
            };

            int index = 0;
            for (int i = 1; i < 4; ++i) {
                if (arr[i] < arr[index]) {
                    index = i;
                }
            }

            switch (index) {
                case 0: p1.pos.x += arr[index]; p1.vel.x = 0; break;
                case 1: p1.pos.x -= arr[index]; p1.vel.x = 0; break;
                case 2: p1.pos.y += arr[index]; p1.vel.y = 0; break;
                case 3: p1.pos.y -= arr[index]; p1.vel.y = 0; break;
            }
        }
    }

    // Update pipes
    for (int i = 0; i < _scene._pipes.size(); i++) {
        Pipe& pipe = _scene._pipes[i];

        // Reset when the left of screen
        if (pipe.pos.x < _scene._cam_pos.x - _scene._pipe_width) {
            pipe.pos.x = _res.x + 250 + _scene._cam_pos.x;
            pipe.pos.y = 100 + rand() % 200;
            pipe.passed = false;
        }

        // Check player-pipe collisions
        auto hboxes = pipe.rect(_scene._pipe_width, _scene._gap_height);
        if (CheckCollisionRecs(p1_rect, hboxes[0]) ||
            CheckCollisionRecs(p1_rect, hboxes[1])) {
            _gamestate = GameState::GAMEOVER;
        }

        // Scoring
        if (!pipe.passed &&
            pipe.pos.x + _scene._pipe_width < p1.pos.x &&
            p1.pos.y > hboxes[0].y &&
            p1.pos.y < hboxes[1].y
        ) {
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




