#include "Wall.h"
#include <iostream>

Wall::Wall(int gX, int gY, std::string imageName, SDL_Renderer* ren, int iDir) : BasicStructure(gX, gY, imageName, ren)
{
	std::cout << "Wall Constructor, " << this << std::endl;
	dir = iDir;
	Uint32 pix;
	int w, h;
	/* old method, wall created new texture of specific size
	 Now, just store pointer to whole text and keep srcRect for drawing
	 required overwrite of BasicStructure::Draw()

	SDL_QueryTexture(image, &pix, NULL, &w, &h);
	SDL_Texture* newTex = SDL_CreateTexture(ren, pix, SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetRenderTarget(ren, newTex);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_SetTextureBlendMode(newTex, SDL_BLENDMODE_BLEND);
	SDL_RenderClear(ren);
	*/
	/*
	SDL_Point rotPoint= { 24, 24 };
	SDL_Rect destRect;
	SDL_RenderCopyEx(ren, image, NULL, &destRect, dir * 90.0, &rotPoint, SDL_FLIP_NONE);
	*/
	srcRect = { 50 * iDir, 0, 50, 50 };
	/*
	SDL_RenderCopy(ren, image, &srcRect, NULL);
	SDL_SetRenderTarget(ren, NULL);
	SDL_DestroyTexture(image);
	image = newTex;
	SDL_QueryTexture(image, NULL, NULL, &w, &h);
	std::cout << "wall new tex w, h: " << w << ", " << h << std::endl;
	std::cout << "wall xy: " << x << ", " << y << std::endl;
	*/
}

void Wall::Draw(SDL_Renderer* ren, SDL_Texture* bgTex)
{
	SDL_SetRenderTarget(ren, bgTex);
	SDL_RenderCopy(ren, image, &srcRect, &rect);
	SDL_SetRenderTarget(ren, NULL);
}

void Wall::Use()
{
	std::cout << "Wall Use! pos: " << x << ", " << y << "  :  " << this << std::endl;
}
Wall::~Wall()
{
}
