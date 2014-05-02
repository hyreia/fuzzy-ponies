#include "main.hpp"

int main(int argc, char *argv[])
{
	game.reset(new Game());
	game->Init();
	game->Run();


	return 0;
}