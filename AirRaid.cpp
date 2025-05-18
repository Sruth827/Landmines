#include "AirRaid.h"
#include <random>
#include <iostream>

AirRaid::AirRaid(Grid& grid, Game& gameReference) 
	: gen(std::random_device{}()) {
	this->takeOff = TakeOff(); 
	this->dropTarget = DropTarget();
	std::cout << "Take off location (" << takeOff.first << ", " << takeOff.second << ")" << std::endl;
	cellSize = grid.getCellSize();
	bomberTexture = LoadTexture("assests/2d-jet-clipart-9.png");
	currentTarget = { static_cast<float>(takeOff.first), static_cast<float>(takeOff.second) };
}

AirRaid::~AirRaid()
{
}

std::vector<std::pair<int, int>> AirRaid::DropTarget()
{
	std::uniform_int_distribution<> distX(2, 30);
	std::uniform_int_distribution<> distY(2, 50);

	int numTargets = 5;
	std::vector< std::pair<int, int >> targets;
	// check if point already exists


	while (static_cast<int>(targets.size()) < numTargets) {
		int x = distX(gen);
		int y = distY(gen);
		std::cout << "Trying to place mine at: (" << x << ", " << y << ")" << std::endl;
		targets.push_back({ x, y });
	}

	return targets;
}

std::pair<int, int> AirRaid::TakeOff()
{
	std::uniform_int_distribution<> distX(0, 30);
	std::uniform_int_distribution<> distY(40, 50);
	return { distX(gen), distY(gen) };
	
}

Vector2 AirRaid::MoveTowards(Vector2 current, Vector2 target, float speed) {
	Vector2 direction = { target.x - current.x, target.y - current.y };
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);

	if (length > 1.0f) {  // Prevent jitter at final position
		direction.x /= length;
		direction.y /= length;
		current.x += direction.x * speed;
		current.y += direction.y * speed;
	}

	return current;
}

float AirRaid::Distance(Vector2 a, Vector2 b) {
	return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

void AirRaid::Draw()
{
	DrawTextureEx(bomberTexture, { (float)(currentTarget.x * cellSize), (float)(currentTarget.y * cellSize) }, 0.0f, 1.0f, WHITE);

	

	if (currentTargetIndex < dropTarget.size()) {
		currentTarget = MoveTowards(currentTarget, { static_cast<float>(dropTarget[currentTargetIndex].first),
													 static_cast<float>(dropTarget[currentTargetIndex].second) }, 0.15f);

		if (Distance(currentTarget, { static_cast<float>(dropTarget[currentTargetIndex].first),
									  static_cast<float>(dropTarget[currentTargetIndex].second) }) < 1.0f) {
			currentTargetIndex++;
		}
	}
	else {
		currentTarget.x += 15.0f;  // Move off-screen after all targets are hit
	}
	

}
