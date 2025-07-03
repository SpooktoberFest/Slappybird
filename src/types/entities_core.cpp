#include "entities.hpp"

#include "algorithm"

#include "raylib.h"

#include "debug.hpp"
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


void Menu::clamp() {
    if (buttons.empty()) return;
    index = branchless_ternary(
        uint8_t(index < 200),
        uint8_t(index % buttons.size()),
        uint8_t(buttons.size() + index)
    );
}
void ButtonList::clamp() {
    if (buttons.empty()) return;
    index = branchless_ternary(
        uint8_t(index < 200),
        uint8_t(index % buttons.size()),
        uint8_t(buttons.size() + index)
    );
}


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

void ButtonList::load_buttons(const std::vector<std::string>& str_vec, ActionType type) {
    buttons.clear();
    for (std::size_t i=0 ; i < str_vec.size() ; ++i)
        buttons.push_back(Button{str_vec[i], Action{type, uint8_t(i)}});
}


// Rect functions
std::vector<Rectangle> ButtonList::rects(const Vector2& res) const {
    std::vector<Rectangle> rects;
    const float offset = pos < 0.0f ? (horizontal ? res.y : res.x) : 0.0f;
    Rectangle rect = {
        horizontal ? begin : pos + offset,
        horizontal ? pos + offset : begin,
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

Rectangle Chararacter::rect(float w, float h) const {
    return Rectangle{pos.x, pos.y, std::move(w), std::move(h)};
};
std::array<Rectangle, 2> Pipe::rect(const float w, const float h) const {
    return {Rectangle{pos.x, 0, w, pos.y}, Rectangle{pos.x, pos.y+h, w, 12_b}};
};
Rectangle Platform::rect() const {
    return Rectangle{pos.x, pos.y, size.x, size.y};
};
