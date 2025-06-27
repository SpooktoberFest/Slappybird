
#include <ctime>
#include "game.hpp"
#include "debug.hpp"

const static auto src = "Main";

int main(int argc, char** argv) {
    {
        srand(time(0));
        if (argc == 1) {    // Run game
            Scene scene_1;
            Game game;
            game.load_scene(&scene_1);


            while (!WindowShouldClose()) {
                game.simulate();
                game.render();
            }
        } else if (argc == 2) {
            Serializer serializer;
            serializer.devModeLoad(argv[1]);
            
        }



    }
    return 0;
}
