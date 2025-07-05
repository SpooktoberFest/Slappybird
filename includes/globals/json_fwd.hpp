#pragma once

#include "nlohmann/json.hpp"

// This header should only be included in source files.

struct JsonRef {
    JsonRef(const nlohmann::json& j_) : j(j_) {};
    operator const nlohmann::json&() const { return j; }
private:
    const nlohmann::json& j;
};
