
#ifndef SUPERFLAPPY_FWD_DECL_HPP
#define SUPERFLAPPY_FWD_DECL_HPP

#include <cstring>

// Forward declarations

// nlohmann::json
struct JsonRef;

// SuperFlappy structs
struct Spawner;
struct Menu;
struct Game;
struct World;

// Raylib structs
struct Shader;
struct Vector2;
struct Rectangle;
struct Shader;
struct Shader_ {
    Shader_() {};
    Shader_(const Shader& s);
    operator Shader() const;
private:
    unsigned int id;
    int *locs;
};


// Raylib enum
enum KeyboardKey_ {};


#endif // SUPERFLAPPY_FWD_DECL_HPP