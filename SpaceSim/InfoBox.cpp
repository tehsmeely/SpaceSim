#include "InfoBox.h"

InfoBox::InfoBox(SDL_Renderer* ren, SDL_Point bottomleft)
{
	image = textureManager.loadImage(ren, "../res/infoBox.png");
	int w, h;
	SDL_QueryTexture(image, NULL, NULL, &w, &h);
	rect = { bottomleft.x, bottomleft.y - h, w, h };
	renderer = ren;

	std::string fontpath = "../res/TinDog.ttf";
	font = TTF_OpenFont(fontpath.c_str(), 11);
	if (font == nullptr) std::cout << ">>ERROR<< Issue opening ttf font: " << fontpath << std::endl;

	//Create a full transparent texture to draw the different info texts to when shown
	infoOverlay = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetTextureBlendMode(infoOverlay, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer, infoOverlay);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
}


InfoBox::~InfoBox()
{
	TTF_CloseFont(font);
}

// Covers same types as will be fed in, then wokrs out how to show them
// currently, first 3 details of tile struct
void InfoBox::Show(Tile tile)
{

	//using std::string;
	SDL_Rect drawingRect = { 70, 0, 0, 0 };

	SDL_Texture* walkText = renderText((tile.walkable) ? "Yes" : "No",
		SDL_Colour{ 0, 0, 0, 255 }, renderer, font);

	SDL_Texture* lightText = renderText(std::to_string(tile.lightLevel),
		SDL_Colour{ 0, 0, 0, 255 }, renderer, font);

	SDL_Texture* airText = renderText(std::to_string(tile.airLevel),
		SDL_Colour{ 0, 0, 0, 255 }, renderer, font);

	int w, h;

	SDL_SetRenderTarget(renderer, infoOverlay);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	//Walk: (70, 40)
	SDL_QueryTexture(walkText, NULL, NULL, &w, &h);
	drawingRect = { 70, 40, w, h };
	SDL_RenderCopy(renderer, walkText, NULL, &drawingRect);

	//Light: (70, 57)
	SDL_QueryTexture(lightText, NULL, NULL, &w, &h);
	drawingRect = { 70, 57, w, h };
	SDL_RenderCopy(renderer, lightText, NULL, &drawingRect);

	//Air: (70,73)
	SDL_QueryTexture(airText, NULL, NULL, &w, &h);
	drawingRect = { 70, 73, w, h };
	SDL_RenderCopy(renderer, airText, NULL, &drawingRect);

	SDL_SetRenderTarget(renderer, NULL);
}

void InfoBox::Draw(SDL_Renderer* ren)
{
	SDL_RenderCopy(ren, image, NULL, &rect);
	SDL_RenderCopy(ren, infoOverlay, NULL, &rect);
}