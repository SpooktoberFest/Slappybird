#include "serializer.hpp"

#include <fstream>
#include <sstream>

#include <cereal/archives/binary.hpp>
// #include <sqlite3.h>

#include "debug.hpp"


const static auto src = "Serializer";

using json = nlohmann::json;

const std::string Serializer::cannon_scenes_path = "resources/cannon_scenes/";
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
                    profile.equipment = unlock_map<Equipment>(profile_data.value("_sets", std::vector<u_int16_t>()));
                    profile.moves = unlock_map<Technique>(profile_data.value("_movesets", std::vector<u_int16_t>()));
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
        { if (json_data.count(name)) { tmp = json_data[name].get<std::vector<float>>(); target = {tmp[0]*BLOCK, tmp[1]*BLOCK};} };


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
                get_vec2(json_elem, button.pos, "pos");
                get_vec2(json_elem, button.size, "size");
                button.action = json_elem["action"].get<Action>();
                button.text = json_elem["text"].get<std::string>();
                scene._buttons.push_back(std::move(button));
            }
        };
        breakpoint = "_platforms";
        if (json_data.count("_platforms")) {
            Platform platform;
            for (const auto& json_elem : json_data["_pipes"]) {
                get_vec2(json_elem, platform.pos, "pos");
                get_vec2(json_elem, platform.size, "size");
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
        get_vec2(json_data, scene._cam_vel, "_cam_vel");
        get_vec2(json_data, scene._cam_pos, "_cam_pos");

        return scene;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Unexpected exception during processing of scene." + breakpoint + ": " + std::string(e.what()));
        return std::nullopt;
    }
}


// std::optional<Scene> Serializer::fetchScene(const std::string& name, bool custom) {
//     sqlite3* db = nullptr;
//     if (sqlite3_open((custom ? custom_scenes_path : cannon_scenes_path).c_str(), &db) != SQLITE_OK) {
//         LOG_ERROR(src, "Cannot open database: " + std::string(sqlite3_errmsg(db)));
//         return std::nullopt;
//     }

//     const char* query = "SELECT data FROM scenes WHERE name = ?";
//     sqlite3_stmt* stmt = nullptr;
//     if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
//         sqlite3_close(db);
//         LOG_ERROR(src, "Failed to prepare SQL statement");
//         return std::nullopt;
//     }

//     sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

//     std::optional<Scene> result;

//     if (sqlite3_step(stmt) == SQLITE_ROW) {
//         const void* blob = sqlite3_column_blob(stmt, 0);
//         int blob_size = sqlite3_column_bytes(stmt, 0);

//         std::istringstream iss(std::string(reinterpret_cast<const char*>(blob), blob_size));
//         Scene scene;
//         try {
//             cereal::BinaryInputArchive archive(iss);
//             archive(scene);
//             result = scene;
//         } catch (const std::exception& e) {
//             LOG_ERROR(src, "Deserialization failed: " + std::string(e.what()));
//         }
//     } else {
//         LOG_ERROR(src, "Scene not found: " + name);
//     }

//     sqlite3_finalize(stmt);
//     sqlite3_close(db);
//     return result;
// }


// bool Serializer::saveSceneToDB(const std::string& name, const Scene& scene, bool custom) {
//     sqlite3* db = nullptr;
//     const std::string db_path = custom ? custom_scenes_path : cannon_scenes_path;

//     if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
//         LOG_ERROR(src, "Cannot open database: " + std::string(sqlite3_errmsg(db)));
//         return false;
//     }

//     const char* query = R"(
//         INSERT INTO scenes (name, data)
//         VALUES (?, ?)
//         ON CONFLICT(name) DO UPDATE SET data = excluded.data
//     )";

//     sqlite3_stmt* stmt = nullptr;
//     if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
//         LOG_ERROR(src, "Failed to prepare SQL statement: " + std::string(sqlite3_errmsg(db)));
//         sqlite3_close(db);
//         return false;
//     }

//     // Serialize scene to binary blob
//     std::ostringstream oss(std::ios::binary);
//     try {
//         cereal::BinaryOutputArchive archive(oss);
//         archive(scene);
//     } catch (const std::exception& e) {
//         LOG_ERROR(src, "Serialization failed: " + std::string(e.what()));
//         sqlite3_finalize(stmt);
//         sqlite3_close(db);
//         return false;
//     }

//     std::string blob = oss.str();

//     sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
//     sqlite3_bind_blob(stmt, 2, blob.data(), static_cast<int>(blob.size()), SQLITE_TRANSIENT);

//     bool success = true;
//     if (sqlite3_step(stmt) != SQLITE_DONE) {
//         LOG_ERROR(src, "Failed to insert or update scene: " + std::string(sqlite3_errmsg(db)));
//         success = false;
//     }

//     sqlite3_finalize(stmt);
//     sqlite3_close(db);
//     return success;
// }
