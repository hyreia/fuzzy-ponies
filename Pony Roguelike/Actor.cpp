#include <vector>
#include <string>
#include <sstream>
#include "main.hpp"

Actor::Actor(int x, int y, int ascii, Color color, int texture, int delay, bool isBlocking, bool isDrawn):
x(x), 
y(y), 
ascii(ascii),
color(color),
image(texture),
turnDelay(900), 
ActorTemplateIdUsedOnCreation(0),
isDrawn(true),
isBlocking(true)
{}

Actor::~Actor()
{
	if(ai) delete ai;
}

bool operator< (const Actor& lhs, const Actor& rhs)
{
	if(!rhs.isDrawn) return false;
	if(!rhs.isBlocking) return false;

	if(lhs.image < rhs.image) return true;

	return false;
}

void Actor::ResetDelay()
{
	int delay = 900;
	//There's a lot of factors typically involved
	//Depending on the components the actor has

	turnDelay = delay;
}

/* X, Y, Ascii, Image, ColorR, ColorG, ColorB, Template, Delay, isDrawn, isBlocking, (Name)*/
bool Actor::Deserialize(std::vector<std::string> tokens)
{
	if(tokens.size() < SERIALIZATION_MINIMUM_SIZE) return false;
	if(tokens[SERIAL_ACTOR] != "Actor") return false;
	
	x = atoi(tokens		[SERIAL_X].c_str()); 
	y = atoi(tokens		[SERIAL_Y].c_str());

	ascii = tokens[SERIAL_ASCII].c_str()[0]; 
	image = atoi(tokens [SERIAL_IMAGE].c_str());
	color = Color( (unsigned char)atoi(tokens[SERIAL_COLOR_R].c_str()),
				   (unsigned char)atoi(tokens[SERIAL_COLOR_G].c_str()),
				   (unsigned char)atoi(tokens[SERIAL_COLOR_B].c_str()),
				   (unsigned char)atoi(tokens[SERIAL_COLOR_A].c_str()));
	ActorTemplateIdUsedOnCreation = atoi(tokens[SERIAL_TEMPLATE].c_str());
	turnDelay = atoi(tokens[SERIAL_DELAY].c_str());
	isDrawn = atoi(tokens		[SERIAL_ISDRAWN].c_str());
	isBlocking = atoi(tokens		[SERIAL_ISBLOCKING].c_str());
	if(tokens.size() > SERIAL_NAME)
	{
		name = tokens[SERIAL_NAME];
	}

	return true;
}

std::string Actor::Serialize()
{
	const char DELIM = ':';
	const char ActorWord[] = "Actor";
	std::ostringstream ss;
	ss << ActorWord << DELIM 
	   << x << DELIM
	   << y << DELIM
	   << (char)ascii << DELIM
	   << image << DELIM
	   << (int)color.r << DELIM
	   << (int)color.g << DELIM
	   << (int)color.b << DELIM
	   << (int)color.a << DELIM
	   << ActorTemplateIdUsedOnCreation << DELIM
	   << turnDelay << DELIM
	   << isDrawn << DELIM
	   << isBlocking;
	if(!name.empty())
	{
		ss << DELIM << name;
	}

	return ss.str();
}