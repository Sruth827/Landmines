#include <raylib.h>
#include <iostream>
#include <random>
#include "playerPosition.h"
#include "game.h"



playerPosition::playerPosition(Grid& grid, Game& gameReference) : game(gameReference) {
    
    cellSize = grid.getCellSize();
    numRows = grid.getRows();
    numCols = grid.getCols();
    const std::vector<std::vector<int>>& gameGrid = grid.GetGameGrid();
    GenerateStartPosition(grid);
    previousRow = row;
    previousColumn = column;
    Direction playerDirection = UP;
   

    
    //texttures
    playerTexture = LoadTexture("assests/survivor-idle_flashlight_0.png");
    helicopterTexture = LoadTexture("assests/helo1.png");
}


playerPosition::~playerPosition() {
    UnloadTexture(playerTexture);
}

void playerPosition::HandleInput() {
    int keyPressed = GetKeyPressed();
    switch (keyPressed) {
    case KEY_LEFT:
        RotatePlayerLeft();
        break;
    case KEY_RIGHT:
        RotatePlayerRight();
        break;
    case KEY_UP:
        MovePlayerForward();
        break;
    }
}

void playerPosition::RotatePlayerLeft() {
    if (playerDirection == Direction::UP) {
        playerDirection = Direction::LEFT;
    }
    else if (playerDirection == Direction::LEFT) {
        playerDirection = Direction::DOWN;
    }
    else if (playerDirection == Direction::DOWN) {
        playerDirection = Direction::RIGHT;
    }
    else if (playerDirection == Direction::RIGHT){
        playerDirection = Direction::UP;
    }
}

void playerPosition::RotatePlayerRight() {
    if (playerDirection == Direction::UP) {
        playerDirection = Direction::RIGHT;
    }
    else if (playerDirection == Direction::RIGHT) {
        playerDirection = Direction::DOWN;
    }
    else if (playerDirection == Direction::DOWN) {
        playerDirection = Direction::LEFT;
    }
    else if (playerDirection == Direction::LEFT) {
        playerDirection = Direction::UP;
    }
}

void playerPosition::MovePlayerForward() {
    previousRow = row;
    previousColumn = column; 

    if (playerDirection == Direction::UP && row > 0) {
        row -= 1;
    }
    else if (playerDirection == Direction::DOWN && row < numRows - 1){
        row += 1;
    }
    else if (playerDirection == Direction::LEFT && column > 0) {
        column -= 1;
    }
    else if (playerDirection == Direction::RIGHT && column < numCols - 1) {
        column += 1;
    }
}

static bool isSafePosition(int row, int col, const std::vector<std::vector<int>>& gameGrid) {
    const int radius = 1;  // Safe distance from mines

    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            int checkRow = row + dx;
            int checkCol = col + dy;

            // Ensure it's within bounds before checking
            if (checkRow >= 0 && checkRow < gameGrid.size() &&
                checkCol >= 0 && checkCol < gameGrid[0].size()) {
                
                if (gameGrid[checkRow][checkCol] == 1) {
                    return false;  // not safe
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
    const int maxAttempts = 1000;  //avoid infinite loops

    std::cout << "Grid Rows: " << grid.getRows() << " (Start Range: "
        << startRow_min << "-" << startRow_max << ")" << std::endl;


    do {
        newRow = distRow(gen);
        newCol = distCol(gen);
        attempts++;
    } while (!isSafePosition(newRow, newCol, gameGrid) && attempts < maxAttempts);

    if (attempts >= maxAttempts) {
        std::cerr << "Warning: Could not find a safe position for the player!" << std::endl;
        newRow = startRow_max;  // default fallback position
        newCol = grid.getCols() / 2;
    }

    row = newRow;
    column = newCol;
}

void playerPosition::GenerateHeloPosition(const Grid& grid){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distRow(0, 9); 
    std::uniform_int_distribution<> distCol(0, grid.getCols() - 1); 

    int heliRow, heliCol;
    const std::vector<std::vector<int>>& gameGrid = grid.GetGameGrid();
    int attempts = 0;
    const int maxAttempts = 1000;  // avoid infinite loops

    do {
        heliRow = distRow(gen);
        heliCol = distCol(gen);
        attempts++;
    } while (gameGrid[heliRow][heliCol] == 1 && attempts < maxAttempts);  //check to make sure it wont be placed on mine

    if (attempts >= maxAttempts) {
        std::cerr << "Warning: Could not find a safe helicopter landing spot!" << std::endl;
        heliRow = 0; // default position
        heliCol = grid.getCols() / 2;
    }

    heloRow = heliRow;
    heloCol = heliCol;

    std::cout << " Helicopter placed at: (" << heloRow << ", " << heloCol << ")" << std::endl;
}

void playerPosition::Respawn(const Grid& grid) {
    GenerateStartPosition(grid);  // Reassign the player's position
}

void playerPosition::Draw() {
    //DrawRectangle(column * cellSize + 30, row * cellSize + 30, cellSize - 1, cellSize - 1, BROWN);
    DrawTextureEx(helicopterTexture, { (float)(heloCol * cellSize), (float)(heloRow * cellSize) }, 0.0f, 0.15f, WHITE);

    if (playerDirection == Direction::UP) {
        rotate = 270.0f;
    }
    else if (playerDirection == Direction::RIGHT) {
        rotate = 0.0f;
    }
    else if (playerDirection == Direction::DOWN) {
        rotate = 90.0f;
    }
    else if (playerDirection == Direction::LEFT) {
        rotate = 180.0f;
    }

    Vector2 texturePosition = { (float)(column * cellSize) + 40, (float)(row * cellSize) + 37 };
    Vector2 rotationOrigin = { playerTexture.width * 0.06f, playerTexture.height * 0.06f };  

    DrawTexturePro(playerTexture,
        { 0, 0, (float)playerTexture.width, (float)playerTexture.height },  
        { texturePosition.x, texturePosition.y, (float)playerTexture.width * 0.12f, (float)playerTexture.height * 0.12f },  
        rotationOrigin,  
        rotate,
        WHITE);
}

void playerPosition::DrawSpotLight(Shader& fogShader, Camera2D& camera){
        
    Vector2 playerWorldPos = { (float)(column * cellSize) + 40, (float)(row * cellSize) + 37 };
    Vector2 playerScreenPos = GetWorldToScreen2D(playerWorldPos, camera);
    SetShaderValue(fogShader, GetShaderLocation(fogShader, "spotlightPos"), &playerScreenPos, SHADER_UNIFORM_VEC2);
    float spotlightRadiusValue = 700.0f;
    SetShaderValue(fogShader, GetShaderLocation(fogShader, "spotlightRadius"), &spotlightRadius, SHADER_UNIFORM_FLOAT);

 }

bool playerPosition::HasMoved() const
{
    return previousRow != row || previousColumn != column;
}




/*void playerPosition::Draw()
{   //this is how we will end up rotating
    if (playerDirection == Direction::UP) {
        rotate = 270.0f;
    }
    else if (playerDirection == Direction::RIGHT) {
        rotate = 0.0f;
    }
    else if (playerDirection == Direction::DOWN) {
        rotate = 90.0f;
    }
    else if (playerDirection == Direction::LEFT) {
        rotate = 180.0f;
    }
    DrawRectangle(column * cellSize + 30, row * cellSize + 30, cellSize - 1, cellSize - 1, GRAY);
    DrawTextureEx(playerTexture, { (float)(column * cellSize) + 25,(float)(row * cellSize) + 50 }, rotate, 0.12f, WHITE);

}
*/



