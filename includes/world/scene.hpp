#ifndef SUPERFLAPPY_SCENE_HPP
#define SUPERFLAPPY_SCENE_HPP

#include <stdlib.h>
#include <limits>
#include <vector>
#include <optional>
#include <array>

#include <cereal/types/vector.hpp>
#include <nlohmann/json.hpp>

#include "entities.hpp"



class Scene {
public:
    Scene();
    virtual ~Scene() {};

    World _world;
    u_int8_t _selected;
    Vec2 _cam_vel = {QNAN, QNAN}; // Where QNAN means follow player
    Vec2 _cam_pos;
    u_int16_t _score = 0;
    std::vector<Action> _actions = {{ActionType::LOAD_WORLD, 0}};

    // (De)Serialization
    Scene& load(const nlohmann::json& j);
    template <class Archive>
    void serialize(Archive& ar) {
        ar(_world, _cam_vel, _cam_pos, _score);
    }
};

struct Menu {
    std::vector<Button> _buttons;
    u_int8_t _selected;

    // (De)Serialization
    Menu& load(const nlohmann::json& j);
    template <class Archive>
    void serialize(Archive& ar) {
        ar(_buttons);
    }
};



#endif // SUPERFLAPPY_SCENE_HPP