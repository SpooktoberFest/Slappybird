#ifndef SUPERFLAPPY_SERIALIZER_HPP
#define SUPERFLAPPY_SERIALIZER_HPP

#include "scene.hpp"
#include "properties.hpp"
// #include "entities.hpp"


class Serializer {
public:
    Serializer() {};
    virtual ~Serializer() {};

    bool saveScene(int index);
    bool loadScene(int index);

    bool saveCostumScene(std::string name);
    bool loadCostumScene(std::string name);

    bool saveProfile(std::string name);
    bool loadProfile(std::string name);

    bool devModeTemplate();
    bool devModeLoad(std::string path);

private:

    bool devSaveScene(std::string path, std::string scene_str);
    bool devSaveProfile(std::string path, std::string profile_str);


    std::string devScene(Scene scene);
    std::string devProfile(Profile profile);

    Scene loaded_scene;
    Profile loaded_profile;
    const static std::string cannon_scenes_path;
    const static std::string custom_scenes_path;
    const static std::string profiles_path;
};


#endif // SUPERFLAPPY_SERIALIZER_HPP