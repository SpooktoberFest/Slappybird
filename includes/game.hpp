#ifndef SUPERFLAPPY_GAME_HPP
#define SUPERFLAPPY_GAME_HPP

#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "debug.hpp"

#define TITLE "Super Flappy Kendoka Person!"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

const static auto src = "Game";

struct Bird {
    float y;
    float velocity;
    Rectangle rect;
};

struct Pipe {
    float x;
    float gapY;
    Rectangle topRect;
    Rectangle bottomRect;
    bool passed;
};

struct World {
    Bird bird;
    std::vector<Pipe> pipes;
    int score = 0;
    bool gameOver = false;
};

#define INITIALIZE                                  \
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE); \
    /* InitAudioDevice(); */                        \
    SetTargetFPS(60);                               \
    srand(time(0));                                 \
    LOG_INFO(src, "Initialized Raylib Components");

#define SHUTDOWN                \
    /* CloseAudioDevice(); */   \
    CloseWindow();              \
    LOG_INFO(src, "Closed Raylib Components");

void render(World& world);


#endif // SUPERFLAPPY_GAME_HPP