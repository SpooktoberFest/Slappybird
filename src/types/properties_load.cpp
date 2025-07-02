#include "properties.hpp"

#include "utils.hpp"
#include "json_fwd.hpp"


Vec2& Vec2::load(const JsonRef jf, const std::string field, float def) {
    const nlohmann::json& j = jf;
    if (!j.count(field)) { x = def; y = def; return *this; }
    const auto& arr = j[field];
    x = arr[0].is_null() ? def : arr[0].get<float>() * BLOCK;
    y = arr[1].is_null() ? def : arr[1].get<float>() * BLOCK;
    return *this;
}
Loadout& Loadout::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
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
ControlScheme& ControlScheme::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    move = j.value("move", move);
    nav = j.value("nav", nav);
    jump = j.value("jump", jump);
    select = j.value("select", select);
    pause = j.value("pause", pause);
    reset = j.value("reset", reset);
    return *this;
}
Profile& Profile::load(const JsonRef jf) {
    const nlohmann::json& j = jf;
    equipment = unlock_map<Equipment>(j.value("equipment", std::vector<u_int16_t>()));
    moves = unlock_map<Technique>(j.value("moves", std::vector<u_int16_t>()));
    return *this;
}
Action& Action::load(const JsonRef jf) {
    const nlohmann::json& j = jf;

    return *this;
}



