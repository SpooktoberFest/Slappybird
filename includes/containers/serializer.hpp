#ifndef SUPERFLAPPY_SERIALIZER_HPP
#define SUPERFLAPPY_SERIALIZER_HPP

#include "scene.hpp"
// #include "entities.hpp"
// #include "properties.hpp"


class Serializer {
public:
    Serializer();
    virtual ~Serializer() {};

    bool saveScene(int index);
    bool loadScene(int index);

    bool saveCostumScene(std::string name);
    bool loadCostumScene(std::string name);

    bool saveProfile(std::string name);
    bool loadProfile(std::string name);

private:

    Scene loaded_scene;
    Profile loaded_profile;
    const static std::string cannon_scenes_path;
    const static std::string custom_scenes_path;
    const static std::string profiles_path;

};


#endif // SUPERFLAPPY_SERIALIZER_HPP