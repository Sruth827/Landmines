#pragma once
#include <raylib.h>
#include "game.h"
#include <iostream>
#include "playerPosition.h"
#include "AirRaid.h"
#include "config.h"

Game::Game(Grid& grid) : grid(grid) {
	
	Player = std::make_unique<playerPosition>(grid, *this);
	Player->GenerateHeloPosition(grid);
	
	//this->spawnAirRaid();
	cellSize = CELL_SIZE;
	gameGrid = grid.GetGameGrid();

	gameOver = false;
	gameWon = false;
	timeRemaining = 120.0f;

	isMineAhead = false;
	soundPlayed = false;
	audioUnlocked = false;
	
	loopTime = 5.0f;
	lastPlayTime = 0.0f;
	ambientTimer = 0.0f;

	InitAudioDevice();
	LoadSounds();
	camera.target = Vector2{ (float)Player->row, (float)Player->column };
	camera.offset = Vector2{ 400.0f, 300.0f };  // centered on screen
	camera.rotation = 0.0f;
	camera.zoom = 2.0f; 

}

Game::~Game()
{
	AirRaids.clear();
}

void Game::LoadSounds() {
	explosion = LoadSound("sfx/hq-explosion.ogg");
	death = LoadSound("sfx/scream.ogg");
	titleMusic = LoadMusicStream("sfx/titlemusic.ogg");
	ambientSound = LoadSound("sfx/ambient war sfx.ogg");
	detector = LoadSound("sfx/detectorsound.ogg");
	SetSoundVolume(ambientSound, 0.6f);
	SetSoundVolume(explosion, 0.6f);
	SetMusicVolume(titleMusic, 0.2f);	
}

void Game::InitializeGraphics()
{
	int textureWidth = tileX * cellSize + 30;
	int textureHeight = tileY * cellSize + 30;
	fogTexture = LoadRenderTexture(textureWidth, textureHeight);
	SetTextureFilter(fogTexture.texture, TEXTURE_FILTER_BILINEAR);

	// Initialize fogPixels with solid fog
	fogPixels.resize(textureWidth * textureHeight);
	for (int i = 0; i < fogPixels.size(); i++) {
		fogPixels[i] = { 0, 0, 0, 255 }; // opaque black fog
	}
	UpdateTexture(fogTexture.texture, fogPixels.data());
}


void Game::Draw()
{
	if (!gameOver) {
		grid.Draw();
		Player->Draw();
		
		/*for (const auto& air_raid : AirRaids) {
			air_raid->Draw();
		}

		//std::cout << "Drawing Fog..." << std::endl;
		DrawFog();
		*/
	}
}


void Game::Update(Shader& fogShader)
{
#if defined(PLATFORM_WEB)
	// This is the check to unlock audio.
	if (!audioUnlocked && GetKeyPressed() != 0)
	{
		printf("USER INPUT DETECTED: Unlocking audio now.\n");
		audioUnlocked = true;
}
#else
	// On desktop, audio is always unlocked.
	audioUnlocked = true;
#endif

	// This lets us test playing a sound with the 'J' key.
	if (IsKeyPressed(KEY_J))
	{
		printf("--- 'J' key pressed! --- \n");
		if (audioUnlocked)
		{
			printf("Attempting to play explosion sound because audio IS unlocked.\n");
			PlaySound(explosion);
		}
		else
		{
			printf("Did NOT play explosion sound because audio is STILL locked.\n");
		}
	}
	// First, handle the respawn input, as it resets the entire game state.
	if ((gameOver || gameWon) && IsKeyPressed(KEY_R)) {
		Player->Respawn(grid);
		Player->GenerateHeloPosition(grid); // Re-place the helicopter for a new round
		gameOver = false;
		gameWon = false;
		soundPlayed = false;
		timeRemaining = 120.0f; // Reset timer
#if defined(PLATFORM_WEB)
		audioUnlocked = false;
#endif
		return; // Exit early to start the next frame fresh
	}

	// --- Main Game Logic ---
	// Only run the main logic if the game is not over and not won yet.
	if (!gameOver && !gameWon) {

		// 1. Update Timer & Check for Time-Out Loss Condition
		timeRemaining -= GetFrameTime();
		if (timeRemaining <= 0.0f) {
			timeRemaining = 0.0f;
			gameOver = true; // Player loses if time runs out
		}

		// 2. Handle Player Input
		Player->HandleInput();

		//Audio fix
		if (audioUnlocked)
		{
			// This is the ONLY place we update the music stream now.
			UpdateMusicStream(titleMusic);

			// This is your original "keep-alive" logic, which is great for looping.
			// It will now work on the web because it's inside the 'audioUnlocked' gate.
			if (!IsMusicStreamPlaying(titleMusic)) { PlayMusicStream(titleMusic); }
			if (!IsSoundPlaying(ambientSound)) { PlaySound(ambientSound); }
		}

		// 3. Check Game State After the Move
		// Check for WIN condition FIRST.
		if (Player->row == Player->heloRow && Player->column == Player->heloCol) {
			gameWon = true;
			StopMusicStream(titleMusic); // Stop the tense music on win
		}
		// Only if the player hasn't won, check for MINE collision (lose condition).
		else if (gameGrid[Player->row][Player->column] == 1) {
			gameOver = true;
		}

		// 4. Handle Other Per-Frame Updates (like audio, camera, etc.)
		// Check the game state again in case it just changed in the step above.
		CheckForMineAhead();
		Player->DrawSpotLight(fogShader, this->camera);

		float screenHeight = (float)GetScreenHeight();
		SetShaderValue(fogShader, GetShaderLocation(fogShader, "screenHeight"), &screenHeight, SHADER_UNIFORM_FLOAT);

		camera.target.x += (Player->column * cellSize - camera.target.x) * 0.1f;
		camera.target.y += (Player->row * cellSize - camera.target.y) * 0.1f;

	}

	// --- Handle Audio For End States ---
	if (gameOver && !soundPlayed) {
		StopMusicStream(titleMusic);
		PlaySound(death);
		PlaySound(explosion);
		soundPlayed = true;
	}
}

