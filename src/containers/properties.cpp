#include "properties.hpp"

Vec2& Vec2::load(const nlohmann::json& j, const std::string field, const float def) {
    if (!j.count(field)) { x = def; y = def; return *this; }
    const auto& arr = j[field];
    x = arr[0].is_null() ? def : arr[0].get<float>() * BLOCK;
    y = arr[1].is_null() ? def : arr[1].get<float>() * BLOCK;
    return *this;
}
Loadout& Loadout::load(const nlohmann::json& j) {
    head = j.value("head", head);
    torso = j.value("torso", torso);
    hands = j.value("hands", hands);
    legs = j.value("legs", legs);
    weapon = j.value("weapon", weapon);
    const std::vector<float> tmp = j.value("moveset", std::vector<float>());
    for (std::size_t i{0} ; i < std::min(tmp.size(), std::size_t(4)) ; ++i)
        moveset[i] = static_cast<Technique>(tmp[i]);
    return *this;
}
ControlScheme& ControlScheme::load(const nlohmann::json& j) {
    // std::vector<KeyboardKey> tmp;
    move = j.value("move", move);
    nav = j.value("nav", nav);
    jump = j.value("jump", jump);
    select = j.value("select", select);
    pause = j.value("pause", pause);
    reset = j.value("reset", reset);
    return *this;
}
Profile& Profile::load(const nlohmann::json& j) {
    equipment = unlock_map<Equipment>(j.value("equipment", std::vector<u_int16_t>()));
    moves = unlock_map<Technique>(j.value("moves", std::vector<u_int16_t>()));
    return *this;
}
Action& Action::load(const nlohmann::json& j) {

    return *this;
}



