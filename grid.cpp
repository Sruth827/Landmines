#include <raylib.h>
#include "grid.h"
#include <iostream>
#include <set>
#include <random>

Grid::Grid(int Rows, int Columns, int CellSize)
{
	numRows = Rows;
	numCols = Columns;
	cellSize = CellSize;
	minePosition mineField;
	generatedPoints = mineField.getGeneratedPoints();
	ground = LoadTexture("assests/muddy_ground.png");
	Initialize();
}


void Grid::Initialize()
{
	gameGrid.assign(numRows, std::vector<int>(numCols, 0));

	for (const auto& point : generatedPoints) {
		int x = point.first;
		int y = point.second;
		if (x >= 0 && x < numRows && y >= 0 && y < numCols) {
			gameGrid[x][y] = 1;
		}else {
			std::cerr << "Invalid mine position (" << x << ", " << y << ")!" << std::endl;
		}
	}
	
	for (const auto& point : generatedPoints) {
		std::cout << "(" << point.first << ", " << point.second << ")";
	}
	std::cout << std::endl;

	for (int row = 0; row < numRows; row++) {
		for (int column = 0; column < numCols; column++) {
			std::cout << gameGrid[row][column] << " ";
		}
		std::cout << std::endl;
	}
}

void Grid::Print()
{
	for (int row = 0; row < numRows; row++) {
		for (int column = 0; column < numCols; column++) {
			std::cout << gameGrid[row][column] << " ";
		}
		std::cout << std::endl;
	}
}

void Grid::Draw()
{
	DrawTextureEx(ground, { 30, 30 }, 0.0f, 0.8f, WHITE); 
	/*for (int row = 0; row < numRows; row++) {
		for (int column = 0; column < numCols; column++) {
			int cellValue = gameGrid[row][column];
			DrawRectangle(column * cellSize + 30, row * cellSize + 30, cellSize - 1, cellSize - 1, BROWN);
		}
	}
	
	for (const auto& point : generatedPoints)
	{
		int x = point.first;
		int y = point.second;
		DrawRectangle(y * cellSize + 30, x * cellSize + 30, cellSize - 1, cellSize - 1, DARKBROWN);
	}
	*/
}

int Grid::getRows() const
{
	return numRows;
}

int Grid::getCols() const
{
	return numCols;
}

int Grid::getCellSize() const
{
	return cellSize;
}

const std::vector<std::vector<int>>& Grid::GetGameGrid() const
{
	return gameGrid;
}

std::vector<Vector2> Grid::GetGeneratedPoints() const {
	std::vector<Vector2> convertedPoints;

	for (const auto& point : generatedPoints) {
		convertedPoints.push_back(Vector2{ static_cast<float>(point.first), static_cast<float>(point.second) });
	}
	std::cout << "Generated Mines:\n";
	for (const auto& point : generatedPoints) {
		std::cout << "Mine at (" << point.first << ", " << point.second << ")\n";
	}


	return convertedPoints;
}











