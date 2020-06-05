#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <cstdlib>
#include <iterator>
#include "Player.h"
#include "Factory.h"
#include "Types.h"
#include "Tile.h"
#include "Bag.h"
#include "GameEngine.h"
#include <vld.h>

//used to handle the main game loop
void game(int seed, bool load);


int main(int argc, char const* argv[])
{
	//loop until we quit
	bool quit = false;
	while (!quit) {
		//print the menu, and take the user's input
		IOClass::printMenu();
		std::vector<std::string> fullinput = IOClass::takeUserInput();
		std::string input = fullinput.at(0);

		//handle input
		if (input == "1") {
			bool valid = true;
			int seed = 0;
			//new game
			if (argc == 3) {
				if (argv[1] == "-s") {
					try {
						seed = atoi(argv[2]);
					}
					catch (...) {
						seed = 0;
					}

					srand(seed);

				}
				else {
					std::cout << "Argument available: -s <seed>";
				}

			}
			else {
				//generate a random seed
				// srand(time(0));
				srand(1);
			}

			seed = rand();
			game(seed, false);
		}
		else if (input == "2") {
			//load game
			game(0, true);
		}
		else if (input == "3") {
			//credits
			IOClass::printCredits();
		}
		else if (input == "4") {
			//quit
			quit = true;
		}
		else {
			//bad input, try again
			std::cout << "Choice not recognised, please choose an option from the list" << std::endl;
		}

		if (!std::cin) {
			quit = true;
		}
	}

	return 0;
}

void game(int seed, bool load) {
	std::cout << "Starting a new game" << std::endl;
	int turn = 0;

	bool valid = false;
	int playerCount = 2;
	int tablesCount = 2;

	GameEngine* gameEngine = nullptr;

	if (load == false) {
		while (!valid) {
			std::cout << "Please input the number of players" << std::endl;
			std::string players = IOClass::takeUserInput().at(0);

			try {
				playerCount = std::stoi(players);
			}
			catch (...) {
				std::cout << "Error! Please input a number." << std::endl;
			}

			if (playerCount >= 2 && playerCount <= 4) {
				valid = true;
			}
			else {
				std::cout << "Oops! Only 2 - 4 players can play." << std::endl;
			}
		}

		valid = false;
		while (!valid) {
			std::cout << "Please input the number of tables" << std::endl;
			std::string tables = IOClass::takeUserInput().at(0);

			try {
				tablesCount = std::stoi(tables);
				valid = true;
			}
			catch (...) {
				std::cout << "Error! Please input a number." << std::endl;
			}
		}

		int factoryCount = 0;

		if (playerCount == 2) {
			factoryCount = NUM_FACTORIES_2;
		}
		else if (playerCount == 3) {
			factoryCount = NUM_FACTORIES_3;
		}
		else {
			factoryCount = NUM_FACTORIES_4;
		}
		gameEngine = new GameEngine(playerCount, tablesCount);
	}
	//if we are loading, open the file and load
	if (load) {
		gameEngine = new GameEngine(playerCount, tablesCount);
		turn = gameEngine->loadGame();
		if (turn == -1) {
			load = false;
			std::cout << "Load Failed. Starting a fresh game." << std::endl << std::endl;
		}
		else {
			std::cout << "Loaded" << std::endl << std::endl;
			gameEngine->setLoaded(load);
		}
	}
	//if we are not loading, start new game with seed
	else {
		//bag prep
		gameEngine->getBag()->shuffle(seed);

		//player name input

		for (int i = 0; i < playerCount; i++) {
			std::cout << std::endl << "Enter a name for player " << std::to_string(i + 1) 
				<< ":" << std::endl;
			std::string name = IOClass::takeUserInput().at(0);
			gameEngine->setPlayer(i, name);
		}

		//let the games begin
		std::cout << "Let's Play!" << std::endl << std::endl;
	}

	gameEngine->doGameLoop(seed);


	//always clean up your toys kids
	delete gameEngine;
}
