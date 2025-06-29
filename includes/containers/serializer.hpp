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

    bool saveScene(std::string name, Scene* scene_opt=nullptr);
    bool loadScene(std::string name, Scene* scene_opt=nullptr);

    bool saveProfile(std::string name, Profile* profile_opt=nullptr);
    bool loadProfile(std::string name, Profile* profile_opt=nullptr);

    bool saveMenu(std::string name, Menu* menu_opt=nullptr);
    bool loadMenu(std::string name, Menu* menu_opt=nullptr);

    bool devModeLoad(std::string path);
    std::vector<std::string> get_files(const int start_index, const std::string& path, std::string type);


    Scene loaded_scene;
    Profile loaded_profile;
    Menu loaded_menu;

    const static std::string scenes_path;
    const static std::string profiles_path;

private:

    std::optional<Scene> from_json(const nlohmann::json& j);

    // std::optional<Scene> fetch_scene(const std::string& name, bool custom);
    // void send_scene(const std::string& name, bool custom);
    // std::optional<Profile> fetch_profile(const std::string& name);
    // void send_profile(const std::string& name);
    // bool saveSceneToDB(const std::string& name, const Scene& scene, bool custom);

    inline void get_maybe_vec2(const nlohmann::json& json_data, Vec2& write_to, const std::string field_name) {
        if (json_data.count(field_name)) {
            std::vector<float> tmp = json_data[field_name].get<std::vector<float>>();
            write_to = {tmp[0]*BLOCK, tmp[1]*BLOCK};
        }
    };


};


#endif // SUPERFLAPPY_SERIALIZER_HPP