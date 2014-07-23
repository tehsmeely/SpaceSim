#ifndef TILE_H
#define TILE_H
#include "BasicStructure.h"
struct Tile
{
	bool walkable;
	int lightLevel;
	int airLevel;
	BasicStructure* structure;
};


#endif