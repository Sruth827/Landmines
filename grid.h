#pragma once
#include <set>
#include <vector>
#include "minePosition.h"

class Grid {
public:
	Grid(int Rows, int Columns, int CellSize);
	void Initialize();
	void Print(); 
	void Draw();
	int getRows() const;
	int getCols() const;
	int getCellSize() const; 
	const std::vector<std::vector<int>>& GetGameGrid() const;
	std::vector<Vector2> GetGeneratedPoints() const;
	Texture2D ground;


private:
	int numRows;
	int numCols;
	int cellSize;
	std::vector<std::vector<int>> gameGrid;
	std::set<std::pair<int, int>> generatedPoints;
	
	
	
};
