#pragma once
#include "grid.h"

class playerPosition {
public:
	playerPosition(Grid& grid);
	void GenerateStartPosition(const Grid& grid);
	void Respawn(const Grid& grid);
	void Draw();
	int row;
	int column;
private:
	std::vector<std::vector<int>>gameGrid;
	int numRows;
	int numCols;
	int cellSize;
};

