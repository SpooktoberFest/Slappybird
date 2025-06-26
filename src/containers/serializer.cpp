#include "serializer.hpp"

#include <fstream>
#include <sstream>

// #include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

#include "debug.hpp"


const static auto src = "Serializer";

using json = nlohmann::json;

const std::string Serializer::cannon_scenes_path = "resources/scenes/";
const std::string Serializer::custom_scenes_path = "resources/custom_scenes/";
const std::string Serializer::profiles_path = "resources/profiles/";


bool Serializer::saveScene(std::string name, bool custom, Scene* scene_opt) {
    try {
        std::ofstream os(
            (custom ? custom_scenes_path : cannon_scenes_path) + name,
            std::ios::binary
        );
        if (!os) return false;
        cereal::BinaryOutputArchive archive(os);
        archive(scene_opt ? *scene_opt : loaded_scene);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to serialize scene: " + std::string(e.what()));
        return false;
    }
}
bool Serializer::loadScene(std::string name, bool custom, Scene* scene_opt) {
    try {
        std::ifstream is(
            custom ? custom_scenes_path : cannon_scenes_path,
            std::ios::binary
        );
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

    try {
        int failures = 0;
        int total = 0;

        // Cannon scenes
        if (data.contains("cannon_scenes")) {
            for (const auto& scene_json : data["cannon_scenes"]) {
                ++total;
                std::string name = scene_json["name"];
                auto scene_opt = from_json(scene_json["data"]);
                if (scene_opt) saveScene(name, false, &(*scene_opt));
                else ++failures;
            }
        }

        // Custom scenes
        if (data.contains("custom_scenes")) {
            for (const auto& scene_json : data["custom_scenes"]) {
                ++total;
                std::string name = scene_json["name"];
                auto scene_opt = from_json(scene_json["data"]);
                if (scene_opt) saveScene(name, true, &(*scene_opt));
                else ++failures;
            }
        }

        // Profiles
        if (data.contains("profiles")) {
            for (const auto& profile_json : data["profiles"]) {
                ++total;
                try {
                    std::string name = profile_json["name"];
                    const auto& profile_data = profile_json["data"];
                    Profile profile;
                    profile._sets = unlock_map<Equipment>(profile_data.value("_sets", std::vector<u_int16_t>()));
                    profile._movesets = unlock_map<Technique>(profile_data.value("_movesets", std::vector<u_int16_t>()));
                    saveProfile(name, &profile);
                } catch (const std::exception& e) {
                    LOG_ERROR(src, "Failed to parse profile: " + std::string(e.what()));
                    ++failures;
                }
            }
        }

        LOG_INFO(src, "Finished translating: " + std::to_string(failures) + " failures out of " + std::to_string(total));
        return failures == 0;

    } catch (const std::exception& e) {
        LOG_ERROR(src, "Unexpected exception during processing: " + std::string(e.what()));
        return false;
    }
}

std::optional<Scene> Serializer::from_json(const json& json_data) {
    Scene scene;
    std::vector<float> tmp;
    std::string breakpoint;

    const auto get_f = [&json_data](float& target, std::string name)
        { if (json_data.count(name)) { target = json_data[name].get<float>(); } };
    const auto get_vec2 = [&tmp](const json& json_data, Vec2& target, std::string name)
        { if (json_data.count(name)) { tmp = json_data[name].get<std::vector<float>>(); target = {tmp[0], tmp[1]};} };
    const auto get_rect = [&tmp](const json& json_data, Rect& target, std::string name)
        { if (json_data.count(name)) { tmp = json_data[name].get<std::vector<float>>(); target = {tmp[0], tmp[1], tmp[2], tmp[3]};} };


    try {
        breakpoint = "_player";
        if (json_data.count("_player")) {
            auto& in = json_data["_player"];
            get_vec2(in, scene._player.pos, "pos");
            get_vec2(in, scene._player.vel, "vel");
        };
        breakpoint = "_pipes";
        if (json_data.count("_pipes")) {
            Pipe pipe;
            for (const auto& json_elem : json_data["_pipes"]) {
                get_vec2(json_elem, pipe.pos, "pos");
                scene._pipes.push_back(std::move(pipe));
            }
        };
        breakpoint = "_buttons";
        if (json_data.count("_buttons")) {
            Button button;
            for (const auto& json_elem : json_data["_buttons"]) {
                get_rect(json_elem, button.rect, "rect");
                button.action = json_elem["action"].get<Action>();
                button.text = json_elem["text"].get<std::string>();
                scene._buttons.push_back(std::move(button));
            }
        };
        breakpoint = "_platforms";
        if (json_data.count("_platforms")) {
            Platform platform;
            for (const auto& json_elem : json_data["_pipes"]) {
                get_rect(json_elem, platform.rect, "rect");
                scene._platforms.push_back(std::move(platform));
            }
        };

        breakpoint = "floats";
        get_f(scene._pipe_width, "_pipe_width");
        get_f(scene._gap_height, "_gap_height");
        get_f(scene._gravity, "_gravity");
        get_f(scene._jump_strength, "_jump_strength");
        get_f(scene._pipe_speed, "_pipe_speed");
        get_f(scene._move_speed, "_move_speed");

        breakpoint = "_score";
        scene._score = json_data.value("_score", 0);
        breakpoint = "camera";
        get_vec2(json_data, scene._camera_velocity, "_camera_velocity");
        get_vec2(json_data, scene._camera_position, "_camera_position");

        return scene;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Unexpected exception during processing of scene." + breakpoint + ": " + std::string(e.what()));
        return std::nullopt;
    }
}


