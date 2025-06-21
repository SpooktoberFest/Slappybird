#include "world_1.hpp"

World spawn_world(const unsigned number_of_pipes) {

    World world;

    world.bird = { SCREEN_HEIGHT/2, 0, {100, SCREEN_HEIGHT/2, 34, 24} };

    world.pipes.resize(number_of_pipes);
    for (std::size_t i = 0; i < number_of_pipes; i++) {
        float x = SCREEN_WIDTH + i * 250;
        float gapY = 100 + rand() % 200;
        world.pipes[i].x = x;
        world.pipes[i].gapY = gapY;
        world.pipes[i].passed = false;
    }

    return world;
};


void simulate(World& world) {
    // Update bird
    if (IsKeyPressed(KEY_SPACE)) world.bird.velocity = JUMP_STRENGTH;
    world.bird.velocity += GRAVITY;
    world.bird.y += world.bird.velocity;
    world.bird.rect.y = world.bird.y;

    // Update pipes
    for (int i = 0; i < world.pipes.size(); i++) {
        world.pipes[i].x -= PIPE_SPEED;

        if (world.pipes[i].x < -PIPE_WIDTH) {
            world.pipes[i].x = SCREEN_WIDTH + 250;
            world.pipes[i].gapY = 100 + rand() % 200;
            world.pipes[i].passed = false;
        }

        world.pipes[i].topRect = (Rectangle){world.pipes[i].x, 0, PIPE_WIDTH, world.pipes[i].gapY};
        world.pipes[i].bottomRect = (Rectangle){world.pipes[i].x, world.pipes[i].gapY + GAP_HEIGHT, PIPE_WIDTH, SCREEN_HEIGHT};

        // Collision
        if (CheckCollisionRecs(world.bird.rect, world.pipes[i].topRect) ||
            CheckCollisionRecs(world.bird.rect, world.pipes[i].bottomRect) ||
            world.bird.y < 0 || world.bird.y + world.bird.rect.height > SCREEN_HEIGHT) {
            world.gameOver = true;
        }

        // Scoring
        if (!world.pipes[i].passed && world.pipes[i].x + PIPE_WIDTH < world.bird.rect.x) {
            world.pipes[i].passed = true;
            world.score++;
        }
    }
};


void game_over(World& world) {
    if (IsKeyPressed(KEY_R)) {
        // Reset game
        world.bird.y = SCREEN_HEIGHT / 2;
        world.bird.velocity = 0;
        world.score = 0;
        world.gameOver = false;
        for (int i = 0; i < world.pipes.size(); i++) {
            world.pipes[i].x = SCREEN_WIDTH + i * 250;
            world.pipes[i].gapY = 100 + rand() % 200;
            world.pipes[i].passed = false;
        }
    }
};

