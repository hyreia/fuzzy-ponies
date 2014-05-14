#include "main.hpp"
#include "Random.hpp"

void RegionalMap::InitToField(int width, int height, int tileMaterial)
{
	std::vector<MapTile> mapColumn(height, MapTile(0));
	std::vector<std::vector<MapTile> > mapRow(width, mapColumn);
	tile = mapRow;

	int r = rng::Rand(4);

	//int r = rng::rand(4);
	for(int x = 0; x < width; x++) for(int y=0; y < height; y++)
	{
		int r = rng::Rand(4);
		tile[x][y].variant = r;
		
	}

}