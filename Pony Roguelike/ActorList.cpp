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
			else if(buffer[0] == '#' || buffer[0] == ';'){	printf("%s\n", str.c_str()); }
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
					printf("%d\n", actorId);
					auto tokenized = split(parseOutId[1], ActorComponent::DELIM);

				}
				//if we could parse out an id, keep going
					//tokenize the rest by the component delimiter
					//deserialize an Actor with the tokenized string
				//Syntax is wrong, either malformed or not actor line, move along
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
	}

	return true;
}

void ActorList::SaveActorsToFile()
{}

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