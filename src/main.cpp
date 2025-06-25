
#include <ctime>
#include "game.hpp"

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
            if (argv[1] == "template")
                serializer.devModeTemplate();
            else 
                serializer.devModeLoad(argv[1]);
            
        }



    }
    return 0;
}
