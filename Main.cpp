#include "Game.h"
#include <iostream>

int main(int argc, char **argv) {
	Game *game = new Game();
	game->execute();
	delete game;
	return 0;
}
