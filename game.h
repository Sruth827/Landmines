#pragma once
#include <vector>
#include <map>
#include <memory>
#include "grid.h"

class playerPosition;

class Game {
public:
	Game(Grid& grid);
	playerPosition& GetPlayer() { return *Player; }
	void LoadSounds();
	void Draw();
	void Update(); 
	bool gameOver; 

	Camera2D camera;
	Vector2 playerCameraPosition;

private:
	Grid& grid; 
	std::unique_ptr<playerPosition> Player;
	//void CheckCollision(playerPosition& player, const Grid& grid);
	void CheckForMineAhead(); 
	int cellSize;
	std::vector<std::vector<int>>gameGrid;
	
	bool soundPlayed;
	Sound explosion;
	Sound death;
	Sound ambientSound;
	Sound detector;
	Music titleMusic;

	float loopTime;
	float lastPlayTime; 
	float ambientTimer;
};
