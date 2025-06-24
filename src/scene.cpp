#include "game.hpp"
#include "debug.hpp"

const static auto src = "Scene";

Scene::Scene() {
    const int number_of_pipes = 5;
    float screen_height_px = 450;

    player.hitbox;
    player.velocity;
    player.move_speed = 5.0f;

    _pipes.resize(number_of_pipes);
    for (std::size_t i = 0; i < number_of_pipes; i++) {
        float x = screen_height_px + i * 250;
        float gapY = 100 + rand() % 200;
        _pipes[i].x = x;
        _pipes[i].gapY = gapY;
        _pipes[i].passed = false;
    }

    _platforms.push_back({
        {50, 400, 150, 40}
    });

    _buttons.push_back({
        {600, 100, 90, 40}, "Button 1"
    });

};
