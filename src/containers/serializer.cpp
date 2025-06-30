#include "serializer.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <algorithm>

#include <cereal/archives/binary.hpp>
// #include <sqlite3.h>

#include "debug.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

const static auto src = "Serializer";

const std::string Serializer::scenes_path = "resources/scenes/";
const std::string Serializer::profiles_path = "resources/profiles/";


bool Serializer::saveScene(std::string name, Scene* scene_opt) {
    try {
        std::ofstream os(scenes_path + name, std::ios::binary);
        if (!os) return false;
        cereal::BinaryOutputArchive archive(os);
        archive(scene_opt ? *scene_opt : loaded_scene);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to serialize scene: " + std::string(e.what()));
        return false;
    }
}
bool Serializer::loadScene(std::string name, Scene* scene_opt) {
    try {
        std::ifstream is(scenes_path + name, std::ios::binary);
        if (!is) return false;
        cereal::BinaryInputArchive archive(is);
        archive(scene_opt ? *scene_opt : loaded_scene);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to deserialize scene: " + std::string(e.what()));
        return false;
    }
}

bool Serializer::saveProfile(std::string name, Profile* profile_opt) {
    try {
        std::ofstream os(profiles_path + name, std::ios::binary);
        if (!os) return false;
        cereal::BinaryOutputArchive archive(os);
        archive(profile_opt ? *profile_opt : loaded_profile);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to serialize profile: " + std::string(e.what()));
        return false;
    }
}
bool Serializer::loadProfile(std::string name, Profile* profile_opt) {
    try {
        std::ifstream is(profiles_path + name, std::ios::binary);
        if (!is) return false;
        cereal::BinaryInputArchive archive(is);
        archive(profile_opt ? *profile_opt : loaded_profile);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to deserialize profile: " + std::string(e.what()));
        return false;
    }
}

bool Serializer::loadMenu(std::string name, Menu* menu_opt) {
    try {
        std::ifstream is(scenes_path + name, std::ios::binary);
        if (!is) return false;
        cereal::BinaryInputArchive archive(is);
        archive(menu_opt ? *menu_opt : loaded_menu);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to deserialize menu: " + std::string(e.what()));
        return false;
    }
}
bool Serializer::saveMenu(std::string name, Menu* menu_opt) {
    try {
        std::ofstream os(scenes_path + name, std::ios::binary);
        if (!os) return false;
        cereal::BinaryOutputArchive archive(os);
        archive(menu_opt ? *menu_opt : loaded_menu);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to serialize menu: " + std::string(e.what()));
        return false;
    }
}

bool Serializer::devModeLoad(std::string path) {
    LOG_INFO(src, "Translating JSON data...");
    json data;

    try {
        std::ifstream is(path);
        if (!is) {
            LOG_ERROR(src, "Failed to load file at path: " + path);
            return false;
        }
        data = json::parse(is);
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to parse JSON file: " + std::string(e.what()));
        return false;
    }

    int failures = 0;
    int total = 0;

    // Scenes
    if (data.contains("scenes")) {
        for (const auto& j : data["scenes"]) {
            ++total;
            try {
                Scene scene = Scene().load(j["data"]);
                saveScene(j["name"], &scene);
            } catch (const std::exception& e) {
                LOG_ERROR(src, "Failed to parse scene: " + std::string(e.what()));
                ++failures;
            }
        }
    }

    // Menus
    if (data.contains("menus")) {
        for (const auto& j : data["menus"]) {
            ++total;
            try {
                Menu menu = Menu().load(j["data"]);
                saveMenu(j["name"], &menu);
            } catch (const std::exception& e) {
                LOG_ERROR(src, "Failed to parse menu: " + std::string(e.what()));
                ++failures;
            }
        }
    }

    // Profiles
    if (data.contains("profiles")) {
        for (const auto& j : data["profiles"]) {
            ++total;
            try {
                Profile profile = Profile().load(j["data"]);
                saveProfile(j["name"], &profile);
            } catch (const std::exception& e) {
                LOG_ERROR(src, "Failed to parse profile: " + std::string(e.what()));
                ++failures;
            }
        }
    }

    LOG_INFO(src, "Finished translating: " + std::to_string(failures) + " failures out of " + std::to_string(total));
    return failures == 0;

}


std::vector<std::string> Serializer::get_files(const int start_index, const std::string& path, std::string type) {
    std::vector<std::string> matched_files;

    try {
        if (!fs::exists(path) || !fs::is_directory(path)) {
            LOG_ERROR(src, "Invalid path: " + path)
            return matched_files;
        }

        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file() && entry.path().extension() == type) {
                matched_files.push_back(entry.path().filename().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        LOG_ERROR(src, "Filesystem error: " + std::string(e.what()));
    }

    return matched_files;
}


