#include "serializer.hpp"

#include <cereal/archives/binary.hpp>
#include <fstream>

#include "debug.hpp"

const static auto src = "Serializer";

Serializer::Serializer() {};


bool Serializer::saveScene(int index) {
    try {
        std::ofstream os(cannon_scenes_path, std::ios::binary);
        if (!os) return false;
        cereal::BinaryOutputArchive archive(os);
        archive(loaded_scene);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to serialize scene: " + std::string(e.what()));
        return false;
    }
}
bool Serializer::loadScene(int index) {
    try {
        std::ifstream is(cannon_scenes_path, std::ios::binary);
        if (!is) return false;
        cereal::BinaryInputArchive archive(is);
        archive(loaded_scene);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to deserialize scene: " + std::string(e.what()));
        return false;
    }
}

bool Serializer::saveCostumScene(std::string name) {
    try {
        std::ofstream os(custom_scenes_path + name, std::ios::binary);
        if (!os) return false;
        cereal::BinaryOutputArchive archive(os);
        archive(loaded_scene);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to serialize custom scene: " + std::string(e.what()));
        return false;
    }
}
bool Serializer::loadCostumScene(std::string name) {
    try {
        std::ifstream is(custom_scenes_path + name, std::ios::binary);
        if (!is) return false;
        cereal::BinaryInputArchive archive(is);
        archive(loaded_scene);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to deserialize custom scene: " + std::string(e.what()));
        return false;
    }
}

bool Serializer::saveProfile(std::string name) {
    try {
        std::ofstream os(profiles_path + name, std::ios::binary);
        if (!os) return false;
        cereal::BinaryOutputArchive archive(os);
        archive(loaded_profile);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to serialize profile: " + std::string(e.what()));
        return false;
    }
}
bool Serializer::loadProfile(std::string name) {
    try {
        std::ifstream is(profiles_path + name, std::ios::binary);
        if (!is) return false;
        cereal::BinaryInputArchive archive(is);
        archive(loaded_profile);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR(src, "Failed to deserialize profile: " + std::string(e.what()));
        return false;
    }
}

const std::string Serializer::cannon_scenes_path = "resources/scenes/";
const std::string Serializer::custom_scenes_path = "resources/custom_scenes/";
const std::string Serializer::profiles_path = "resources/profiles/";

