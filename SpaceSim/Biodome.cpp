#include "Biodome.h"


Biodome::Biodome(int iX, int iY, int x2, int y2, SDL_Renderer* ren, Tile** grid)
{
	x = iX;
	y = iY;
	size_x = x2 - x;
	size_y = y2 - y;

	//make corners
	grid[x][y].structure = new Wall(x, y, "../res/biodomeCorner.png", ren, 0);
	grid[x + size_x][y].structure = new Wall(x + size_x, y, "../res/biodomeCorner.png", ren, 1);
	grid[x + size_x][y + size_y].structure = new Wall(x + size_x, y + size_y, "../res/biodomeCorner.png", ren, 2);
	grid[x][y + size_y].structure = new Wall(x, y + size_y, "../res/biodomeCorner.png", ren, 3);
	Wall* wall0 = dynamic_cast<Wall*> (grid[x][y].structure);
	Wall* wall1 = dynamic_cast<Wall*> (grid[x+size_x][y].structure);
	Wall* wall2 = dynamic_cast<Wall*> (grid[x+size_x][y+size_y].structure);
	Wall* wall3 = dynamic_cast<Wall*> (grid[x][y+size_y].structure);
	walls.push_back(wall0);
	walls.push_back(wall1);
	walls.push_back(wall2);
	walls.push_back(wall3);
	grid[x][y].walkable = false;
	grid[x + size_x][y].walkable = false;
	grid[x + size_x][y + size_y].walkable = false;
	grid[x][y + size_y].walkable = false;
	///*
	// make walls
	// top
	for (int i = x+1; i < x2; i++)
	{
		grid[i][y].structure = new Wall(i, y, "../res/biodomeWall.png", ren, 0);
		Wall* wall = dynamic_cast<Wall*>(grid[i][y].structure);
		walls.push_back(wall);
		grid[i][y].walkable = false;
	}
	//right
	for (int j = y + 1; j < y2; j++)
	{
		grid[x2][j].structure = new Wall(x2, j, "../res/biodomeWall.png", ren, 1);
		Wall* wall = dynamic_cast<Wall*>(grid[x2][j].structure);
		walls.push_back(wall);
		grid[x2][j].walkable = false;
	}
	//bottom
	for (int i = x + 1; i < x2; i++)
	{
		grid[i][y2].structure = new  Wall(i, y2, "../res/biodomeWall.png", ren, 2);
		Wall* wall = dynamic_cast<Wall*>(grid[i][y2].structure);
		walls.push_back(wall);
		grid[i][y2].walkable = false;
	}
	//left
	for (int j = y + 1; j < y2; j++)
	{
		grid[x][j].structure = new Wall(x, j, "../res/biodomeWall.png", ren, 3);
		Wall* wall = dynamic_cast<Wall*>(grid[x][j].structure);
		walls.push_back(wall);
		grid[x][j].walkable = false;
	}
	//*/
}

void Biodome::Draw(SDL_Renderer* renderer, SDL_Texture* background)
{
	for (Wall* wall : walls)
	{
		wall->Draw(renderer, background);
	}
}


Biodome::~Biodome()
{
}


