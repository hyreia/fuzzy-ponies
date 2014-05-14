#ifndef ACTOR_LIST_HPP
#define ACTOR_LIST_HPP

class ActorList
{
public:
	ActorList();
	~ActorList();
	bool LoadActorsFromFile();
	void SaveActorsToFile();

	void CreateSomeActorsRandomly(RegionalMap *map, int mapWidth, int mapHeight);

	int InsertActor(Actor *actor);
	Actor *GetActor(int id);
	void RemoveActor(int id);

	bool MoveActorToActiveList(int id);
	void MoveActorToInactiveList(int id);

	void MakeActorsInRegionActive(const RegionalMap &region);
	void MakeActorsInRegionInactive(const RegionalMap &region);

	void FreeActorList();


	std::priority_queue<Actor*, std::vector<Actor*>, ActorComparator> GetActors();
private:

	int nextAvailableId;
	std::map<int, Actor*> actors;
	/* Actors on a regional map not in memory */
	std::map<int, Actor*> inactiveActors;

	void InsertActor(int id, Actor *actor);

	ActorList &operator=(const ActorList&);
	ActorList(const ActorList&);
};



#endif
