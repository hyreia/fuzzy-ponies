#ifndef MAP_HPP
#define MAP_HPP

//#include <vector>
#include <list>
//#include "MapTile.hpp"

/* Basically an array of tiles that can be inhabited by actors. Since the majority of tiles will be bare, we do not keep any pointers to them however. 
Each tile has a reference to what type of tile it is. From that we can get the tile's property. */

class Actor;
class Item;

struct Map
{
	Map(){}
	~Map(){}
	void InitToField(int width, int height, int tileMaterial);
	std::vector< std::vector< MapTile> > tile;

	std::list<Actor*> actors;
};

#endif