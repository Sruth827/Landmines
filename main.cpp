#include <raylib.h>
#include "game.h"
#include "grid.h"
#include "playerPosition.h"
#include <iostream>


int main() {

	int numRows = 50;
	int numCols = 30;
	int cellSize = 20;
	InitWindow(1080, 1080, "Landmines");
	SetTargetFPS(60);

	Font font = LoadFontEx("Font/armalite.ttf", 64, 0, 0);


	Grid battleField = Grid(numRows, numCols, cellSize);
	Game game(battleField);
	
	while (!WindowShouldClose()) {
		game.HandleInput();
		game.Update();
		game.Draw();
		BeginDrawing();
		ClearBackground(BLACK);
		
		DrawTextEx(font, "SURVIVE", { 700, 30 }, 90, 2, WHITE);

		
		if (game.gameOver) {
			DrawTextEx(font, "GAMEOVER!", {300, 400}, 100, 2, WHITE);
			DrawTextEx(font, "Press R to Respawn", {100, 500}, 100, 2, WHITE);
		}

		EndDrawing(); 
	}
	CloseWindow(); 
	return 0;
}