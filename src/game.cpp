#include "game.hpp"

void render(World& world) {
    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Slappy Bird (Raylib)", 10, 10, 20, DARKGRAY);
    DrawRectangleRec(world.bird.rect, RED);

    for (int i = 0; i < world.pipes.size(); i++) {
        DrawRectangleRec(world.pipes[i].topRect, GREEN);
        DrawRectangleRec(world.pipes[i].bottomRect, GREEN);
    }

    DrawText(TextFormat("Score: %d", world.score), SCREEN_WIDTH - 150, 10, 20, BLUE);

    if (world.gameOver) {
        DrawText("Game Over! Press R to Restart", SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 - 10, 20, MAROON);
    }

    EndDrawing();
};