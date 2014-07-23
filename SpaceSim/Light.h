#pragma once
#include "BasicStructure.h"
class Light :
	public BasicStructure
{
public:
	Light(int, int, std::string, SDL_Renderer*);
	virtual void Use();
	~Light();
};

