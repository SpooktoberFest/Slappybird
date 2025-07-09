#include "properties.hpp"

#include "utils.hpp"
#include "json_fwd.hpp"


Vec2::Vec2(const JsonRef jr, const std::string field, float def) {
    const nlohmann::json& j = jr;
    if (!j.count(field)) { x = def; y = def; return; }
    const auto& arr = j[field];
    x = arr[0].is_null() ? def : arr[0].get<float>();
    y = arr[1].is_null() ? def : arr[1].get<float>();
}
Loadout::Loadout(const JsonRef jr) {
    const nlohmann::json& j = jr;
    head = j.value("head", head);
    torso = j.value("torso", torso);
    hands = j.value("hands", hands);
    legs = j.value("legs", legs);
    weapon = j.value("weapon", weapon);
    const std::vector<float> tmp = j.value("moveset", std::vector<float>());
    for (std::size_t i{0} ; i < std::min(tmp.size(), std::size_t(4)) ; ++i)
        moveset[i] = static_cast<Technique>(tmp[i]);
}
ControlScheme::ControlScheme(const JsonRef jr) {
    const nlohmann::json& j = jr;
    move = j.value("move", move);
    nav = j.value("nav", nav);
    jump = j.value("jump", jump);
    select = j.value("select", select);
    pause = j.value("pause", pause);
    reset = j.value("reset", reset);
}
Profile::Profile(const JsonRef jr) {
    const nlohmann::json& j = jr;
    equipment = unlock_map<Equipment>(j.value("equipment", std::vector<uint8_t>()));
    moves = unlock_map<Technique>(j.value("moves", std::vector<uint8_t>()));
}
Action::Action(const JsonRef jr) {
    const nlohmann::json& j = jr;
    type = j.value("action_type", type);
    index = j.value("action_index", index);
}
Trigger::Trigger(const JsonRef jr) {
    const nlohmann::json& j = jr;
    type = j.value("trigger_type", type);
    parameter = j.value("trigger_parameter", parameter);
}
