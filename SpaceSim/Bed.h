#pragma once
#include "BasicStructure.h"
class Bed :
	public BasicStructure
{
public:
	Bed(int, int, std::string, SDL_Renderer*);
	void Use();
	~Bed();
};

