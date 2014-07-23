#include "BasicStructure.h"
#include <string>
#include "SDLhelpers.h"
#include "TextureManager.h"

BasicStructure::BasicStructure(int gX, int gY, std::string imageName, SDL_Renderer* ren)
{
	std::cout << "BasicScructure constructor, " << this << std::endl;
	x = gX;
	y = gY;
	//image = loadTexture(imageName, ren);
	image = textureManager.loadImage(ren, imageName);
	rect.w = TILE_SIZE_X; rect.h = TILE_SIZE_Y;
	rect.x = x * TILE_SIZE_X; rect.y = y * TILE_SIZE_Y;
}

void BasicStructure::Draw(SDL_Renderer* ren, SDL_Texture* gridTex)
{
	SDL_SetRenderTarget(ren, gridTex);
	SDL_RenderCopy(ren, image, NULL, &rect);
	SDL_SetRenderTarget(ren, NULL);
}

BasicStructure::~BasicStructure()
{
}
