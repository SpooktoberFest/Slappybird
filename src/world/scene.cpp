#include "scene.hpp"
#include "debug.hpp"

#include "json_fwd.hpp"

#define foreach_if_exists(str) if (j.count(str)) for (const auto& elem : j[str])
const static auto src = "Scene";

Scene::Scene() {
    _world.player = Chararacter();
    _world.biomes = {Biome()};
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

    // _buttons = {
    //     Button{{20_b, 5_b}, {5_b, 2_b}, "Button 1", ActionType::TOGGLE_GROUP_1},
    //     Button{{20_b, 8_b}, {5_b, 2_b}, "Button 2", ActionType::TOGGLE_GROUP_2}
    // };
};

// Load functions

Scene& Scene::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    if (j.count("player"))           _world.player = Chararacter().load(j["player"]);
    foreach_if_exists("enemies")    _world.enemies.push_back(Chararacter().load(elem));
    foreach_if_exists("spawners")   _world.spawners.push_back(Spawner().load(elem));
    foreach_if_exists("pipe")       _world.pipes.push_back(Pipe().load(elem));
    foreach_if_exists("buttons")    _world.buttons.push_back(Button().load(elem));
    foreach_if_exists("platforms")  _world.platforms.push_back(Platform().load(elem));
    foreach_if_exists("biomes")     _world.biomes.push_back(Biome().load(elem));
    _cam_vel.load(j, "cam_pos");
    _cam_vel.load(j, "cam_vel", QNAN);
    _score = j.value("score", 0);
    return *this;
}
Menu& Menu::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    foreach_if_exists("buttons")       _buttons.push_back(Button().load(elem));
    return *this;
}





