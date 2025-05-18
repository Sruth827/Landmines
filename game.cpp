#pragma once
#include <raylib.h>
#include "game.h"
#include <iostream>
#include "playerPosition.h"
#include "AirRaid.h"

Game::Game(Grid& grid) : grid(grid) {
	Player = std::make_unique<playerPosition>(grid, *this);
	this->spawnAirRaid();
	cellSize = 20;
	gameGrid = grid.GetGameGrid();
	gameOver = false;
	soundPlayed = false;
	loopTime = 5.0f;
	lastPlayTime = 0.0f;
	ambientTimer = 0.0f;

	InitAudioDevice();
	LoadSounds();
	camera.target = Vector2{ (float)Player->row, (float)Player->column };
	camera.offset = Vector2{ 400.0f, 300.0f };  // centered on screen
	camera.rotation = 0.0f;
	camera.zoom = 2.5f; 

}

Game::~Game()
{
	AirRaids.clear();
}

void Game::LoadSounds() {
	explosion = LoadSound("sfx/hq-explosion.ogg");
	death = LoadSound("sfx/scream.ogg");
	titleMusic = LoadMusicStream("sfx/titlemusic.wav");
	ambientSound = LoadSound("sfx/ambient war sfx.ogg");
	detector = LoadSound("sfx/detectorsound.wav");
	SetSoundVolume(ambientSound, 0.6f);
	SetSoundVolume(explosion, 0.6f);
	SetMusicVolume(titleMusic, 0.2f);	
}


void Game::Draw()
{
	if (gameOver) {

	}
	else {
		grid.Draw();
		Player->Draw();
		for (const auto& air_raid : AirRaids) {
			air_raid->Draw();
		}
	}
}


void Game::Update()
{
	if (gameGrid[Player->row][Player->column] == 1) {  // if player hits a mine
		gameOver = true;
		std::cerr << "Game Over! Press R to Respawn" << std::endl;
	}
	if (gameOver && !soundPlayed) {
		PlaySound(death);
		PlaySound(explosion);
		soundPlayed = true;  // mark that sound has played
	}

	if (gameOver && IsKeyPressed(KEY_R)) {
		Player->Respawn(grid);
		gameOver = false;
		soundPlayed = false;  // Reset sound trigger for next death
	}

	bombTimer += GetFrameTime();  // Accumulate time
	if (bombTimer >= bombInterval) {
		spawnAirRaid();  // Spawn a new bombing run
		bombTimer = 0.0f;  // Reset timer after spawning
	}
	
	if (!gameOver) {
		//linear interpolation 
		Player -> HandleInput();
		CheckForMineAhead();
		camera.target.x += (Player->column * cellSize - camera.target.x) * 0.1f;
		camera.target.y += (Player->row * cellSize - camera.target.y) * 0.1f;

		UpdateMusicStream(titleMusic);
		if (!IsMusicStreamPlaying(titleMusic)) {
			PlayMusicStream(titleMusic);
		}
		if (!IsSoundPlaying(ambientSound)) {
			PlaySound(ambientSound);
		}
			
		
	}
	else {
		StopMusicStream(titleMusic);
	}

}

void Game::spawnAirRaid()
{
	std::cout << "new AirRaid created";
	AirRaids.emplace_back(std::make_unique<AirRaid>(grid, *this));
}



/*void Game::CheckCollision(playerPosition& player, const Grid& grid) {
	const std::vector<std::vector<int>>& gameGrid = grid.GetGameGrid();

	// Ensure row and column are within bounds
	if (player.row >= 0 && player.row < gameGrid.size() &&
		player.column >= 0 && player.column < gameGrid[0].size()) {

		if (gameGrid[player.row][player.column] == 1) {  // If player hits a mine
			std::cerr << "Game Over! Respawning player..." << std::endl;
			player.Respawn(grid);
		}
	}
}
*/

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
			if (!IsSoundPlaying(detector)) {
				PlaySound(detector);
				std::cout << "Mine detected! Playing sound...\n";

			}
		}
		else {
			if (IsSoundPlaying(detector)) {
				StopSound(detector);
				std::cout << "No mine ahead! Stopping sound...\n";

			}
		}
	}
}





