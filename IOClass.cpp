#include "IOClass.h"


void IOClass::printFactories(Factory** factories, int factoriesSize, std::vector<Factory*> tables) {
	//print all factories given
	std::cout << "Factories: " << std::endl << "0: ";
	tables.at(0)->toString();
	std::cout << std::endl;

	for (int i = 0; i < factoriesSize; i++) {
		if (factories != nullptr) {
			std::cout << i + 1 << ": ";
			factories[i]->toString();
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//prints a playerwall
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
