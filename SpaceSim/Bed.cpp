#include "Bed.h"
#include <string>
#include <iostream>

Bed::Bed(int gX, int gY, std::string imageName, SDL_Renderer* ren) : BasicStructure(gX, gY, imageName, ren)
{
}

void Bed::Use()
{
	std::cout << "Using Bed" << std::endl;
}

Bed::~Bed()
{
}
