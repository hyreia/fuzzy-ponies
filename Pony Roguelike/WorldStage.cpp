#include "main.hpp"
#include <sstream>

WorldStage::WorldStage()
{}

WorldStage::~WorldStage()
{}

void WorldStage::Start()
{
	CreateMaterials();



	if(!actorList.LoadActorsFromFile())
	{
		int mapWidth = 80;
		int mapHeight = 80;
		actorList.CreateSomeActorsRandomly(currentMap, mapWidth, mapHeight);
	}

	LoadMapFromFileOrCreateNewMap();



}

void WorldStage::CreateMaterials()
{
	materials.push_back(Material("grass", "a patch of grass", '.', ',', ',', '.', 
		Color::Black, Color::DarkGreen, false, GRASS_TILE_IMAGE, 0));

	materials.push_back(Material("stone wall", "a rough stone wall", '#', '#', '#', '#',
		Color::DarkGrey, Color::Grey, true, STONE_TILE_IMAGE, 0));
}

void WorldStage::Shutdown()
{
	SaveMapToFile(currentMap);
	delete currentMap;

	actorList.SaveActorsToFile();
}
void WorldStage::LoadMapFromFileOrCreateNewMap()
{
	const int MAP_WIDTH = 80;
	const int MAP_HEIGHT = 80;
	const int ARBITRARY_NUMBER_OF_WALLS = 20;
	const int VALID_VERSION = 0;
	currentMap = new RegionalMap();
	currentMap->InitToField(MAP_WIDTH, MAP_HEIGHT, 0);

	//Determine map player is on
	int mapX = 0, mapY = 0;
	auto player = actorList.GetActor(1);
	if(player)
	{
		mapX = player->x/MAP_WIDTH;
		mapY = player->y/MAP_HEIGHT;
	}

	//Load that one
	std::stringstream ss;
	ss << "region." << mapX << "." << mapY;
	
	//Try to load the region by that name
	ALLEGRO_FILE *file = al_fopen(ss.str().c_str(), "r");
	if(file)
	{
		const int BUF_SIZE = 512;
		char buffer[BUF_SIZE];
		bool isValid = true;

		if(!al_fgets(file, buffer, BUF_SIZE))
		{
			std::string str = buffer;
			if(!str.empty() && str[str.length()-1] == '\n') str.erase(str.length()-1);
			if(strcmp(str.c_str(), "b272bda9bf0c1cdcba614b5ed99c4d62") != 0)
			{
				isValid = false;
			}
		}
		if(isValid && !al_fgets(file, buffer, BUF_SIZE))
		{
			std::string str = buffer;
			if(!str.empty() && str[str.length()-1] == '\n') str.erase(str.length()-1);
			if(strcmp(str.c_str(), "0") != 0)
			{
				isValid = false;
			}
		}

		if(isValid)
		{

			for(int y = 0; y < MAP_HEIGHT; y++) for(int x = 0; x < MAP_WIDTH; x++)
			{
				//returns bytes read, 32bits = 4 bytes
				int material = al_fread32le(file);
				if(!al_feof(file))
				{
					currentMap->tile[x][y].materialTypeIndex = material;
				}
			}

			//Get actor IDs
			int actorId = al_fread32le(file);
			while(!al_feof(file))
			{
				currentMap->actorIDs.push_back(actorId);
				actorId = al_fread32le(file);
			}
		}
		al_fclose(file);
	}
	else //no file
	{
		for(int nWalls = 0; nWalls < ARBITRARY_NUMBER_OF_WALLS; nWalls++)
		{
			//Manually place down some walls to run into
			int x = rng::Rand(MAP_WIDTH);
			int y = rng::Rand(MAP_HEIGHT);
			currentMap->tile[x][y].materialTypeIndex = 1;
		}
	}


}

void WorldStage::SaveMapToFile(RegionalMap *map)
{
	std::stringstream ss;
	const int MAP_WIDTH=80, MAP_HEIGHT=80;

	//Open a file based on map position
	ss << "region." << map->startX/MAP_WIDTH << "." << map->startY/MAP_HEIGHT;
	ALLEGRO_FILE *file = al_fopen(ss.str().c_str(), "w");
	if(file)
	{
		//Write magic number to it and newline
		al_fputs(file, "b272bda9bf0c1cdcba614b5ed99c4d62");
		al_fputs(file, "\n");

		//Write version number and newline
		al_fputs(file, "0\n");

		//Write each tile as a 32le
		for(int y = 0; y < MAP_HEIGHT; y++) for(int x = 0; x < MAP_WIDTH; x++)
		{
			al_fwrite32le(file, map->tile[x][y].materialTypeIndex);
		}

		//Write each actor id as a 32le
		for(auto actorId = map->actorIDs.begin(); actorId != map->actorIDs.end(); actorId++)
		{
			al_fwrite32le(file, (*actorId));
		}


		//Close a file
		al_fclose(file);
	}
}

void WorldStage::SetCurrentMap(RegionalMap *regionalMap)
{
	currentMap = regionalMap;
}