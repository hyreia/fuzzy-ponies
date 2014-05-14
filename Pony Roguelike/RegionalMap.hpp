#ifndef REGIONAL_MAP_HPP
#define REGIONAL_MAP_HPP

//#include <vector>
#include <list>
//#include "MapTile.hpp"

/* Basically an array of tiles that can be inhabited by actors. Since the majority of tiles will be bare, we do not keep any pointers to them however. 
Each tile has a reference to what type of tile it is. From that we can get the tile's property. */

class Actor;

struct RegionalMap
{
	RegionalMap(): startX(0), startY(0){}
	RegionalMap(int startX, int startY):
	startX(startX), startY(startY)
	{}
	~RegionalMap(){}
	void InitToField(int width, int height, int tileMaterial);
	std::vector< std::vector< MapTile> > tile;

	int startX, startY;
	std::list<int> actorIDs;
};

#endif