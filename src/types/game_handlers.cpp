#include "game.hpp"

#include <cmath>

#include "utils.hpp"
#include "debug.hpp"

#include "raylib.h"

const static auto src = "GameHandlers";


void Game::handle_entitysim() {

    Chararacter& player = *_scene._world.player;

    // Update Player
    {
        player.vel.y = (player.vel.y + _scene._world.biomes[0].gravity);
        player.pos += player.vel * _per_block;
    }

    // Update Enemies
    for (Chararacter& enemy : _scene._world.enemies)
        enemy.pos += enemy.vel;


    if (check_flag(_gamestate, GameState::GAMEOVER)) return;

    
    // Update Camera
    {
        const Vec2& cam_vel = _scene._cam_vel;
        Vec2& cam_pos = _scene._cam_pos;
        if (std::isnan(cam_vel.x))
            cam_pos.x = player.pos.x - (_res.x / 2);
        else cam_pos.x += cam_vel.x;
        if (std::isnan(cam_vel.y))
            cam_pos.y = player.pos.y - (_res.y / 2);
        else cam_pos.y += cam_vel.y;
    }

}

void Game::handle_spawning() {
    for (const auto& spawner : _scene._world.spawners) {
        if (spawner.check_predicate(*this)) {
            World spawn_in = spawner.spawn_in;
            // Optionals
            if (spawn_in.player) {
                spawn_in.player->pos.emplace(spawner.pos);
                _scene._world.player = *spawn_in.player;
            }
            if (spawn_in.menu) {
                _scene._world.menu = *spawn_in.menu;
            }
            // Vectors
            for (auto& spawn : spawn_in.enemies) {
                spawn.pos.emplace(spawner.pos);
                _scene._world.enemies.push_back(std::move(spawn));
            }
            for (auto& spawn : spawn_in.spawners) {
                spawn.pos.emplace(spawner.pos);
                _scene._world.spawners.push_back(std::move(spawn));
            }
            for (auto& spawn : spawn_in.pipes) {
                spawn.pos.emplace(spawner.pos);
                _scene._world.pipes.push_back(std::move(spawn));
            }
            for (auto& spawn : spawn_in.platforms) {
                spawn.pos.emplace(spawner.pos);
                _scene._world.platforms.push_back(std::move(spawn));
            }
            for (auto& spawn : spawn_in.biomes) {
                spawn.pos.emplace(spawner.pos);
                _scene._world.biomes.push_back(std::move(spawn));
            }
        }
    }
}

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
        _gamestate = GameState::INIT;
    }
    if (IsKeyPressed(_controls.pause)) {
        if (check_flag(_gamestate, GameState::PAUSED) && _menus.size() > 1U) _menus.pop();
        else flip_flag(_gamestate, GameState::PAUSED);
    }

    // Mouse & Buttons
    if (check_flag(_gamestate, GameState::PAUSED) ?
        bool(!_menus.empty() && !_menus.top().buttons.empty()) :
        bool(_scene._world.menu && !_scene._world.menu->buttons.empty())) {

        // LOG_DEBUG(
        //     "\n\n\n  Game paused: " + std::to_string(check_flag(_gamestate, GameState::PAUSED) + 
        //     ", Pause menus exist: " + std::to_string(!_menus.empty()) + 
        //     ", World menus exist: " + std::to_string(_scene._world.menu == std::nullopt));

        Menu& smenu = check_flag(_gamestate, GameState::PAUSED) ? _menus.top() : *_scene._world.menu;


        // Navigation keys
        {
            smenu.clamp();
            ButtonList* sblist = &smenu.buttons[smenu.index];
            uint8_t nav_x = IsKeyPressed(_controls.nav[RIGHT]) - IsKeyPressed(_controls.nav[LEFT]);
            uint8_t nav_y = IsKeyPressed(_controls.nav[DOWN]) - IsKeyPressed(_controls.nav[UP]);
            if (sblist->horizontal) std::swap(nav_x, nav_y);

            sblist->index += nav_y;
            smenu.index += nav_x;
            sblist->clamp();
            smenu.clamp();

            if (nav_y != 0) {
                // Update selected button list
                sblist = &smenu.buttons[smenu.index];
                // Carry index if they have same orientation
                const bool same_orientation =
                    (sblist->horizontal == smenu.buttons[smenu.index].horizontal);
                sblist->index = branchless_ternary(
                    same_orientation,
                    smenu.buttons[smenu.index].index,
                    sblist->index
                );
                // Clamp new button list
                sblist->clamp();
            }
        }

        // Mouse hover
        Vector2 mouse_pos = GetMousePosition() * _per_block;
        uint8_t i, j;
        bool hit = false;
        for (i=0; !hit && i < smenu.buttons.size(); ++i) {
            const auto hitboxes = smenu.buttons[i].rects(_res);
            for (j=0; j < hitboxes.size(); ++j) {
                const Rectangle& hitbox = hitboxes[j];
                if (CheckCollisionPointRec(mouse_pos, hitbox)) {
                    hit = true;
                    smenu.index = i;
                    smenu.buttons[i].index = j;

                    // Mouse click
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        handle_action(smenu.buttons[i].buttons[j].action);
                    break;
                }
            }
        }

        // Select key
        if (IsKeyPressed(_controls.select)) {            
            LOG_DEBUG("smenu.index: " + std::to_string(smenu.index));
            const ButtonList& sblist = smenu.buttons[smenu.index];
            LOG_DEBUG("sblist.index: " + std::to_string(sblist.index));
            handle_action(sblist.buttons[sblist.index].action);
        }
    }

    if (check_flag(_gamestate, GameState(PAUSED | GAMEOVER))) return;

    // Move player
    Chararacter& p1 = *_scene._world.player;
    Biome& b = _scene._world.biomes[0];
    if (IsKeyPressed(_controls.jump)) p1.vel.y = b.jump_strength;
    const float move_speed = b.move_speed * _per_block;
    const Vec2 vel = {
        (IsKeyDown(_controls.move[RIGHT]) - IsKeyDown(_controls.move[LEFT])) * move_speed,
        (IsKeyDown(_controls.move[DOWN]) - IsKeyDown(_controls.move[UP])) * move_speed
    };
    if (vel.x && vel.y) {
        p1.pos.x += vel.x * 0.75;
        p1.pos.y += vel.y * 0.75;
    } else {
        p1.pos.x += vel.x;
        p1.pos.y += vel.y;
    }
}

