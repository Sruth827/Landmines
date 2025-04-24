#pragma once
#include <set>

class minePosition {
public:
	minePosition(); 
	std::set<std::pair<int, int>> getGeneratedPoints() const;

private:
	int numRows;
	int numCols;
	int cellSize;
	std::set<std::pair<int, int>> generatedPoints;
	std::set<std::pair<int, int>> SetMinePositions();

};
