#ifndef SUPERFLAPPY_WORLD_1_HPP
#define SUPERFLAPPY_WORLD_1_HPP

#include "game.hpp"

#define PIPE_WIDTH 60
#define GAP_HEIGHT 150
#define GRAVITY 0.5f
#define JUMP_STRENGTH -8.0f
#define PIPE_SPEED 3
#define MAX_PIPES 5

World spawn_world(const unsigned number_of_pipes);

void simulate(World& world);

void game_over(World& world);


#endif // SUPERFLAPPY_WORLD_1_HPP