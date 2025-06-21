
// #include "menu.hpp"
#include "world_1.hpp"

int main(void) {
    INITIALIZE

    World world = spawn_world(MAX_PIPES);

    while (!WindowShouldClose()) {
        if (!world.gameOver) simulate(world);
        else game_over(world);
        render(world);
    }

    SHUTDOWN
    return 0;
}
