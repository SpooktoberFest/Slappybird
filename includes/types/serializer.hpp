#pragma once

#include "entities.hpp"
#include "properties.hpp"

class Serializer {
public:
    Serializer() {};
    ~Serializer() {};

    bool saveScene(std::string name, Scene* scene_opt=nullptr);
    bool loadScene(std::string name, Scene* scene_opt=nullptr);

    bool saveProfile(std::string name, Profile* profile_opt=nullptr);
    bool loadProfile(std::string name, Profile* profile_opt=nullptr);

    bool saveMenu(std::string name, Menu* menu_opt=nullptr);
    bool loadMenu(std::string name, Menu* menu_opt=nullptr);

    bool devModeLoad(std::string path);


    Scene loaded_scene;
    Profile loaded_profile;
    Menu loaded_menu;

    const static std::string scenes_path;
    const static std::string profiles_path;

    std::vector<std::string> get_files(const std::string& path, const std::string type);


private:


    // std::optional<Scene> fetch_scene(const std::string& name, bool custom);
    // void send_scene(const std::string& name, bool custom);
    // std::optional<Profile> fetch_profile(const std::string& name);
    // void send_profile(const std::string& name);
    // bool saveSceneToDB(const std::string& name, const Scene& scene, bool custom);



};
