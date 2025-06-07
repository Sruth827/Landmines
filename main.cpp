#include <raylib.h>
#include <raymath.h>
#include "game.h"
#include "grid.h"
#include "playerPosition.h"
#include <iostream>
#include <filesystem>
#include <fstream>


int main() {
	printf("Raylib Version: %s\n", RAYLIB_VERSION);
	int numRows = 50;
	int numCols = 30;
	int cellSize = 20;
	int win_height = 1080;
	int win_width = 1080;
	InitWindow(win_width, win_height, "Landmines");
	SetTargetFPS(60);

	Font font = LoadFontEx("Font/armalite.ttf", 64, 0, 0);

	Shader fogShader = LoadShader(0, "shaders/fog.fs");
	if (fogShader.id == 0) std::cerr << "Shader failed to load!" << std::endl;
	
	// Set initial fog values
	Grid battleField = Grid(numRows, numCols, cellSize);
	Game game(battleField);
	game.InitializeGraphics();
	
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		BeginShaderMode(fogShader);
		
		//game.GetPlayer().DrawSpotLight(fogShader);
		BeginMode2D(game.camera);
		game.Update(fogShader);
		game.Draw();
		EndMode2D();


		EndShaderMode();
		
		DrawRectangle(0, 850, win_width, win_height - 850, BROWN);
		DrawTextEx(font, "BEHIND ENEMY MINES", { 100, 900 }, 90, 2, BLACK);
		//DrawTextEx(font, "ENEMY", { 700, 120 }, 90, 2, WHITE);
		//DrawTextEx(font, "MINES", { 700, 210 }, 90, 2, WHITE);

		if (game.gameOver) {
			DrawTextEx(font, "GAMEOVER!", {300, 400}, 100, 2, WHITE);
			DrawTextEx(font, "Press R to Respawn", {100, 500}, 100, 2, WHITE);
		}
		
		
		EndDrawing(); 
	}
	CloseAudioDevice();
	CloseWindow(); 
	
	return 0;
}