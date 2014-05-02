#ifndef ACTOR_HPP
#define ACTOR_HPP

class Ai;

class Actor
{
public:

	//Location in the map
	int x, y;

	//For drawing purposes
	int image; //TODO: turn this into an index
	int ascii;

	int turnDelay;		//when delay is 0, actor ai will be called
	int ActorTemplateIdUsedOnCreation;

	Color color;
	

	bool isDrawn, isBlocking;	
	
	//Not always used, species is found from template
	std::string name;
	
	void ResetDelay();

	//For drawing and ordering priority
	friend bool operator< (const Actor &one, const Actor& two);
	
	/* Serialization order: 
	/* X, Y, Ascii, Image, ColorR, ColorG, ColorB, Template, Delay, isDrawn, isBlocking, (Name)*/
	enum ACTOR_SERIALIZATION
	{
		SERIAL_ACTOR,
		SERIAL_X,
		SERIAL_Y,
		SERIAL_ASCII,
		SERIAL_IMAGE,
		SERIAL_COLOR_R,
		SERIAL_COLOR_G,
		SERIAL_COLOR_B,
		SERIAL_COLOR_A,
		SERIAL_TEMPLATE,
		SERIAL_DELAY,
		SERIAL_ISDRAWN,
		SERIAL_ISBLOCKING,
		SERIAL_NAME,
		SERIALIZATION_MINIMUM_SIZE=SERIAL_NAME,
	};

	void Deserialize(std::vector<std::string> tokens);
	std::string Serialize();

	Actor(int x, int y, int ch, Color color, int image, int delay=900, bool isBlocking=true, bool isDrawn=true);
	~Actor();

	//Components available
	Ai *ai;
};

#endif