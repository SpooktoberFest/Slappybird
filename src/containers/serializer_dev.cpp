#include "serializer.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "debug.hpp"

using json = nlohmann::json;
const static auto src = "Serializer";

bool Serializer::devModeLoad(std::string path) {
    json data;
    try {
        std::ifstream is(path);
        if (!is) {
            LOG_ERROR(src, "Failed to load file at path: " + path);
            return false;
        }
        data = json::parse(is);
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to parse object: " + std::string(e.what()));
        return false;
    }

    try {
        int failures = 0;
        int total = 0;
        int tmp;

        tmp = data.count("cannon_scenes");
        if (tmp) { total += tmp;
            for (json& scene : data["cannon_scenes"]) {
                failures += devSaveScene(cannon_scenes_path + scene["name"].get<std::string>(), scene["data"].dump());
            }
        }
        tmp = data.count("custom_scenes");
        if (tmp) { total += tmp;
            for (json& scene : data["custom_scenes"]) {
                failures += devSaveScene(custom_scenes_path + scene["name"].get<std::string>(), scene["data"].dump());
            }
        }
        tmp = data.count("profiles");
        if (tmp) { total += tmp;
            for (json& scene : data["profiles"]) {
                failures += devSaveProfile(profiles_path + scene["name"].get<std::string>(), scene["data"].dump());
            }
        }

        LOG_INFO(src, "Finished translating: " + std::to_string(failures) + " failures out of " + std::to_string(total));
        return failures == 0;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to parse object: " + std::string(e.what()));
        return false;
    }
}


bool Serializer::devModeTemplate() {
    json data;
    json& cannon = (data["cannon_scenes"] = nlohmann::json::array());
    json& custom = (data["custom_scenes"] = nlohmann::json::array());
    json& profiles = (data["profiles"] = nlohmann::json::array());
    

    nlohmann::json element;
    element["name"] = "pleb.profile";
    element["data"] = devProfile(Profile());
    profiles.push_back(element);


    try {
        std::ofstream os("template.json");
        if (!os) {
            LOG_ERROR(src, "Failed to load file.");
            return false;
        }
        os << data.dump();
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to write object: " + std::string(e.what()));
        return false;
    }
}



