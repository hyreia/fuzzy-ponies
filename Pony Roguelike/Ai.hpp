#ifndef AI_HPP
#define AI_HPP

#include "ActorComponents.hpp"

class Ai: public ActorComponent
{
public:
	virtual void Act(Actor *owner) = 0;
	virtual ~Ai(){}
protected:
	Ai(const char *componentSecondaryType);
	const char* componentSecondaryType;
};

class PlayerControlledAi: public Ai
{
public:
	static const char *type;
	PlayerControlledAi(): Ai(type)
	{}
	~PlayerControlledAi(){}
	void Act(Actor *owner){}
};

#endif