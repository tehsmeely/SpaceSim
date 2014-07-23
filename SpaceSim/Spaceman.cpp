#include "Spaceman.h"
#include "SDLhelpers.h"
#include <iostream>
#include "globals.h"
Spaceman::Spaceman(int iX, int iY, std::string imageName, SDL_Renderer* ren)
{
	x = iX;
	y = iY;
	image = loadTexture(imageName, ren);
	// init needs to 100% satiated
	food = 100;air = 100;sanity = 100;rest = 100;
}

void Spaceman::Draw(SDL_Renderer* renderer, SDL_Rect cameraRect)
{
	//convert map position 
	
	//check is in cameraRect -Note: will have to handle width of image later (maybe with offsets in below logic)
	bool inX = (x > cameraRect.x) && (x < cameraRect.x + cameraRect.w);
	bool inY = (y > cameraRect.y) && (y < cameraRect.y + cameraRect.h);
	if (inX && inY)
	{
		
		SDL_Rect drawRect;
		drawRect.w = (trueSize * SCREEN_WIDTH) / cameraRect.w;
		drawRect.h = (trueSize * SCREEN_HEIGHT) / cameraRect.h;
		drawRect.x = ((x - cameraRect.x) * SCREEN_WIDTH) / cameraRect.w;
		drawRect.y = ((y - cameraRect.y) * SCREEN_HEIGHT) / cameraRect.h;
		SDL_RenderCopy(renderer, image, NULL, &drawRect);
		//std::cout << "Drawing spaceman" << drawRect.x << ", " << drawRect.y << " - " << drawRect.w << ", " << drawRect.h <<std::endl;
	}
}

Spaceman::~Spaceman()
{
}
