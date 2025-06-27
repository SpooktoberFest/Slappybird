#include "game.hpp"
#include "debug.hpp"
#include <fstream>

const static auto src = "Scene";

Scene::Scene() {
    const int number_of_pipes = 5;
    float screen_height_px = 450;

    _player.pos = {5_b, 10_b};

    _pipes.resize(number_of_pipes);
    for (std::size_t i = 0; i < number_of_pipes; i++) {
        float x = screen_height_px + i * 250;
        float gapY = 100 + rand() % 200;
        _pipes[i].pos.x = x;
        _pipes[i].pos.y = gapY;
        _pipes[i].passed = false;
    }

    _platforms.push_back(
        {{5_b, 15_b}, {5_b, 1_b}}
    );

    _buttons = {
        Button{{20_b, 5_b}, {5_b, 2_b}, "Button 1", Action::TOGGLE_GROUP_1},
        Button{{20_b, 8_b}, {5_b, 2_b}, "Button 2", Action::TOGGLE_GROUP_2}
    };
};

