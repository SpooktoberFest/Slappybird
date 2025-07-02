#include "entities.hpp"

#include "algorithm"

#include "raylib.h"

#include "utils.hpp"
#include "json_fwd.hpp"
#include "game.hpp"


const static auto src = "Entities";

Scene::Scene() {
    _world.player = Chararacter();
    _world.biomes = {Biome()};
    _cam_vel = {QNAN, QNAN};

    // const int number_of_pipes = 5;
    // float screen_height_px = 480;

    // _player.pos = {5_b, 10_b};

    // _pipes.resize(number_of_pipes);
    // for (std::size_t i = 0; i < number_of_pipes; i++) {
    //     float x = screen_height_px + i * 250;
    //     float gapY = 100 + rand() % 200;
    //     _pipes[i].pos.x = x;
    //     _pipes[i].pos.y = gapY;
    //     _pipes[i].passed = false;
    // }

    // _platforms.push_back(
    //     {{5_b, 15_b}, {5_b, 1_b}}
    // );

    // buttons = {
    //     Button{{20_b, 5_b}, {5_b, 2_b}, "Button 1", ActionType::TOGGLE_GROUP_1},
    //     Button{{20_b, 8_b}, {5_b, 2_b}, "Button 2", ActionType::TOGGLE_GROUP_2}
    // };
};


bool Spawner::check_predicate(const Game& context) const {
    const std::vector<Action>& actions = context.get_scene()._actions;
    return (
        actions.end() !=
        std::find_if(actions.begin(), actions.end(),
            [&](const Action& elem) {
                if (elem.type != predicate.type) return false;
                if (!use_index) return true;
                return (elem.index != predicate.index);
            }
        )
    );
}

// Rect functions
std::vector<Rectangle> ButtonList::rects(const Vector2& res) const {
    std::vector<Rectangle> rects;
    const float offset = pos < 0.0f ? (horizontal ? res.x : res.y) : 0.0f;
    Rectangle rect = {
        horizontal ? spacing : pos + offset,
        horizontal ? pos + offset : spacing,
        button_dims.x,
        button_dims.y
    };
    for (const auto& _ : buttons) {
        rects.push_back(rect);
        horizontal ?
            rect.x += button_dims.x + spacing :
            rect.y += button_dims.y + spacing;
    }
    return rects;
};

// Rect functions

Rectangle Chararacter::rect(float w, float h) const {
    return Rectangle{pos.x, pos.y, std::move(w), std::move(h)};
};
std::array<Rectangle, 2> Pipe::rect(const float w, const float h) const {
    return {Rectangle{pos.x, 0, w, pos.y}, Rectangle{pos.x, pos.y+h, w, 12_b}};
};
Rectangle Platform::rect() const {
    return Rectangle{pos.x, pos.y, size.x, size.y};
};
