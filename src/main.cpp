
// #include "menu.hpp"
#include "game.hpp"

int main(void) {
    {
        Scene scene;

        while (!WindowShouldClose()) {
            scene.simulate();
            scene.render();
        }

    }
    return 0;
}
