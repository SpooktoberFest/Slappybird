#include "entities.hpp"

#include "algorithm"

#include "utils.hpp"
#include "debug.hpp"
#include "json_fwd.hpp"


#define foreach_if_exists(str) if (j.count(str)) for (const auto& elem : j[str])
#define get_if_exists(str) if (j.count(str)) j[str]
#define branchless_ternary(pred, a, b) (a & -pred) | (b & ~-pred)

const static auto src = "EntitiesLoad";


// Load functions

Chararacter::Chararacter(const JsonRef jr) {
    const nlohmann::json& j = jr;
    pos = Vec2(j, "pos");
    vel = Vec2(j, "vel");
}

Pipe::Pipe(const JsonRef jr) {
    const nlohmann::json& j = jr;
    pos = Vec2(j, "pos");
}
Platform::Platform(const JsonRef jr) {
    const nlohmann::json& j = jr;
    pos = Vec2(j, "pos");
    size = Vec2(j, "size", 1);
}
Biome::Biome(const JsonRef jr) {
    const nlohmann::json& j = jr;
    pipe_width = j.value("pipe_width", pipe_width);
    gap_height = j.value("gap_height", gap_height);
    gravity = j.value("gravity", gravity);
    jump_strength = j.value("jump_strength", jump_strength);
    pipe_speed = j.value("pipe_speed", pipe_speed);
    move_speed = j.value("move_speed", move_speed);
}

Button::Button(const JsonRef jr) {
    const nlohmann::json& j = jr;
    text = j.value("text", "");
    action = Action(j);
}
ButtonList::ButtonList(const JsonRef jr) {
    const nlohmann::json& j = jr;
    foreach_if_exists("buttons")       buttons.push_back(Button(elem));
    horizontal = j.value("horizontal", false);
    special_content = j.value("special_content", Type::NONE);
    pos = j.value("pos", 0.0f);
    spacing = j.value("spacing", 0.0f);
    begin = j.value("begin", 0.0f);
    end = j.value("end", 15.0f);
    button_dims = Vec2(j, "button_dims", 1.0f);
}
Menu::Menu(const JsonRef jr) {
    const nlohmann::json& j = jr;
    fixed = j.value("fixed", true);
    foreach_if_exists("buttons")    buttons.push_back(ButtonList(elem));
}
World::World(const JsonRef jr) {
    const nlohmann::json& j = jr;
    if (j.count("player"))  player = Chararacter(j["player"]);
    if (j.count("menu"))  menu = Menu(j["menu"]);
    foreach_if_exists("enemies")    enemies.push_back(Chararacter(elem));
    foreach_if_exists("spawners")   spawners.push_back(Spawner(elem));
    foreach_if_exists("pipe")       pipes.push_back(Pipe(elem));
    foreach_if_exists("platforms")  platforms.push_back(Platform(elem));
    foreach_if_exists("biomes")     biomes.push_back(Biome(elem));
}

Spawner::Spawner(const JsonRef jr) {
    const nlohmann::json& j = jr;
    pos = Vec2(j, "pos");
    vel = Vec2(j, "vel");
    fixed = j.value("fixed", true);
    if (j.count("spawn_in")) spawn_in = World(j["spawn_in"]);
    predicate = Trigger(j);
}
Scene::Scene(const JsonRef jr) {
    const nlohmann::json& j = jr;
    _world = World(j);
    _cam_vel = Vec2(j, "cam_pos");
    _cam_vel = Vec2(j, "cam_vel", QNAN);
    _score = j.value("score", 0);
}
