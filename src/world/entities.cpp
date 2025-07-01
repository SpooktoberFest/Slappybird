#include "entities.hpp"

#include "algorithm"
#include "json_fwd.hpp"

#include "game.hpp"

#include "raylib.h"

#define foreach_if_exists(str) if (j.count(str)) for (const auto& elem : j[str])


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

Rectangle Chararacter::rect(float w, float h) const {
    return Rectangle{pos.x, pos.y, std::move(w), std::move(h)};
};
std::array<Rectangle, 2> Pipe::rect(const float w, const float h) const {
    return {Rectangle{pos.x, 0, w, pos.y}, Rectangle{pos.x, pos.y+h, w, 12_b}};
};
Rectangle Platform::rect() const {
    return Rectangle{pos.x, pos.y, size.x, size.y};
};
Rectangle Button::rect(const Vector2& res) const {
    return Rectangle{
        (pos.x < 0.0f ? res.x + pos.x : pos.x),
        (pos.y < 0.0f ? res.y + pos.y : pos.y),
        size.x, size.y};
};

// Load() functions

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
Button& Button::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    pos.load(j, "pos");
    size.load(j, "size", 1_b);
    action.type = j.value("action_type", ActionType::NOP);
    action.index = j.value("action_index", 0);
    parameter = j.value("parameter", 0);
    text = j.value("text", "");
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
World& World::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    if (j.count("player"))  player = Chararacter().load(j["player"]);
    foreach_if_exists("enemies")    enemies.push_back(Chararacter().load(elem));
    foreach_if_exists("spawners")   spawners.push_back(Spawner().load(elem));
    foreach_if_exists("pipe")       pipes.push_back(Pipe().load(elem));
    foreach_if_exists("buttons")    buttons.push_back(Button().load(elem));
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