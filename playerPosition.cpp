#include <raylib.h>
#include <iostream>
#include <random>
#include "playerPosition.h"


playerPosition::playerPosition(Grid& grid) {
    cellSize = grid.getCellSize();
    numRows = grid.getRows();
    numCols = grid.getCols();
    const std::vector<std::vector<int>>& gameGrid = grid.GetGameGrid();
    GenerateStartPosition(grid);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(numRows - 4, numRows - 1);
    std::uniform_int_distribution<> distY(0, numCols - 4);
}

bool isSafePosition(int row, int col, const std::vector<std::vector<int>>& gameGrid) {
    const int radius = 1;  // Safe distance from mines

    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            int checkRow = row + dx;
            int checkCol = col + dy;

            // Ensure it's within bounds before checking
            if (checkRow >= 0 && checkRow < gameGrid.size() &&
                checkCol >= 0 && checkCol < gameGrid[0].size()) {
                
                if (gameGrid[checkRow][checkCol] == 1) {
                    return false;  // Not a safe position
                }
            }
        }
    }
    return true;  // Safe position
}

void playerPosition::GenerateStartPosition(const Grid& grid) {
    std::random_device rd;
    std::mt19937 gen(rd());

    int startRow_min = grid.getRows() - 5;
    int startRow_max = grid.getRows() - 1;

    std::uniform_int_distribution<> distRow(startRow_min, startRow_max);
    std::uniform_int_distribution<> distCol(0, grid.getCols() - 1);
    
    const std::vector<std::vector<int>>& gameGrid = grid.GetGameGrid();
    int newRow;
    int newCol;
    int attempts = 0;
    const int maxAttempts = 1000;  // Prevent infinite loops

    std::cout << "Grid Rows: " << grid.getRows() << " (Start Range: "
        << startRow_min << "-" << startRow_max << ")" << std::endl;


    do {
        newRow = distRow(gen);
        newCol = distCol(gen);
        attempts++;
    } while (!isSafePosition(newRow, newCol, gameGrid) && attempts < maxAttempts);

    if (attempts >= maxAttempts) {
        std::cerr << "Warning: Could not find a safe position for the player!" << std::endl;
        newRow = startRow_max;  // Default fallback position
        newCol = grid.getCols() / 2;
    }

    row = newRow;
    column = newCol;
}

void playerPosition::Respawn(const Grid& grid) {
    GenerateStartPosition(grid);  // Reassign the player's position
}


void playerPosition::Draw()
{
	DrawRectangle(column * cellSize+30, row * cellSize+30, cellSize - 1, cellSize - 1, RED);

}




