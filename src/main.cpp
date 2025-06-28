
#include <ctime>
#include "game.hpp"
#include "debug.hpp"

const static auto src = "Main";

int main(int argc, char** argv) {
    {
        srand(time(0));
        if (argc == 1) {    // Run game
            Game game;

            game._serializer.loadScene("WIP.scene");
            game.reset_scene();

            LOG_DEBUG(src, std::to_string(game._serializer.loaded_scene._platforms.size()) + " platforms in scene!");
            auto& pf = game._serializer.loaded_scene._platforms[0];
            LOG_DEBUG(src, std::to_string(pf.pos.x)+","+std::to_string(pf.pos.y)+","+std::to_string(pf.size.y)+","+std::to_string(pf.size.y));



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