/*void Game::spawnAirRaid()
{
	std::cout << "new AirRaid created";
	AirRaids.emplace_back(std::make_unique<AirRaid>(grid, *this));
}

void Game::UpdateVisibility(const std::unique_ptr<playerPosition>& player) {
	int visibilityRadius = 20;
	bool updated = false;
	int textureWidth = tileX * cellSize;

	for (int y = player->row - visibilityRadius; y <= player->row + visibilityRadius; y++) {
		for (int x = player->column - visibilityRadius; x <= player->column + visibilityRadius; x++) {
			if (x >= 0 && x < tileX && y >= 0 && y < tileY) {
				int index = y * tileX + x;
				if (tileFog[index] != 1) {  // if this tile hasn't been cleared yet
					tileFog[index] = 1;

					// Update pixel region for the cell:
					for (int py = 0; py < cellSize; py++) {
						for (int px = 0; px < cellSize; px++) {
							int pixelIndex = (y * cellSize + py) * textureWidth + (x * cellSize + px);
							if (pixelIndex < fogPixels.size()) {
								fogPixels[pixelIndex] = { 0, 0, 0, 0 };  // fully transparent
							}
						}
					}
					updated = true;
				}
			}
		}
	}

	if (updated) {
		UpdateTexture(fogTexture.texture, fogPixels.data());
	}
}
*/




void Game::DrawFog()
{
	Rectangle sourceRect = { 0, 0, (float)fogTexture.texture.width, (float)fogTexture.texture.height };
	Rectangle destRect = { 0, 0, (float)(tileX * cellSize), (float)(tileY * cellSize) };
	DrawTexturePro(fogTexture.texture, sourceRect, destRect, { 0.0f, 0.0f }, 0.0f, WHITE);

}



void Game::CheckForMineAhead() {
	std::map<playerPosition::Direction, Vector2> directionOffsets = {
		{playerPosition::Direction::UP,    {0, -1}},
		{playerPosition::Direction::DOWN,  {0, 1}},
		{playerPosition::Direction::LEFT,  {-1, 0}},
		{playerPosition::Direction::RIGHT, {1, 0}}
	};

	Vector2 offset = directionOffsets[Player->playerDirection];
	int nextRow = Player->row + (int)offset.y;
	int nextCol = Player->column + (int)offset.x;

	if (nextRow >= 0 && nextRow < grid.getRows() &&
		nextCol >= 0 && nextCol < grid.getCols()) {

		if (grid.GetGameGrid()[nextRow][nextCol] == 1) {
			isMineAhead = true; 
			if (!IsSoundPlaying(detector)) {
				PlaySound(detector);
				std::cout << "Mine detected! Playing sound...\n";
			}
		}
		else {
			isMineAhead = false; 
			if (IsSoundPlaying(detector)) {
				StopSound(detector);
				std::cout << "No mine ahead! Stopping sound...\n";
			}
		}
	}
	else {
		isMineAhead = false; 
	}
}







