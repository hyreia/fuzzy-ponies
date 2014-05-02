#ifndef KEY_EVENT_HPP
#define KEY_EVENT_HPP

struct KeyEvent
{
	KeyEvent(): code(0), ch(-1)
	{}
	int code; //code of physical key pressed
	int ch; //If key is printable > 0, takes in account caps, spaces, etc
};

#endif