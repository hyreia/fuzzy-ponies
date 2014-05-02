#ifndef CONSOLE_TILE_HPP
#define CONSOLE_TILE_HPP

struct ConsoleTile
{
	ConsoleTile();
	~ConsoleTile();

	bool redraw;
	int ch;
	Color backgroundColor;
	Color foregroundColor;
	ALLEGRO_BITMAP *texture;
};


#endif