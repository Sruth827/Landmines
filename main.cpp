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
	InitWindow(1080, 1080, "Landmines");
	SetTargetFPS(60);

	Font font = LoadFontEx("Font/armalite.ttf", 64, 0, 0);

	Shader fogShader = LoadShader(0, "shaders/fog.fs");
	if (fogShader.id == 0) std::cerr << "Shader failed to load!" << std::endl;
	
	// Set initial fog values
	Grid battleField = Grid(numRows, numCols, cellSize);
	Game game(battleField);
	
	while (!WindowShouldClose()) {
		BeginDrawing();
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
		ClearBackground(BLACK);
		//BeginShaderMode(fogShader);
		BeginBlendMode(BLEND_ALPHA);
		
		//game.GetPlayer().DrawSpotLight(fogShader);
		BeginMode2D(game.camera);
		game.Update();
		
		game.Draw();
		EndMode2D();
		EndBlendMode();
		//EndShaderMode();
		
		DrawTextEx(font, "BEHIND ENEMY MINES", { 100, 850 }, 90, 2, WHITE);
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