#ifndef MOUSE_STATE_HPP
#define MOUSE_STATE_HPP

#include <bitset>

struct MouseState
{
	MouseState():
	x(0), y(0), z(0),
	dx(0),dy(0),dz(0)
	{}
	int x, y, z;	//absolute position in window
	int dx, dy, dz;	//distance moved since last mouse state check

	inline bool IsButtonPressed(unsigned int button)
	{
		bool pressed = buttonsPressed.test(button);
		buttonsPressed.reset(button);

		return pressed;
	}
	inline bool IsButtonDown(unsigned int button)
	{
		return buttonsDown.test(button);
	}
	inline void UpdateState(ALLEGRO_EVENT &ev)
	{
		if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
		{
			x=ev.mouse.x;
			y=ev.mouse.y;
			z=ev.mouse.z;
			dx=ev.mouse.dx;
			dy=ev.mouse.dy;
			dz=ev.mouse.dz;
		}

		if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			buttonsDown.reset(ev.mouse.button);
			buttonsPressed.reset(ev.mouse.button);
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			buttonsDown.set(ev.mouse.button);
			buttonsPressed.reset(ev.mouse.button);
		}

	}

	std::bitset< sizeof(unsigned int)*CHAR_BIT > buttonsPressed;
	std::bitset< sizeof(unsigned int)*CHAR_BIT > buttonsDown;
	
};

#endif