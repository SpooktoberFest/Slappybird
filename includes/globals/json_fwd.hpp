#ifndef SUPERFLAPPY_JSONFWD_HPP
#define SUPERFLAPPY_JSONFWD_HPP

#include "nlohmann/json.hpp"

struct JsonRef {
    JsonRef(const nlohmann::json& j_) : j(j_) {};
    operator const nlohmann::json&() const { return j; }
private:
    const nlohmann::json& j;
};

#endif // SUPERFLAPPY_JSONFWD_HPP