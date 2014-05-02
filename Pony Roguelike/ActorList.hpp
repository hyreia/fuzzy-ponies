#ifndef ACTOR_LIST_HPP
#define ACTOR_LIST_HPP

class ActorList
{
public:
	ActorList();
	~ActorList();
	bool LoadActorsFromFile();
	void SaveActorsToFile();

	int InsertActor(Actor *actor);
	Actor *GetActor(int id);
	void RemoveActor(int id);

	void FreeActorList();

private:
	int nextAvailableId;
	std::map<int, Actor*> actors;
	void InsertActor(int id, Actor *actor);

	ActorList &operator=(const ActorList&);
	ActorList(const ActorList&);
};

#endif
