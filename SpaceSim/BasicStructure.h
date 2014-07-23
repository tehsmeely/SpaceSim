#pragma once

#ifndef BASICSTRUCTURE
#define BASICSTRUCTURE

#include <SDL.h>
#include <string>
#include "globals.h"
#include <iostream>
class BasicStructure
{
protected:
	int x, y;
	SDL_Texture* image;
	SDL_Rect rect;
public:
	BasicStructure(int, int, std::string, SDL_Renderer*);
	~BasicStructure();
	virtual void Draw(SDL_Renderer*, SDL_Texture*);
	virtual void Use()
	{
		std::cout << "BasicScructureUse: " << x << ", " << y << "  :  " << this << std::endl;
	}
	int getX() {return x; }
	int getY() {return y; }
};


#endif
