#pragma once
#include "SDLhelpers.h"
#include "globals.h"
#include "Tile.h"

class InfoBox
{
private:
	SDL_Renderer* renderer;
	SDL_Texture *image, *infoOverlay;
	SDL_Rect rect;
	TTF_Font* font;
public:
	InfoBox(SDL_Renderer*, SDL_Point);
	~InfoBox();
	void Show(Tile);
	void Draw(SDL_Renderer*);
};

