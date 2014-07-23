#include <iostream>
#include <utility>
#include "SDLhelpers.h"
#include <vector>
#include <new>
#include <map>
#include "globals.h"
#include "Tile.h"
#include "Structures.h"
#include "Spaceman.h"
#include "Biodome.h"
#include "UI.h"
#include "InfoBox.h"
// Constant Declares
/* declared in globals.h
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

const int GRID_SIZE_X = 20;
const int GRID_SIZE_Y = 20;
const int TILE_SIZE_X = 50;
const int TILE_SIZE_Y = 50;


struct Tile
{
	bool walkable;
	int lightLevel;
	int airLevel;
	//bool empty;
	BasicStructure* structure;
};

struct Coord
{
	int x;
	int y;
};
*/
// Function declares
void printGrid(Tile**);
SDL_Texture* makeBackgroundTexture(SDL_Renderer*);
SDL_Texture*  makeLightTexture(SDL_Renderer* renderer, Tile** grid);
void makeLightTexture(SDL_Renderer*, Tile**, SDL_Texture*);
void updateLighting(Tile**, std::vector<SDL_Point>);
void updateLightingSingle(Tile**, int, int);
void moveCamera(SDL_Rect*, int, int);
void zoomCamera(SDL_Rect*, int);

int main(int argc, char **argv){	
	//Init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){ logSDLError(std::cout, "SDL_Init"); return 1; }
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){ logSDLError(std::cout, "IMG_Init"); return 1; }
	if (TTF_Init() != 0){ logSDLError(std::cout, "TTF_Init"); return 1; }
	// get window
	SDL_Window *window = SDL_CreateWindow("Space Sim | Jongy Games", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr){ logSDLError(std::cout, "SDL_CreateWindow"); return 1; }
	// get renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){ logSDLError(std::cout, "SDL_CreateRenderer"); return 1; }


	//Set up game grid and set to default tile
	Tile** tileGrid = (Tile**)malloc(GRID_SIZE_X*sizeof(Tile));
	for (int i = 0; i < GRID_SIZE_X; i++)
		tileGrid[i] = (Tile*)malloc(GRID_SIZE_Y * sizeof(Tile));
	
	for (int j = 0; j < GRID_SIZE_Y; j++)
	{
		for (int i = 0; i < GRID_SIZE_X; i++)
		{
			Tile basicTile;
			basicTile.walkable = true, basicTile.lightLevel = 0, basicTile.airLevel = 10;
			basicTile.structure = nullptr;
			tileGrid[i][j] = basicTile;
		}
	}

	/*
	std::cout << "Map test" << std::endl;
	std::map<char, int> maps;
	maps['a'] = 1;
	if (maps.count('a'))
	{
		std::cout << maps['a'] << std::endl;
	}
	std::cout << "Map test end" << std::endl;
	*/

	//Structure Vectors
	std::vector<SDL_Point> lights;
	std::vector<SDL_Point> beds;
	std::vector<Spaceman> spacemen;

	//updateLighting(tileGrid, lights);

	SDL_Texture* background = makeBackgroundTexture(renderer);
	SDL_Texture* lightingBG = makeLightTexture(renderer, tileGrid);

	// Object additions for testing
	Bed bed = Bed(6, 6, "../res/bed.png", renderer);
	bed.Draw(renderer, background);
	tileGrid[6][6].structure = &bed;
	beds.push_back(SDL_Point{ 6, 6 });

	Spaceman spaceman = Spaceman(100, 100, "../res/spaceman.png", renderer);


	int lmode = 0, rmode = 0; //altered inside UI, to tell main loop what to do with mouseclicks
	UI ui = UI(renderer, &lmode, &rmode);
	InfoBox infoBox = InfoBox(renderer, SDL_Point{ 0, SCREEN_HEIGHT });
	infoBox.Show(tileGrid[0][0]);
	std::cout << "After info box setup" << std::endl;

	/* Camera rect, dictates which part of full map texture is loaded to screen*/
	SDL_Rect cameraRect;
	cameraRect.x = 5 * TILE_SIZE_X; //snap x to tile size
	cameraRect.y = cameraRect.x * (SCREEN_WIDTH / SCREEN_HEIGHT);
	int camTile_x = 10, camTile_y = 10;
	cameraRect.w = camTile_x * TILE_SIZE_X;
	cameraRect.h = camTile_y * TILE_SIZE_Y;
	SDL_Event e;
	int quit = 0;
	
	SDL_Point* click = nullptr;
	while (!quit)
	{
		SDL_RenderClear(renderer);

		// World Draws:

		//background first
		SDL_RenderCopy(renderer, background, &cameraRect, NULL);
		/**/

		spaceman.Draw(renderer, cameraRect);
		

		// lighting layer last
		SDL_RenderCopy(renderer, lightingBG, &cameraRect, NULL);
		/**/

		// UI/Overlay draws
		ui.Draw(renderer);
		infoBox.Draw(renderer);

		//SDL_RenderCopy(renderer, lighting, NULL, NULL);
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				quit = 1;
			else if (e.type == SDL_KEYDOWN)
			{
				SDL_Keycode keySymbol = e.key.keysym.sym;
				if (keySymbol == SDLK_p) printGrid(tileGrid);
				switch (keySymbol)
				{
				//WASD moves camera
				case SDLK_d:
					moveCamera(&cameraRect, 1, 0);
					break;
				case SDLK_a:
					moveCamera(&cameraRect, -1, 0);
					break;
				case SDLK_w:
					moveCamera(&cameraRect, 0, -1);
					break;
				case SDLK_s:
					moveCamera(&cameraRect, 0, 1);
					break;
				//Arrow keys move character for debug
				case SDLK_RIGHT:
					std::cout << "Right" << std::endl;
					spaceman.x += 10;
					break;
				case SDLK_LEFT:
					spaceman.x -= 10;
					break;
				case SDLK_UP:
					spaceman.y -= 10;
					break;
				case SDLK_DOWN:
					spaceman.y += 10;
					break;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					std::cout << "Left Click" << std::endl << "Mode: " << rmode << std::endl;
					SDL_Point pos;
					pos.x = e.button.x;
					pos.y = e.button.y;
					// First check if UI or world is clicked

					if (collidepoint(pos, ui.GetRect()))

					{ //UI clicked
						ui.Click(pos);
					}

					else

					{ // World Clicked
						// clickedX and clickedY are the grid positions of the click
						SDL_Point clickedPos;
						clickedPos.x = (e.button.x * cameraRect.w) / (SCREEN_WIDTH * TILE_SIZE_X) + (cameraRect.x / TILE_SIZE_X);
						clickedPos.y = (e.button.y * cameraRect.h) / (SCREEN_HEIGHT * TILE_SIZE_Y) + (cameraRect.y / TILE_SIZE_Y);
						if (lmode == 1)
						{
							if (tileGrid[clickedPos.x][clickedPos.y].structure == nullptr)
							{
								if (click == nullptr)
								{
									// for rectangular non-singular placement
									// copy click location into new structure for first click
									SDL_Point newPoint = clickedPos;
									click = &newPoint;
								}
								else
								{
									std::cout << "Making biodome" << std::endl;
									Biodome biodome = Biodome(click->x, click->y, clickedPos.x, clickedPos.y, renderer, tileGrid);
									biodome.Draw(renderer, background);
									int k = 0;

									/*
									for (Wall* w : biodome.walls)
									{
									std::cout << w.getX() << ", " << w.getY() << std::endl;
									tileGrid[w.getX()][w.getY()].structure = &w;
									std::cout << k++ << ": " << w.getX() << ", " << w.getY() << std::endl;;
									w.Use();
									tileGrid[w.getX()][w.getY()].structure->Use();
									std::cout << std::endl;
									} //*/
									click = nullptr;
								}
							}
							else
							{
								std::cout << "Clicked pos: " << clickedPos.x << ", " << clickedPos.y << std::endl;
								std::cout << "object position data (x,y): " << tileGrid[clickedPos.x][clickedPos.y].structure->getX() << ", " << tileGrid[clickedPos.x][clickedPos.y].structure->getY() << std::endl;
								std::cout << std::endl;
							}
						}
						else
						{
							infoBox.Show(tileGrid[clickedPos.x][clickedPos.y]);
						}
					}
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					SDL_Point clickedPos;
					clickedPos.x = (e.button.x * cameraRect.w) / (SCREEN_WIDTH * TILE_SIZE_X) + (cameraRect.x / TILE_SIZE_X);
					clickedPos.y = (e.button.y * cameraRect.h) / (SCREEN_HEIGHT * TILE_SIZE_Y) + (cameraRect.y / TILE_SIZE_Y);
					if (tileGrid[clickedPos.x][clickedPos.y].structure == nullptr || rmode == 0)
					{
						/*
						tileGrid[clickedPos.x][clickedPos.y].structure = new Light(clickedPos.x, clickedPos.y, "../res/light.png", renderer);
						tileGrid[clickedPos.x][clickedPos.y].structure->Draw(renderer, background);
						lights.push_back(SDL_Point{ clickedPos.x, clickedPos.y });
						updateLightingSingle(tileGrid, clickedPos.x, clickedPos.y);
						lightingBG = makeLightTexture(renderer, tileGrid);
						tileGrid[clickedPos.x][clickedPos.y].structure->Use();

						/* tests of accessing object from grid 
						BasicStructure* l1 = tileGrid[clickedX][clickedY].structure;
						l1->Use();
						Light* l2 = dynamic_cast<Light*>  (tileGrid[clickedX][clickedY].structure);
						l2->Use();
						*/
						bool created = false;

						switch (rmode)
						{
						case 1: //Light
							tileGrid[clickedPos.x][clickedPos.y].structure = new Light(clickedPos.x, clickedPos.y, "../res/light.png", renderer);
							lights.push_back(SDL_Point{ clickedPos.x, clickedPos.y });

							updateLightingSingle(tileGrid, clickedPos.x, clickedPos.y);
							lightingBG = makeLightTexture(renderer, tileGrid);

							created = true;
							break;
						case 2: 
							tileGrid[clickedPos.x][clickedPos.y].structure = new Bed(clickedPos.x, clickedPos.y, "../res/bed.png", renderer);
							beds.push_back(SDL_Point{ clickedPos.x, clickedPos.y });
							created = true;
							break;
						case 3:
							std::cout << "mode 3!" << std::endl;
							break;	
						}

						if (created)
						{
							tileGrid[clickedPos.x][clickedPos.y].structure->Draw(renderer, background);
						}


					}
				}
			}
			else if (e.type == SDL_MOUSEWHEEL)
			{
				std::cout << "MouseWheel: " << e.wheel.y << std::endl;
				std::cout << "Pre cam x y w h: " << " " << cameraRect.x << " " << cameraRect.y << " " << cameraRect.w << " " << cameraRect.h << std::endl;
				zoomCamera(&cameraRect, -e.wheel.y);
				std::cout << "Post cam x y w h: " << " " << cameraRect.x << " " << cameraRect.y << " " << cameraRect.w << " " << cameraRect.h << std::endl;
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				//On mouse move, if new location is in UI, tell UI so it can check if buttons are hovered over
				SDL_Point newPos = { e.motion.x, e.motion.y };
				if (collidepoint(newPos, ui.GetRect())) ui.Update(newPos);
			}
		}
		SDL_RenderPresent(renderer);
	}





	// Free da shiz
	for (int i = 0; i < GRID_SIZE_X; i++)
		free(tileGrid[i]);
	free(tileGrid);

	return 0;
}

