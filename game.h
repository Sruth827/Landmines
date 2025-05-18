#pragma once
#include <vector>
#include <map>
#include <memory>
#include "grid.h"
#include "AirRaid.h"

class playerPosition;
class AirRaid;

class Game {
public:
	Game(Grid& grid);
	~Game();
	playerPosition& GetPlayer() { return *Player; }
	void LoadSounds();
	void Draw();
	void Update(); 
	bool gameOver; 
	void spawnAirRaid();


	Camera2D camera;
	Vector2 playerCameraPosition;

private:
	Grid& grid; 
	std::unique_ptr<playerPosition> Player;
	//void CheckCollision(playerPosition& player, const Grid& grid);
	void CheckForMineAhead(); 
	int cellSize;
	std::vector<std::vector<int>>gameGrid;
	std::vector<std::unique_ptr<AirRaid>> AirRaids;
	bool soundPlayed;
	Sound explosion;
	Sound death;
	Sound ambientSound;
	Sound detector;
	Music titleMusic;

	float loopTime;
	float lastPlayTime; 
	float ambientTimer;

	float bombTimer = 0.0f;  // Tracks time since last bombing run
	float bombInterval = 16.0f;  // Time between bombing runs (seconds)
};
