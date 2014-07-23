#include "Light.h"
#include <string>

Light::Light(int gX, int gY, std::string imageName, SDL_Renderer* ren) : BasicStructure(gX, gY, imageName, ren)
{
	std::cout << "Light constructor, " << this << std::endl;
}

void Light::Use()
{
	std::cout << "Light use " << x << ", " << y << "  :  " << this << std::endl;
}

Light::~Light()
{
}
