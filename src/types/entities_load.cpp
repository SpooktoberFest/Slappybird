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

Chararacter& Chararacter::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    pos.load(j, "pos");
    vel.load(j, "vel");
    return *this;
}

Pipe& Pipe::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    pos.load(j, "pos");
    return *this;
}
Platform& Platform::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    pos.load(j, "pos");
    size.load(j, "size", 1_b);
    return *this;
}
Biome& Biome::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    pipe_width = j.value("pipe_width", pipe_width);
    gap_height = j.value("gap_height", gap_height);
    gravity = j.value("gravity", gravity);
    jump_strength = j.value("jump_strength", jump_strength);
    pipe_speed = j.value("pipe_speed", pipe_speed);
    move_speed = j.value("move_speed", move_speed);
    return *this;
}

Button& Button::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    action.type = j.value("action_type", ActionType::NOP);
    action.index = j.value("action_index", 0);
    text = j.value("text", "");
    return *this;
}
ButtonList& ButtonList::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    foreach_if_exists("buttons")       buttons.push_back(Button().load(elem));
    horizontal = j.value("horizontal", false);
    special_content = j.value("special_content", Type::NONE);
    pos = j.value("pos", 0.0f) * BLOCK;
    spacing = j.value("spacing", 0.0f) * BLOCK;
    begin = j.value("begin", 0.0f) * BLOCK;
    end = j.value("end", 15.0f) * BLOCK;
    button_dims.load(j, "button_dims", 1_b);
    return *this;
}
Menu& Menu::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    foreach_if_exists("buttons")       buttons.push_back(ButtonList().load(elem));
    return *this;
}
World& World::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    if (j.count("player"))  player = Chararacter().load(j["player"]);
    if (j.count("menu"))  menu = Menu().load(j["menu"]);
    foreach_if_exists("enemies")    enemies.push_back(Chararacter().load(elem));
    foreach_if_exists("spawners")   spawners.push_back(Spawner().load(elem));
    foreach_if_exists("pipe")       pipes.push_back(Pipe().load(elem));
    foreach_if_exists("platforms")  platforms.push_back(Platform().load(elem));
    foreach_if_exists("biomes")     biomes.push_back(Biome().load(elem));
    return *this;
}

Spawner& Spawner::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    pos.load(j, "pos");
    vel.load(j, "vel");
    predicate.type = j.value("action_type", ActionType::NOP);
    predicate.index = j.value("action_index", 0);
    use_index = j.value("use_index", false);
    if (j.count("spawn_in")) spawn_in.load(j["spawn_in"]);
    return *this;
}
Scene& Scene::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    _world.load(j);
    _cam_vel.load(j, "cam_pos");
    _cam_vel.load(j, "cam_vel", QNAN);
    _score = j.value("score", 0);
    return *this;
}
