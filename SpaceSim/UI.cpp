#include "UI.h"
#include <iostream>
#include "globals.h"

int UI_WIDTH = 100;

UI::UI(SDL_Renderer* ren, int* lmodeptr, int* rmodeptr)
{
	modeptr_L = lmodeptr;
	modeptr_R = rmodeptr;
	x = SCREEN_WIDTH - UI_WIDTH;
	y = 0;
	std::cout << "UI Constructor : " << this << std::endl;
	image = SDL_CreateTexture(ren, 0, SDL_TEXTUREACCESS_TARGET, UI_WIDTH, SCREEN_HEIGHT);
	if (image == 0) { std::cout << "PANIC!! UI texture creation failed, 0" << std::endl; }
	SDL_SetRenderTarget(ren, image);
	SDL_SetRenderDrawColor(ren, 177, 177, 177, 255);
	SDL_RenderClear(ren);
	rect = { x, y, UI_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderTarget(ren, NULL);


	buttons.push_back(new ImageButton(SDL_Point{ x + 20, y + 200 }, ren, "../res/biodomeButton.png", 1));
	

	std::string names[] = { "lightButton.png", "bedButton.png", "testButton.png" };
	int values[] = { 1, 2, 3 };
	GangedImageButton* gButtons = new GangedImageButton(SDL_Point{ x + 20, y + 20 }, "../res/", names, values, 3, ren);
	gangedButtons.push_back(gButtons);

}




UI::~UI()
{
}


void UI::Draw(SDL_Renderer* ren)
{
	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderCopy(ren, image, NULL, &rect);
	for (GangedImageButton* b : gangedButtons)
	{
		b->Draw(ren);
	}
	for (ImageButton* b : buttons)
	{
		b->Draw(ren);
	}
}

void UI::Click(SDL_Point pos)
{
	//convert window x,y to UI relative x,y
	SDL_Point relPos = { pos.x - x, pos.y - y };
	// Sanity/badcoding check
	if (relPos.x < 0 || relPos.y < 0){std::cout << "Something went wrong here, UI::Click() was called with a click not on its rect " << std::endl;}

	std::cout << "UI clicked. rel: " << relPos.x << ", " << relPos.y << std::endl;
	for (GangedImageButton* b : gangedButtons)
	{
		if (collidepoint(pos, b->GetRect()))
		{
			*modeptr_R = b->Click(pos);
			//b->GetValue();
		}
	}
	for (ImageButton* b : buttons)
	{
		if (collidepoint(pos, b->GetRect()))
		{
			*modeptr_L = b->Click();
			 //b->GetValue();
		}
	}
	

}

void UI::Update(SDL_Point pos)
{
	for (ImageButton* b : buttons)
	{
		if (collidepoint(pos, b->GetRect()))
		{
			b->HoverOn();
		}
		else
		{
			b->HoverOff();
		}
	}
	for (GangedImageButton* b : gangedButtons)
	{
		b->Hover(pos);
	}
}
