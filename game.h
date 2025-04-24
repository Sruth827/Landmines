#pragma once
#include <vector>
#include <map>
#include "playerPosition.h"
#include "grid.h"
#include "game.h"

class Game {
public:
	Game(Grid& grid);
	void Draw();
	void HandleInput();
	void Update(); 
	bool gameOver; 
private:
	Grid grid; 
	playerPosition Player;
	void MovePlayerLeft();
	void MovePlayerRight();
	void MovePlayerDown();
	void MovePlayerUp();
	void CheckCollision(playerPosition& player, const Grid& grid);
	int cellSize;
	std::vector<std::vector<int>>gameGrid;
};
