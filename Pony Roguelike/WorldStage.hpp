#ifndef WORLD_STAGE_HPP
#define WORLD_STAGE_HPP

/* The playing field where the game is played out. All actors can access and query
about the world stage they're in when they act. This is where the core game data resides:
world state, maps and actors, as opposed to things relevant to how you interact
with the game*/

class WorldStage
{
public:
	WorldStage();
	~WorldStage();
	//Load data from file
	//Save data to file

};

#endif