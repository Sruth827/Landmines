#include "minePosition.h"	
#include <set>
#include <random>
#include <iostream>

minePosition::minePosition()
{
	numRows = 50;
	numCols = 30;
	cellSize = 20;
	generatedPoints = SetMinePositions();
}

std::set<std::pair<int, int>> minePosition::getGeneratedPoints() const
{
	return generatedPoints;
}

std::set<std::pair<int, int>> minePosition::SetMinePositions()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distX(0, this->numRows - 1);
	std::uniform_int_distribution<> distY(0, this->numCols - 1);

	// Use a set to store unique points

	const int numMines = 250; // Change this to the desired number of points
	std::set< std::pair<int, int >> mines;
	// Check if point already exists


	while (static_cast<int>(mines.size()) < numMines) {
		int x = distX(gen);
		int y = distY(gen);
		std::cout << "Trying to place mine at: (" << x << ", " << y << ")" << std::endl;
		mines.insert({ x, y });
	}

	return mines;
}


