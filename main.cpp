#include <raylib.h>
#include <raymath.h>
#include "game.h"
#include "grid.h"
#include "config.h"
#include "playerPosition.h"
#include <iostream>
#include <filesystem>
#include <fstream>


int main() {
	printf("Raylib Version: %s\n", RAYLIB_VERSION);
	int win_height = 1080;
	int win_width = 1080;
	InitWindow(win_width, win_height, "Landmines");
	SetTargetFPS(60);

	Font font = LoadFontEx("Font/armalite.ttf", 64, 0, 0);

	Shader fogShader = LoadShader(0, "shaders/fog.fs");
	if (fogShader.id == 0) std::cerr << "Shader failed to load!" << std::endl;
	
	// Set initial fog values
	Grid battleField = Grid(NUM_ROWS, NUM_COLS, CELL_SIZE);
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
		DrawTextEx(font, "BEHIND ENEMY MINES", { 90, 900 }, 100, 2, BLACK);
		//DrawTextEx(font, "ENEMY", { 700, 120 }, 90, 2, WHITE);
		//DrawTextEx(font, "MINES", { 700, 210 }, 90, 2, WHITE);

		int minutes = (int)game.timeRemaining / 60; 
		int seconds = (int)game.timeRemaining % 60; 
		const char* timerText = TextFormat("%02i:%02i", minutes, seconds);
		DrawTextEx(font, "EXFIL!", { 800, 20 }, 50, 2, WHITE);
		DrawTextEx(GetFontDefault(), timerText, {810, 70}, 50, 2, WHITE);

		if (game.isMineAhead && !game.gameOver && !game.gameWon) {

			DrawRectangle(325, 50, 345, 60, Fade(DARKGRAY, 0.7f));
			DrawRectangleLines(325, 50, 345, 60, WHITE);
			DrawTextEx(font, "PROXIMITY ALERT", { 327, 65 }, 40, 2, RED);

		}
		
		Vector2 controlsPosition = { 40, 680 };
		float keySize = 50.0f;
		float keySpacing = 10.0f;
		float titleFontSize = 40.0f;
		float labelFontSize = 30.0f;
		Color keyColor = WHITE;
		Color textColor = WHITE;


		Rectangle titleRec = { controlsPosition.x, controlsPosition.y, 0, titleFontSize };
		Rectangle keysRec = { controlsPosition.x, titleRec.y + titleRec.height + 5, (keySize * 3) + (keySpacing * 2), keySize };
		Rectangle labelsRec = { controlsPosition.x, keysRec.y + keysRec.height + 5, 0, labelFontSize };


		Rectangle panelRec = {
			controlsPosition.x - 15,
			controlsPosition.y - 15,
			keysRec.width + 30,
			(titleRec.height + keysRec.height + labelsRec.height + 25)
		};

	
		DrawRectangleRec(panelRec, Fade(DARKGRAY, 0.7f));


		DrawTextEx(font, "CONTROLS", { titleRec.x, titleRec.y }, titleRec.height, 2, textColor);


		Vector2 leftKeyPos = { keysRec.x, keysRec.y };
		Vector2 upKeyPos = { keysRec.x + keySize + keySpacing, keysRec.y };
		Vector2 rightKeyPos = { keysRec.x + (keySize + keySpacing) * 2, keysRec.y };

		// Draw key boxes
		DrawRectangleLinesEx({ leftKeyPos.x, leftKeyPos.y, keySize, keySize }, 2, keyColor);
		DrawRectangleLinesEx({ upKeyPos.x, upKeyPos.y, keySize, keySize }, 2, keyColor);
		DrawRectangleLinesEx({ rightKeyPos.x, rightKeyPos.y, keySize, keySize }, 2, keyColor);

		
		DrawTriangle(
			{ upKeyPos.x + keySize / 2, upKeyPos.y + 15 },
			{ upKeyPos.x + 15, upKeyPos.y + keySize - 15 },
			{ upKeyPos.x + keySize - 15, upKeyPos.y + keySize - 15 },
			keyColor
		);


		DrawTriangle(
			{ leftKeyPos.x + keySize - 15, leftKeyPos.y + 15 },             
			{ leftKeyPos.x + keySize - 15, leftKeyPos.y + keySize - 15 },    
			{ leftKeyPos.x + 15, leftKeyPos.y + keySize / 2 },                 
			keyColor
		);

		// Right Arrow
		DrawTriangle(
			{ rightKeyPos.x + keySize - 15, rightKeyPos.y + keySize / 2 },
			{ rightKeyPos.x + 15, rightKeyPos.y + 15 },
			{ rightKeyPos.x + 15, rightKeyPos.y + keySize - 15 },
			keyColor
		);

		// Draw text labels for the keys
		DrawTextEx(font, "ROTATE/FORWARD/ROTATE", { labelsRec.x, labelsRec.y }, 10, 2, textColor);
		
		if (game.gameWon) {
			DrawTextEx(font, "YOU WIN!", { 350, 400 }, 100, 2, WHITE);
			DrawTextEx(font, "EXTRACTION REACHED", { 50, 500 }, 100, 2, WHITE);
			DrawTextEx(font, "Press R to Play Again!", { 100, 600 }, 100, 2, WHITE);
		}
		else if (game.gameOver) {
			DrawTextEx(font, "GAMEOVER!", {300, 400}, 100, 2, WHITE);
			DrawTextEx(font, "Press R to Respawn", {100, 500}, 100, 2, WHITE);
		}
		
		
		
		EndDrawing(); 
	}
	CloseAudioDevice();
	CloseWindow(); 
	
	return 0;
}