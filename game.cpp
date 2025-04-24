#pragma once
#include <raylib.h>
#include "game.h"
#include <iostream>

Game::Game(Grid& grid) : Player(grid), grid(grid) {
	cellSize = 20; 
	gameGrid = grid.GetGameGrid();
	gameOver = false;
}

void Game::Draw()
{
	if (gameOver) {
		//Draw Gameover
	}
	else {
		grid.Draw();
		Player.Draw();
	}
}


void Game::HandleInput() {
	if (gameOver) {
		if (IsKeyPressed(KEY_R)) {
			Player.Respawn(grid);
			gameOver = false;
		}
		return;
	}
	int keyPressed = GetKeyPressed();
	switch (keyPressed) {
	case KEY_LEFT:
		MovePlayerLeft();
		break;
	case KEY_RIGHT:
		MovePlayerRight();
		break;
	case KEY_DOWN:
		MovePlayerDown();
		break;
	case KEY_UP:
		MovePlayerUp();
		break;
	}
}

void Game::Update()
{
	if (gameGrid[Player.row][Player.column] == 1) {  // If player hits a mine
		gameOver = true;
		std::cerr << "Game Over! Press R to Respawn" << std::endl;
	}
}

void Game::MovePlayerLeft()
{
	if (Player.column > 0 && Player.column < gameGrid[0].size()) {
		Player.column -= 1;
		Update();
	}
}

void Game::MovePlayerRight()
{
	if (Player.column >= 0 && Player.column < gameGrid[0].size() - 1) {
		Player.column += 1;
		Update();

	}
}

void Game::MovePlayerDown()
{
	if (Player.row >= 0 && Player.row < gameGrid.size() - 1) {
		Player.row += 1;
		Update();
	}
}

void Game::MovePlayerUp(){
	if (Player.row > 0 && Player.row < gameGrid.size()) {
		Player.row -= 1;
		Update();
	}
}

void Game::CheckCollision(playerPosition& player, const Grid& grid) {
	const std::vector<std::vector<int>>& gameGrid = grid.GetGameGrid();

	if (gameGrid[player.row][player.column] == 1) {  // If player hits a mine
		std::cerr << "Game Over! Respawning player..." << std::endl;
		player.Respawn(grid);
	}
}

