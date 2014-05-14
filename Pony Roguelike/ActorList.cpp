#include "main.hpp"
#include <allegro5/allegro.h>
#include <string>
#include <sstream>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if(!item.empty()) elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

ActorList::ActorList(): nextAvailableId(1)
{}

ActorList::~ActorList()
{}

bool ActorList::LoadActorsFromFile()
{
	Actor *pendingActor = NULL;
	int actorId = nextAvailableId;
	//try to get file
	ALLEGRO_FILE *file = al_fopen("actors.act", "r");
	if(file)
	{
	//if we have an open file
		const int BUF_SIZE = 512;
		char buffer[BUF_SIZE];
		while(al_fgets(file, buffer, BUF_SIZE))
		{
			std::string str = buffer;
			if (!str.empty() && str[str.length()-1] == '\n') str.erase(str.length()-1);
			
			//if a line starts with a tab, it's a component to actor being assembled
			if(buffer[0] == '\t')
			{
				//tokenize the line
					//use the first and maybe second parts to determine the component type
					//deserialize a component of that type with the tokenized string
					//add it to the actor being assembled
			}
			else if(buffer[0] == '#' || buffer[0] == ';'){}
			else //it must be for an actor
			{
				//add any pending actor being assembled to the map, clear pending space
				if(pendingActor)
				{
					InsertActor(actorId, pendingActor);
					pendingActor = NULL;
				}
				//parse out the id
				auto parseOutId = split(str, '|');
				if(parseOutId.size() > 1)
				{
					actorId = atoi(parseOutId[0].c_str());
					auto tokenized = split(parseOutId[1], ActorComponent::DELIM);
					pendingActor = new Actor(0, 0, 0, Color::Black, 0);
					pendingActor->Deserialize(tokenized);
				}
				//else syntax is wrong, either malformed or not actor line, move along
				
			}
		}
	//Once the file has been fully read, close it
	al_fclose(file);
	}
	else
	{
		return false;
	}
	//If we have any pending actors, add them to the map
	if(pendingActor)
	{
		InsertActor(actorId, pendingActor);
		pendingActor = NULL;
	}

	return true;
}

void ActorList::CreateSomeActorsRandomly(RegionalMap *map, int mapWidth, int mapHeight)
{

	auto player = new Actor(5, 5, '@', Color::Red, MARKER_PONY_IMAGE);
	InsertActor(1, player);

	int numOfGriffins = rng::RandInRange(10, 20);
	int x = 0;
	int y = 0;
	bool isClear = true;
	for(int griffin = 0; griffin < numOfGriffins; griffin++)
	{
		int spawnAttempts = 0;
		isClear = true;
		do
		{
			
			spawnAttempts++;
			if(spawnAttempts > 10) break;
			x = rng::Rand(mapWidth);
			y = rng::Rand(mapHeight);
			auto actorsList = GetActors();
			
			//oh god, this line is unsafe
			//isClear = !game->world.materials[map->tile[x][y].materialTypeIndex].isBlocking;

			while(!actorsList.empty() && isClear)
			{

				if(actorsList.top()->x == x &&
					actorsList.top()->y == y && actorsList.top()->isBlocking)
				{
					isClear = false;
				}
				actorsList.pop();
			}
			
		} while(!isClear);

		//G for griffin!
		if(isClear)
		{
			auto newActor = new Actor(x, y, 'G', Color::Orange, GRIFFIN_IMAGE);
			InsertActor(griffin+2, newActor);
		}
	}

}

#pragma warning(push)
#pragma warning(disable: 4996) //vsprintf is unsafe and depricated but vsprintf_s is nonstandard
void ActorList::SaveActorsToFile()
{
	//Open actors file in text mode
	ALLEGRO_FILE *file = al_fopen("actors.act", "w");

	//Iterate through each actor
	for(auto actor = actors.begin(); actor != actors.end(); actor++)
	{
		char id_buffer[13];
		sprintf(id_buffer, "%d", (*actor).first);
		//Get serialize string for Actor class
		std::string actorLine = (*actor).second->Serialize();
		//Insert the actor ID and actorId separator
		actorLine.insert(0, "|");
		actorLine.insert(0, id_buffer);
		actorLine.append("\n");
		//Write it to the file
		al_fwrite(file, actorLine.c_str(), strlen(actorLine.c_str()));

		//Then serialize each component of theirs
		
			//Writing each of those components to the file as well, beginning with a tab.
	}
	
	al_fclose(file);

}
#pragma warning(pop)


Actor *ActorList::GetActor(int id)
{
	if(actors.count(id) == 1) return actors[id];
	else return NULL;
}

void ActorList::RemoveActor(int id)
{}

void ActorList::FreeActorList()
{
	for(auto actor = actors.begin(); actor != actors.end(); actor++)
	{
		delete (*actor).second;
	}
	actors.clear();
}

int ActorList::InsertActor(Actor *actor)
{
	//Find vacant slot
	while(actors.count(nextAvailableId)==1) nextAvailableId++;

	InsertActor(nextAvailableId, actor);

	return nextAvailableId++;

}

void ActorList::InsertActor(int id, Actor *actor)
{
	if(actors.count(id)==1) delete actors[id];

	actors.insert(std::pair<int, Actor*>(id, actor));
}

std::priority_queue<Actor*, std::vector<Actor*>, ActorComparator> ActorList::GetActors()
{
	std::priority_queue<Actor*, std::vector<Actor*>, ActorComparator> ret;

	for(auto a = actors.begin(); a != actors.end(); a++)
	{
		ret.push( (*a).second );

	}

	return ret;
}