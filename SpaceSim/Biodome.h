#pragma once
#include "BasicStructure.h"
#include <vector>
#include "Wall.h"
#include "Tile.h"
class Biodome
{
public:
	std::vector<Wall*> walls;
	int x, y, size_x, size_y;
	Biodome(int, int, int, int, SDL_Renderer*, Tile**);
	void Draw(SDL_Renderer*, SDL_Texture*);
	~Biodome();
};

