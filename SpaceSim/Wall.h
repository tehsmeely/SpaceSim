#pragma once
#include "BasicStructure.h"

class Wall :
	public BasicStructure
{
public:
	SDL_Rect srcRect;
	int dir;
	Wall(int, int, std::string, SDL_Renderer*, int iDir);
	virtual void Use();
	virtual void Draw(SDL_Renderer*, SDL_Texture*);
	~Wall();
};

