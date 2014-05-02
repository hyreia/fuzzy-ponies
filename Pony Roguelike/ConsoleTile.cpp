#include "main.hpp"

ConsoleTile::ConsoleTile(): redraw(true), ch(' '),
texture(NULL), backgroundColor(Color::Black), foregroundColor(Color::Black)	
{}
ConsoleTile::~ConsoleTile(){}