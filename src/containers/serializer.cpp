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
            std::string name = j["name"];
            auto scene_opt = from_json(j["data"]);
            if (scene_opt) saveScene(name, &(*scene_opt));
            else ++failures;
        }
    }

    // Menus
    if (data.contains("menus")) {
        for (const auto& j : data["menus"]) {
            ++total;
            Menu menu;
            try {
                // Menu Buttons
                for (const auto& b : j["data"]["buttons"]) {
                    Button button;
                    get_maybe_vec2(b, button.pos, "pos");
                    get_maybe_vec2(b, button.size, "size");
                    button.action = b.value("action", Action::NOP);
                    button.parameter = b.value("parameter", 0);
                    button.text = b.value("text", "");
                    menu._buttons.push_back(std::move(button));
                }
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
                Profile profile{
                    unlock_map<Equipment>(j["data"].value("equipment", std::vector<u_int16_t>())),
                    unlock_map<Technique>(j["data"].value("moves", std::vector<u_int16_t>()))
                };
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

std::optional<Scene> Serializer::from_json(const json& json_data) {
    Scene scene;
    std::string breakpoint;

    try {
        breakpoint = "player";
        if (json_data.count("player")) {
            auto& in = json_data["player"];
            get_maybe_vec2(in, scene._world.player->pos, "pos");
            get_maybe_vec2(in, scene._world.player->vel, "vel");
        };
        breakpoint = "pipes";
        if (json_data.count("pipes")) {
            Pipe pipe;
            for (const auto& j : json_data["pipes"]) {
                get_maybe_vec2(j, pipe.pos, "pos");
                scene._world.pipes.push_back(std::move(pipe));
            }
        };
        breakpoint = "buttons";
        if (json_data.count("buttons")) {
            Button button;
            for (const auto& j : json_data["buttons"]) {
                get_maybe_vec2(j, button.pos, "pos");
                get_maybe_vec2(j, button.size, "size");
                button.action = j.value("action", Action::NOP);
                button.parameter = j.value("parameter", 0);
                button.text = j.value("text", "");
                scene._world.buttons.push_back(std::move(button));
            }
        };
        breakpoint = "platforms";
        if (json_data.count("platforms")) {
            Platform platform;
            for (const auto& j : json_data["platforms"]) {
                get_maybe_vec2(j, platform.pos, "pos");
                get_maybe_vec2(j, platform.size, "size");
                scene._world.platforms.push_back(std::move(platform));
            }
        };
        breakpoint = "biomes";
        if (json_data.count("biomes")) {
            Biome biome;
            for (const auto& j : json_data["biomes"]) {
                biome.pipe_width = j.value("pipe_width", biome.pipe_width);
                biome.gap_height = j.value("gap_height", biome.gap_height);
                biome.gravity = j.value("gravity", biome.gravity);
                biome.jump_strength = j.value("jump_strength", biome.jump_strength);
                biome.pipe_speed = j.value("pipe_speed", biome.pipe_speed);
                biome.move_speed = j.value("move_speed", biome.move_speed);
                scene._world.biomes.push_back(std::move(biome));
            }
        };

        breakpoint = "score";
        scene._score = json_data.value("score", 0);
        breakpoint = "camera";
        get_maybe_vec2(json_data, scene._cam_pos, "cam_pos");
        scene._cam_vel.x = json_data.value("cam_vel_x", QNAN);
        scene._cam_vel.y = json_data.value("cam_vel_y", QNAN);

        return scene;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Unexpected exception during processing of scene." + breakpoint + ": " + std::string(e.what()));
        return std::nullopt;
    }
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


