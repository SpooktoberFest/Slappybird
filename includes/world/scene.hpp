#ifndef SUPERFLAPPY_SCENE_HPP
#define SUPERFLAPPY_SCENE_HPP

#include <stdlib.h>
#include <limits>
#include <vector>
#include <optional>
#include <array>
#include <cereal/types/vector.hpp>

#include "entities.hpp"



class Scene {
public:
    Scene();
    virtual ~Scene() {};

    World _world;
    unsigned short _selected;
    Vec2 _cam_vel = {QNAN, QNAN}; // Where QNAN means follow player
    Vec2 _cam_pos;
    unsigned short _score = 0;

    // Serialization function for cereal
    template <class Archive>
    void serialize(Archive& ar) {
        ar(_world, _cam_vel, _cam_pos, _score);
    }
};

struct Menu {
    std::vector<Button> _buttons;
    unsigned short _selected;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(_buttons);
    }
};



#endif // SUPERFLAPPY_SCENE_HPP