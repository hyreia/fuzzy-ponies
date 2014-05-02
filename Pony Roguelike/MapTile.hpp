#ifndef MAP_TILE_HPP
#define MAP_TILE_HPP

struct MapTile
{
	MapTile(): materialTypeIndex(0), variant(0)
	{}
	MapTile(int materialTypeIndex): materialTypeIndex(materialTypeIndex), variant(0)
	{}
	~MapTile(){}

	int materialTypeIndex;
	int variant;
};

#endif