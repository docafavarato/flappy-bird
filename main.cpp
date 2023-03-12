#include "game.h"
#include <iostream>

int main()
{
	std::srand(time(NULL));
	Game game;

	while (game.running())
	{
		game.update();
		game.render();
	}

	return 0;
}