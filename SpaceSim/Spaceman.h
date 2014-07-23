#pragma once
#include <SDL.h>
#include <string>
class Spaceman
{
public:
	int food, air, sanity, rest;
	SDL_Texture* image;
	SDL_Rect rect;
	int trueSize = 50;
	int x, y; // map position
	Spaceman(int, int, std::string, SDL_Renderer*);
	void Draw(SDL_Renderer*, SDL_Rect);
	~Spaceman();
};

