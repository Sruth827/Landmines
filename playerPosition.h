#pragma once
#include "grid.h"

class Game;

class playerPosition {
public:
	playerPosition(Grid& grid, Game& gameReference);
	~playerPosition();
	void HandleInput();
	void RotatePlayerLeft();
	void RotatePlayerRight();
	void MovePlayerForward();
	void GenerateStartPosition(const Grid& grid);
	void GenerateHeloPosition(const Grid& grid);
	void Respawn(const Grid& grid);
	void Draw();
	void DrawSpotLight(Shader& fogShader, Camera2D& camera);
	bool HasMoved() const;

	int row;
	int column;
	int previousRow;
	int previousColumn; 
	int heloRow;
	int heloCol;
	enum Direction { UP, DOWN, LEFT, RIGHT };
	Direction playerDirection;
	float rotate;
	Vector2 texturePosition;
	Vector2 spotlightPos;
	float spotlightRadius;

	
	
private:
	Game& game;
	std::vector<std::vector<int>>gameGrid;
	int numRows;
	int numCols;
	int cellSize;
	Texture2D playerTexture;
	Texture2D helicopterTexture;
	
	
};