void Game::handle_collision() {
    Chararacter& p1 = *_scene._world.player;
    Biome& b = _scene._world.biomes[0];
    const Rectangle p1_rect = p1.rect(1, 2);

    // Check player-border collisions
    if (!CheckCollisionRecs(
        p1_rect, {_scene._cam_pos.x, _scene._cam_pos.y, _res.x, _res.y}
    )) {
        set_flag(_gamestate, GameState::GAMEOVER);
    }

    // Check player-platforms collisions
    for (const auto& pf : _scene._world.platforms) {
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
    for (int i = 0; i < _scene._world.pipes.size(); i++) {
        Pipe& pipe = _scene._world.pipes[i];

        // Reset when the left of screen
        if (pipe.pos.x < _scene._cam_pos.x - b.pipe_width) {
            pipe.pos.x = _res.x + 250 + _scene._cam_pos.x;
            pipe.pos.y = 100 + rand() % 200;
            pipe.passed = false;
        }

        // Check player-pipe collisions
        auto hboxes = pipe.rect(b.pipe_width, b.gap_height);
        if (CheckCollisionRecs(p1_rect, hboxes[0]) ||
            CheckCollisionRecs(p1_rect, hboxes[1]))
            set_flag(_gamestate, GameState::GAMEOVER);

        // Scoring
        if (!pipe.passed &&
            pipe.pos.x + b.pipe_width < p1.pos.x &&
            p1.pos.y > hboxes[0].y &&
            p1.pos.y < hboxes[1].y
        ) {
            pipe.passed = true;
            _scene._score++;
        }
    }
}

void Game::handle_action(const Action& action) {
    // Other Actions
    switch (action.type) {
    case ActionType::LOAD_WORLD:
        LOG_INFO("Loaded World");
        break;
    case ActionType::EXIT_GAME:
        LOG_INFO("Exiting Game");
        _gamestate = GameState::QUIT;
        break;

    case ActionType::SHOW_PROFILES: {
        LOG_INFO("Fetching Profiles");
        const auto profile_names = _serializer.get_files(_serializer.profiles_path, ".profile");
        if (_scene._world.menu)
            for (auto& blist : _scene._world.menu->buttons)
            if (blist.special_content == Type::PROFILE)
            blist.load_buttons(profile_names, ActionType::LOAD_PROFILE);
        if (!_menus.empty())
            for (auto& blist : _menus.top().buttons)
            if (blist.special_content == Type::PROFILE)
            blist.load_buttons(profile_names, ActionType::LOAD_PROFILE);
        } break;

    case ActionType::LOAD_PROFILE:
        LOG_INFO("Loading Profile");
        _serializer.loadProfile(
            _serializer.get_files(_serializer.profiles_path, ".profile")[action.index],
            &_profile);
        break;
    default:
        LOG_WARN("Unknown ActionType: " + std::to_string(uint8_t(action.type)));
        break;
    }

    _scene._actions.push_back(action);
}




