#ifndef SUPERFLAPPY_SERIALIZER_HPP
#define SUPERFLAPPY_SERIALIZER_HPP

#include <optional>
#include <memory>

#include <nlohmann/json.hpp>

#include "scene.hpp"
#include "properties.hpp"
// #include "entities.hpp"


class Serializer {
public:
    Serializer() {};
    virtual ~Serializer() {};

    bool saveScene(std::string name, bool custom, Scene* scene_opt=nullptr);
    bool loadScene(std::string name, bool custom, Scene* scene_opt=nullptr);

    bool saveProfile(std::string name, Profile* profile_opt=nullptr);
    bool loadProfile(std::string name, Profile* profile_opt=nullptr);

    bool devModeLoad(std::string path);

private:

    std::optional<Scene> from_json(const nlohmann::json& j);

    // std::optional<Scene> fetch_scene(const std::string& name, bool custom);
    // void send_scene(const std::string& name, bool custom);
    // std::optional<Profile> fetch_profile(const std::string& name);
    // void send_profile(const std::string& name);

    bool saveSceneToDB(const std::string& name, const Scene& scene, bool custom);


    Scene loaded_scene;
    Profile loaded_profile;
    const static std::string cannon_scenes_path;
    const static std::string custom_scenes_path;
    const static std::string profiles_path;
};


#endif // SUPERFLAPPY_SERIALIZER_HPP