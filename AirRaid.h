#pragma once
#include <set>
#include <raylib.h>
#include <random>
#include "grid.h"

class Game; 

class AirRaid {
public:
	AirRaid(Grid& grid, Game& gameReference);
	~AirRaid();
	std::vector<std::pair<int, int>> DropTarget();
	std::pair<int, int> TakeOff();
	Vector2 MoveTowards(Vector2 current, Vector2 target, float speed);
	float Distance(Vector2 a, Vector2 b);
	void Draw();

private:
	int cellSize;
	float airSpeed = 5.0;
	int currentTargetIndex = 0;
	Vector2 currentTarget; 
	std::vector<std::pair<int, int>> dropTarget;
	std::pair<int, int> takeOff;
	Texture2D bomberTexture;  
	std::mt19937 gen;


};