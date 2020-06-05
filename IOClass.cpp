#include "IOClass.h"


void IOClass::printFactories(Factory** factories, int factoriesSize, std::vector<Factory*> tables, int tablesCount) {
	//print all factories given
	std::cout << "Factories: " << std::endl << std::endl;
	for (int i = 0; i < tablesCount; i++) {
		std::cout << std::to_string(i) << ": ";
		tables.at(i)->toString();
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int i = 0; i < factoriesSize; i++) {
		if (factories != nullptr) {
			std::cout << i + tablesCount << ": ";
			factories[i]->toString();
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//prints all player walls, with the leftmost being the current player's
//all lines are pushed to 17 wide, and then given whitespace left and right
//as well as a line between them
void IOClass::printPlayerWall(std::vector<Player*> players, int currentPlayer) {
	std::cout << players.at(currentPlayer)->getName() << ", your turn!" << std::endl << std::endl;

	int playerCount = (int)players.size();
	std::vector<int> nameLengths;
	nameLengths.resize(playerCount);

	for (int i = 0; i < playerCount; i++) {
		std::string playerName = players.at((currentPlayer + i) % playerCount)->getName();
		nameLengths.at(i) = playerName.length();

		std::cout << "Mosaic for " << playerName << ":";
		if (i < playerCount - 1) {
			if (nameLengths.at(i) < 6) {
				for (int j = 0; j < nameLengths.at(i); j++) {
					std::cout << " ";
				}
				std::cout << "      |    ";
			}
			else {
				std::cout << "   |    ";
			}
			
		}
		else {
			std::cout << std::endl;
		}
	}
	for (int i = 0; i < playerCount - 1; i++) {
		int extraSpaces = nameLengths.at(i);
		if (extraSpaces > 5) {
			for (int j = 0; j < extraSpaces - 5; j++) {
				std::cout << " ";
			}
		}
		std::cout << "                    |";
	}
	std::cout << std::endl;
	for (int i = 0; i < WALL_DIM + 1; i++) {
		for (int j = 0; j < playerCount; j++) {
			std::string wallString = players.at((currentPlayer + j) % playerCount)->getWall()->getPlayerWallString(i);

			std::cout << wallString;

			int extraSpaces = nameLengths.at(j);
			if (extraSpaces > 5) {
				for (int k = 0; k < extraSpaces - 5; k++) {
					std::cout << " ";
				}
			}

			if (j < playerCount - 1 && i != WALL_DIM) {
				std::cout << "   |    ";
			}
			else if (j < playerCount - 1 && i == WALL_DIM) {
				std::cout << "     |    ";
			}
			else {
				std::cout << std::endl;
			}
		}
	}
}

void IOClass::printPlayerWall(Player* player) {
	std::string playerName = player->getName();
	std::string wallString = player->getWall()->getPlayerWallString();

	std::cout << "Mosaic for " << playerName << ":" << std::endl << std::endl;
	std::cout << wallString << std::endl;
	std::cout << std::endl;
}

//menu
void IOClass::printMenu() {
	//just print these lines as the main menu
	std::cout << "Welcome to Azul!" << std::endl;
	std::cout << "-------------------" << std::endl;
	std::cout << "Menu" << std::endl;
	std::cout << "----" << std::endl;
	std::cout << "1. New Game" << std::endl;
	std::cout << "2. Load Game" << std::endl;
	std::cout << "3. Credits (Show student information)" << std::endl;
	std::cout << "4. Quit" << std::endl;
}

//credits
void IOClass::printCredits() {
	// HANDLE hConsole;

	// hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Name:        Jordan Tori" << std::endl;
	std::cout << "Student ID:  s3542275" << std::endl;
	std::cout << "Email:       s3542275@student.rmit.edu.au" << std::endl;
	std::cout << "Name:        Merhawi Minassi" << std::endl;
	std::cout << "Student ID:  s3668396" << std::endl;
	std::cout << "Email:       s3668396@student.rmit.edu.au" << std::endl;
	std::cout << "Name:        Guy Witherow" << std::endl;
	std::cout << "Student ID:  s3783428" << std::endl;
	std::cout << "Email:       s3783428@student.rmit.edu.au" << std::endl;
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Individual Extension by:" << std::endl;
	std::cout << "Name:        Guy Witherow" << std::endl;
	std::cout << "Student ID:  s3783428" << std::endl;
	std::cout << "Email:       s3783428@student.rmit.edu.au" << std::endl;
	std::cout << "----------------------------------" << std::endl;
	// SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

//reference board, printed before each turn
void IOClass::printReferenceBoard() {
	std::cout << "This is the pattern each board follows!" << std::endl;
	//maybe add command to bring this up?
	char tiles[WALL_DIM] = { 'B', 'Y', 'R', 'U', 'L' };
	for (int i = 0; i < WALL_DIM; i++) {
		for (int j = 0; j < WALL_DIM; j++) {
			int tileNumber = (j + i) % 5;
			std::cout << tiles[tileNumber] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;
	std::cout << "Please make moves using the format 'turn <factory number> <tile color letter> <storage row>" << std::endl << std::endl;
}


//take the user's input
std::vector<std::string> IOClass::takeUserInput() {
	//user input is validated in the turn loop
	std::string input = "";
	std::vector<std::string> containerToReturn;

	//user input space is indicated by an arrow
	std::cout << std::endl << "> ";
	std::getline(std::cin, input);
	std::cout << std::endl;
	//if we got an EOF, kill
	if (!std::cin) {
		containerToReturn.push_back("quit");
	}
	else {
		//break string into arguments
		std::istringstream stream{ input };
		using StrIt = std::istream_iterator<std::string>;
		std::vector<std::string> container{ StrIt{stream}, StrIt{} };

		if (container.size() == 0) {
			container.push_back(" ");
		}
		containerToReturn = container;
	}
	return containerToReturn;
}
