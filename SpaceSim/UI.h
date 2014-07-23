#pragma once
#include <SDL.h>
#include <vector>
#include "imageButton.h"
#include "SDLhelpers.h"
class UI
{
protected:
	SDL_Texture* image;
	SDL_Rect rect;
	int x, y;
	int *modeptr_L, *modeptr_R;
	std::vector<ImageButton*> buttons;
	std::vector<GangedImageButton*> gangedButtons;
public:
	UI(SDL_Renderer*, int*, int*);
	~UI();
	void Draw(SDL_Renderer*);
	void Click(SDL_Point);
	SDL_Rect GetRect() { return rect; }
	void Update(SDL_Point);
};

