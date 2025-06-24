
#include <ctime>
#include "game.hpp"

int main(void) {
    {
        srand(time(0));

        Scene scene_1;
        Game game;
        game.load_scene(&scene_1);

        while (!WindowShouldClose()) {
            game.simulate();
            game.render();
        }
    }
    return 0;
}