void printGrid(Tile** grid)
{
	for (int j = 0; j < GRID_SIZE_Y; j++)
	{
		for (int i = 0; i < GRID_SIZE_X; i++)
		{
			std::cout << grid[i][j].lightLevel << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


SDL_Texture* makeBackgroundTexture(SDL_Renderer* renderer)
{
	SDL_Texture *bgTexture = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, GRID_SIZE_X * TILE_SIZE_X, GRID_SIZE_Y * TILE_SIZE_Y);
	SDL_Texture *tileImage = loadTexture("../res/groundTile.png", renderer);
	SDL_SetRenderTarget(renderer, bgTexture); // draw to our grid texture
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect destRect;
	destRect.h = 50, destRect.w = 50;
	
	for (int j = 0; j < GRID_SIZE_Y; j++)
	{
		destRect.y = j * 50;
		for (int i = 0; i < GRID_SIZE_X; i++)
		{
			destRect.x = 50 * i;
			SDL_RenderCopy(renderer, tileImage, NULL, &destRect);
		}
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderTarget(renderer, NULL); // back to default, not bgTexture anymore
	return bgTexture;
}

/*SDL_Texture* */ void makeLightTexture(SDL_Renderer* renderer, Tile** grid, SDL_Texture *lightTexture)
{
	SDL_SetTextureBlendMode(lightTexture, SDL_BLENDMODE_BLEND);

	//SDL_Texture *lightTexture = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET/* | SDL_PIXELFORMAT_RGBA8888*/, GRID_SIZE_X * TILE_SIZE_X, GRID_SIZE_Y * TILE_SIZE_Y);
	SDL_Texture *tileImage = loadTexture("../res/lighting.png", renderer);
	//SDL_Texture *blackTex = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, TILE_SIZE_X, TILE_SIZE_Y);
	//SDL_SetRenderTarget(renderer, blackTex); // draw to our grid texture
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, lightTexture);
	SDL_Rect destRect, srcRect;
	destRect.h = 50, destRect.w = 50;
	srcRect.h = 50, srcRect.w = 50, srcRect.y = 0;
	int light;
	for (int j = 0; j < GRID_SIZE_Y; j++)
	{
		destRect.y = j * 50;
		for (int i = 0; i < GRID_SIZE_X; i++)
		{	
			light = grid[i][j].lightLevel;
			//succ = SDL_SetTextureAlphaMod(blackTex, (Uint8)((10 - light) * 20));
			//std::cout << "SetTextureAlphaMod: " << succ << std::endl;
			srcRect.x = (10 - light - 1) * 50;
			destRect.x = 50 * i;
			SDL_RenderCopy(renderer, tileImage, &srcRect, &destRect);
			
		}
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderTarget(renderer, NULL); // back to default, not bgTexture anymore
	//return lightTexture;
}
SDL_Texture*  makeLightTexture(SDL_Renderer* renderer, Tile** grid)
{
	//

	Uint32 pix;
	Uint8 alph = 255;
	int succ = 1;
	SDL_Texture *tileImage = loadTexture("../res/lighting.png", renderer);
	SDL_QueryTexture(tileImage, &pix, NULL, NULL, NULL);
	SDL_Texture *lightTexture = SDL_CreateTexture(renderer, pix, SDL_TEXTUREACCESS_TARGET, GRID_SIZE_X * TILE_SIZE_X, GRID_SIZE_Y * TILE_SIZE_Y);
	SDL_SetTextureBlendMode(lightTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(tileImage, SDL_BLENDMODE_NONE);
	SDL_SetRenderTarget(renderer, lightTexture); // draw to our grid texture

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	/*
	SDL_SetTextureAlphaMod(lightTexture, alph);
	std::cout << "pre pre alpha val: " << +alph << "  " << succ << std::endl;
	succ = SDL_GetTextureAlphaMod(lightTexture, &alph);
	std::cout << "pre alpha val: " << +alph << "  " << succ << std::endl;
	succ = SDL_GetTextureAlphaMod(lightTexture, &alph);
	std::cout << "post alpha val: " << +alph << "  " << succ << std::endl;
	//SDL_SetRenderTarget(renderer, lightTexture);
	*/
	SDL_Rect destRect, srcRect;
	destRect.h = 50, destRect.w = 50;
	srcRect.h = 50, srcRect.w = 50, srcRect.y = 0;
	int light;
	for (int j = 0; j < GRID_SIZE_Y; j++)
	{
		destRect.y = j * 50;
		for (int i = 0; i < GRID_SIZE_X; i++)
		{
			light = grid[i][j].lightLevel;
			//succ = SDL_SetTextureAlphaMod(blackTex, (Uint8)((10 - light) * 20));
			//std::cout << "SetTextureAlphaMod: " << succ << std::endl;
			srcRect.x = (10 - light - 1) * 50;
			destRect.x = 50 * i;
			SDL_RenderCopy(renderer, tileImage, &srcRect, &destRect);

		}
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderTarget(renderer, NULL); // back to default, not bgTexture anymore
	return lightTexture;
}
void updateLighting(Tile** grid, std::vector<SDL_Point> lightSources)
{
	int i,j, iXmin, iXmax, iYmin, iYmax;

	//Set all to dark
	for (j = 0; j < GRID_SIZE_Y; j++)
	{
		for (i = 0; i < GRID_SIZE_X; i++)
		{
			grid[i][j].lightLevel = 0;
		}
	}
	//int x, y;
	int iRadius = 1; // starting radius, also 10 - iRadius is the light level
	for (SDL_Point c : lightSources)
	{
		//x = c.x; y = c.y;
		grid[c.x][c.y].lightLevel = 10;
		for (int iRadius = 1; iRadius <= 11; iRadius++)
		{
			/* using iRadius to descide the upper and lower bounds for x and y, and loop round them */
			iXmin = c.x - iRadius;
			iXmax = c.x+ iRadius;
			iYmin = c.y - iRadius;
			iYmax = c.y + iRadius;


			/* +- 1 stops corner points being counted twice */
			/* Top L->R */
			if (iYmin >= 0)
			{
				for (i = iXmin + 1; i <= iXmax; i++)
				{
					if (i >= 0 && i < GRID_SIZE_X)
					{
						if (grid[i][iYmin].lightLevel < 10 - iRadius) grid[i][iYmin].lightLevel = 10 - iRadius;
					}
				}
			}
			/* R top -> bottom */
			if (iXmax < GRID_SIZE_X)
			{
				for (i = iYmin + 1; i <= iYmax; i++)
				{
					if (i >= 0 && i < GRID_SIZE_Y)
					{
						if (grid[iXmax][i].lightLevel < 10 - iRadius) grid[iXmax][i].lightLevel = 10 - iRadius;
					}
				}
			}
			/* Bottom R -> L */
			if (iYmax < GRID_SIZE_Y)
			{
				for (i = iXmax - 1; i >= iXmin; i--)
				{
					if (i >= 0 && i < GRID_SIZE_X)
					{
						if (grid[i][iYmax].lightLevel < 10 - iRadius) grid[i][iYmax].lightLevel = 10 - iRadius;
					}
				}
			}
			/* Left Bottom -> Top */
			if (iXmin >= 0)
			{
				for (i = iYmax - 1; i >= iYmin; i--)
				{
					if (i >= 0 && i < GRID_SIZE_Y)
					{
						if (grid[iXmin][i].lightLevel < 10 - iRadius) grid[iXmin][i].lightLevel = 10 - iRadius;
					}
				}
			}
		}
	}
}

void updateLightingSingle(Tile** grid, int sX, int sY)
{
	int i, j, iXmin, iXmax, iYmin, iYmax;

	//int x, y;
	int iRadius = 1; // starting radius, also 10 - iRadius is the light level
	//x = c.x; y = c.y;
	grid[sX][sY].lightLevel = 10;
	for (int iRadius = 1; iRadius <= 11; iRadius++)
	{
		/* using iRadius to descide the upper and lower bounds for x and y, and loop round them */
		iXmin = sX - iRadius;
		iXmax = sX + iRadius;
		iYmin = sY - iRadius;
		iYmax = sY + iRadius;


		/* +- 1 stops corner points being counted twice */
		/* Top L->R */
		if (iYmin >= 0)
		{
			for (i = iXmin + 1; i <= iXmax; i++)
			{
				if (i >= 0 && i < GRID_SIZE_X)
				{
					if (grid[i][iYmin].lightLevel < 10 - iRadius) grid[i][iYmin].lightLevel = 10 - iRadius;
				}
			}
		}
		/* R top -> bottom */
		if (iXmax < GRID_SIZE_X)
		{
			for (i = iYmin + 1; i <= iYmax; i++)
			{
				if (i >= 0 && i < GRID_SIZE_Y)
				{
					if (grid[iXmax][i].lightLevel < 10 - iRadius) grid[iXmax][i].lightLevel = 10 - iRadius;
				}
			}
		}
		/* Bottom R -> L */
		if (iYmax < GRID_SIZE_Y)
		{
			for (i = iXmax - 1; i >= iXmin; i--)
			{
				if (i >= 0 && i < GRID_SIZE_X)
				{
					if (grid[i][iYmax].lightLevel < 10 - iRadius) grid[i][iYmax].lightLevel = 10 - iRadius;
				}
			}
		}
		/* Left Bottom -> Top */
		if (iXmin >= 0)
		{
			for (i = iYmax - 1; i >= iYmin; i--)
			{
				if (i >= 0 && i < GRID_SIZE_Y)
				{
					if (grid[iXmin][i].lightLevel < 10 - iRadius) grid[iXmin][i].lightLevel = 10 - iRadius;
				}
			}
		}
	}
}
// Move camera dx*tile and dy*tile relatively
void moveCamera(SDL_Rect* cameraRect, int dx, int dy)
{
	cameraRect->x += dx * TILE_SIZE_X;
	cameraRect->y += dy * TILE_SIZE_Y;

	int xMax = (GRID_SIZE_X * TILE_SIZE_X) - cameraRect->w;
	int yMax = (GRID_SIZE_Y * TILE_SIZE_Y) - cameraRect->h;
	if (cameraRect->x < 0) cameraRect->x = 0;
	else if (cameraRect->x > xMax) cameraRect->x = xMax;

	if (cameraRect->y < 0) cameraRect->y = 0;
	else if (cameraRect->y > yMax) cameraRect->y = yMax;
}

void zoomCamera(SDL_Rect* cameraRect, int scroll)
{
	cameraRect->w += scroll * TILE_SIZE_X;
	//cameraRect->h += scroll * TILE_SIZE_Y;
	cameraRect->h = cameraRect->w * (SCREEN_WIDTH / SCREEN_HEIGHT);

	//check camera not wider than Grid
	if (cameraRect->w > GRID_SIZE_X * TILE_SIZE_X) cameraRect->w = GRID_SIZE_X * TILE_SIZE_X;
	if (cameraRect->h > GRID_SIZE_Y * TILE_SIZE_Y) cameraRect->h = GRID_SIZE_Y * TILE_SIZE_Y;

	//check camera not out of bounds
	int xMax = (GRID_SIZE_X * TILE_SIZE_X) - cameraRect->w;
	int yMax = (GRID_SIZE_Y * TILE_SIZE_Y) - cameraRect->h;
	if (cameraRect->x < 0) cameraRect->x = 0;
	else if (cameraRect->x > xMax) cameraRect->x = xMax;
	if (cameraRect->y < 0) cameraRect->y = 0;
	else if (cameraRect->y > yMax) cameraRect->y = yMax;
}
