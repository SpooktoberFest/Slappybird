
#include <ctime>
#include "game.hpp"
#include "debug.hpp"

const static auto src = "Main";

int main(int argc, char** argv) {
    {
        srand(time(0));
        if (argc == 1) {
            Game game;

            while (!game.is_quit()) {
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
