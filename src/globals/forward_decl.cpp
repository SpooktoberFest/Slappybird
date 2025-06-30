#include "forward_decl.hpp"

#include "raylib.h"


Shader_::Shader_(const Shader& s) : id(s.id), locs(s.locs) {};
Shader_::operator Shader() const{ return {id, locs}; };