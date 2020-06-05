#include "GameEngine.h"


GameEngine::GameEngine() {
}

GameEngine::GameEngine(int playerCount, int tableCount) {
	players.resize(playerCount, nullptr);
	if (playerCount == 2) {
		factoryCount = NUM_FACTORIES_2;
	}
	else if (playerCount == 3) {
		factoryCount = NUM_FACTORIES_3;
	}
	else {
		factoryCount = NUM_FACTORIES_4;
	}
	for (int i = 0; i < factoryCount; i++) {
		factories[i] = new Factory();
	}
	
	tables.resize(tableCount, nullptr);
	for (int i = 0; i < tableCount; i++) {
		tables.at(i) = new Factory();
	}

	this->tableCount = tableCount;

	bag = new Bag();
	bag->fill();
	lid = new Bag();
}

GameEngine::~GameEngine() {
	delete bag;
	for (unsigned i = 0; i < players.size(); i++) {
		delete players[i];
	}
	delete tables.at(0);
	for (int i = 0; i < factoryCount; i++) {
		delete factories[i];
	}
	delete lid;
}

bool GameEngine::takePlayerTurn(int playerID)
{
	//before a player's turn, update their board
	IOClass::printFactories(factories, factoryCount, tables, tableCount);
	IOClass::printPlayerWall(players, playerID);
	bool valid = false;
	bool quitStatus = false;

	//if we still dont have a valid input, and have not quit, ask for another input
	while (!valid && quitStatus == false) {
		//take a turn, passing in all data so we can save if need be
		std::vector<std::string> turn = IOClass::takeUserInput();
		
		//if we actually got a valid input
		if (turn.size() > 0 && turn.at(0) != " ") {
			//check what command was entered
			std::string command = turn.at(0);
			//change it to lowercase
			std::transform(command.begin(), command.end(), command.begin(),
				[](unsigned char c) { return std::tolower(c); });

			//if we got a help command, explain a normal move
			if (command == "help" || command == "h") {
				std::cout << "This is where you take your turn. Use the command 'turn', ";
				std::cout << "along with your selections to take your turn!" << std::endl;
				std::cout << "eg: '> turn 3 L 3' moves all light blue tiles, from ";
				std::cout << "factory 3, and puts them in storage line 3" << std::endl << std::endl;
			}
			//otherwise, check if it's a valid turn and try and take that move
			else if (command == "turn" || command == "t") {
				if (turn.size() != 5 && turn.size() != 4) {
					std::cout << "Please use the command format, 'turn <factory> <color_code> <buffer> [table_code]" << std::endl << std::endl;
				}
				else {
					//check turn input is valid
					int factory = -1;
					int bufferLine = -1;
					int table = -1;

					//factory int interp
					try {
						factory = std::stoi(turn.at(1)) - tableCount;
					}
					//catch oob or invalid
					catch (...) {
						std::cout << "Please input a valid factory ID, between 0 and ";
						std::cout << std::to_string(factoryCount + tableCount - 1) << ".";
						std::cout << std::endl << std::endl;
					}
					//bufferline in interp
					try {
						bufferLine = std::stoi(turn.at(3)) - 1;
					}
					//same as above
					catch (...) {
						std::cout << "Please input a valid buffer ID, between 1 and 5." << std::endl << std::endl;
					}

					if (turn.size() == 5) {
						//factory in interp
						try {
							table = std::stoi(turn.at(4));
						}
						//same as above
						catch (...) {
							std::cout << "Please input a valid table ID, between 0 and " << std::to_string(tableCount - 1);
							std::cout << "." << std::endl << std::endl;
						}
					}
					
					std::string tileType = turn.at(2);
					std::transform(tileType.begin(), tileType.end(), tileType.begin(),
						[](unsigned char c) { return std::toupper(c); });

					//change tile string to TileType
					TileType userTileType = Tile::stringToType(tileType);

					std::string errorCheck = "";
					Factory* currentFactory = nullptr;
					if (factory >= 0) {
						currentFactory = factories[factory];
					}
					else {
						currentFactory = tables.at(factory + tableCount);
					}
					Player* currentPlayer = players.at(playerID);

					//check if the player already has a tile of that type on their board
					if (currentPlayer->getWall()->checkWallLineForType(bufferLine, userTileType) == true) {
						errorCheck += "\nError. This line already has a tile of that type.";
					}

					if (table == -1 && factory >= 0 && tableCount > 1) {
						errorCheck += "\nError. You have to select a table to drop excess on, ";
						errorCheck += "unless you are taking from a table.";
					}
					else if (tableCount == 1) {
						table = 0;
					}

					//if no errors came up
					if (errorCheck == "") {
						//take turn
						int tiles = -1;

						//take tiles from table
						if (factory == -1) {
							tiles = tables.at(table)->removeTile(userTileType);
							if (tables.at(table)->removeTile(TileType::FIRST_PLAYER) != 0 && tiles != 0) {
								currentPlayer->getWall()->addToFloorLine(TileType::FIRST_PLAYER, 1);
								tables.at((table + 1) % 2)->removeTile(TileType::FIRST_PLAYER);
							}
							else {
								tables.at(table)->addTile(Tile(TileType::FIRST_PLAYER));
							}
						}
						//take tiles from factory
						else {
							tiles = currentFactory->removeTile(userTileType);
						}

						//if we actually got some tiles out of that
						if (tiles > 0) {
							//move them onto the player's board in accordance with the rules
							valid = true;
							currentPlayer->getWall()->addToStorageLine(userTileType, tiles, bufferLine);
							std::vector<Tile> moveToTable;
							//any tiles left over in a normal factory get moved to the table
							if (factory != -1) {
								moveToTable = currentFactory->getTiles();
								currentFactory->clearFactory();
								//refill the factory with empty tiles (for display purposes)
								//and also due to contract that a factory always has 4 tiles in it
								for (int i = 0; i < TILES_PER_FAC; i++) {
									currentFactory->addTile(Tile());
								}
							}

							//if we do have tiles to move to the table
							if (moveToTable.size() > 0) {
								//move em
								for (auto i = moveToTable.begin(); i != moveToTable.end(); i++) {
									if (i->getType() != TileType::NO_TILE) {
										tables.at(table)->addTile(Tile(i->getType()));
									}
								}
								tables.at(table)->sortFactory();
							}
						}
						else {
							//the player has chosen to take a type of tile that is not in that factory
							if (Tile::typeToString(userTileType) == "First Player Tile") {
								std::cout << "You cannot take the first player tile." << std::endl;
								std::cout << "It is given to the player who first takes from the table"
									<< std::endl << std::endl;
							}
							else {
								if (factory < tableCount - 1) {
									std::cout << "The table has no tiles of type ";
								}
								else {
									std::cout << "Factory number " + std::to_string(factory + 1) + " has no tiles of type ";
								}
								std::cout << Tile::typeToString(userTileType) << ". Please make a valid selection.";
							}
						}
					}
					else {
						//some blah about valid tile types, as well as buffers and factories
						std::cout << errorCheck << std::endl;
					}
				}
			}
			else if (command == "quit" || command == "q") {
				//user input the command to quit
				quitStatus = true;
			}
			else if (command == "save" || command == "s") {
				//save the game
				//handle saving a game, use that game data to save
				if (turn.size() == 2) {
					saveGame(turn.at(1));
					std::cout << "Saved game." << std::endl << "Resuming" << std::endl;
				}
				else {
					std::cout << "Error. When using the save command, please specify a save file name" << std::endl;
				}
			}
			else {
				std::cout << "Please input a valid command." << std::endl << std::endl;
			}
		}
		else if (!std::cin) {
			//if we got an EOF, quit
			quitStatus = true;
		}
		//if we didnt get a valid command, print the command prompt
		else {
			std::cout << "Please input a command." << std::endl;
			std::cout << "======Valid Commands======" << std::endl << std::endl;
			std::cout << "save <save name> - Save the Game." << std::endl;
			std::cout << "turn <factory> <type> <buffer line> - Take a turn, moves items from <factory> of type <type> to <buffer line>." << std::endl;
			std::cout << "save <save name> - Save the Game." << std::endl;
			std::cout << "help - Prints some information about how the turn command works." << std::endl;
			std::cout << "quit - Quits the Game." << std::endl << std::endl;
		}
	}

	return quitStatus;
}

bool GameEngine::doGameLoop(int seed) {
	//basic game loop
	int round = 0;
	bool roundLoop = true;
	bool quit = false;
	//while the round has not ended and we have not quit
	while (roundLoop && !quit) {
		//ensure clear factories, then fill
		//do not do this on the first load of a saved game
		if (isLoaded != true) {
			for (int i = 0; i < factoryCount; i++) {
				factories[i]->clearFactory();
				for (int j = 0; j < TILES_PER_FAC; j++) {
					factories[i]->addTile(bag->getTopTile());
				}
				factories[i]->sortFactory();
			}

			for (int i = 0; i < tableCount; i++) {
				tables.at(i)->clearFactory();
				tables.at(i)->addTile(Tile(TileType::FIRST_PLAYER));
			}

		}
		else {
			isLoaded = false;
		}

		bool tileLoop = true;
		turn = 0;

		//loop of adding tiles to buffers
		while (tileLoop && !quit) {
			//take the player's turn
			//this can return a true value to mean that we quit
			quit = takePlayerTurn(turn % players.size());

			//if we didnt quit
			if (!quit) {
				//check if any tiles are left
				int factoryTileCount = 0;
				int tableTileCount = 0;
				for (int i = 0; i < tableCount; i++) {
					tableTileCount = tables.at(i)->getTileCount();
					if (tableTileCount != 0) {
						i = tableCount;
					}
				}

				
				for (int i = 0; i < factoryCount; i++) {
					//check all factories
					factoryTileCount = factories[i]->getTileCount();
					if (factoryTileCount != 0) {
						i = factoryCount;
					}
				}
				//if any factory has tiles left, let the players go again.
				if (tableTileCount > 0 || factoryTileCount > 0) {
					tileLoop = true;
				}
				else {
					tileLoop = false;
				}
				turn++;
			}
		}
		//if all the tiles off and did not quit
		if (!quit) {
			//move all the players tiles to their wall
			
			std::vector<int> scores;

			for (unsigned i = 0; i < players.size(); i++) {
				scores.push_back(players.at(i)->getWall()->moveStorageToWall(lid));
			}
			
			for (unsigned i = 0; i < players.size(); i++) {
				players.at(i)->setScore(players.at(i)->getScore() + scores.at(i));
			}

			std::cout << "ROUND END" << std::endl << std::endl;
			if (lid->size() > 0 && lid != nullptr) {
				bag->fillFromBag(lid);
			}
			
			if (bag->size() > 0) {
				bag->shuffle(seed);
			}
			

			//print their

			for (unsigned i = 0; i < players.size(); i++) {
				IOClass::printPlayerWall(players.at(i));
				std::cout << "Player " << std::to_string(i+1) <<" Score: " << players.at(i)->getScore() << std::endl << std::endl;
			}

			bool isDone = false;
			bool multipleDone = false;
			int playerDone = -1;
			//move tiles for both players to the board
			//check if either player has a full board line
			for (unsigned i = 0; i < players.size(); i++) {
				isDone = players.at(i)->getWall()->checkIfDone();
				if (isDone) {
					if (playerDone == -1) {
						playerDone = i;
					}
					else {
						multipleDone = true;
						i = players.size();
					}
				}
			}

			//check which win message to send
			if (playerDone != -1 || multipleDone) {
				roundLoop = false;
				if (multipleDone) {
					std::cout << "GAME OVER, BOTH PLAYERS GOT 5 IN A ROW" << std::endl;
				}
				else {
					std::cout << "GAME OVER, " << players.at(playerDone)->getName() << " HAS 5 IN A ROW" << std::endl;
				}
				doGameEnd();
				return true;
			}
			//otherwise, round we go again
			else {
				std::cout << "NEXT ROUND" << std::endl;
			}
			round++;
		}
	}
	return false;
}

void GameEngine::doGameEnd() {
	//clear the screen
	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;

	//get the final scores
	int player1Final = players.at(0)->getWall()->endOfGameScore();
	int player2Final = players.at(1)->getWall()->endOfGameScore();
	players.at(0)->setScore(players.at(0)->getScore() + player1Final);
	players.at(1)->setScore(players.at(1)->getScore() + player2Final);

	//print the final screen
	std::cout << "Final Boards:" << std::endl;
	IOClass::printPlayerWall(players.at(0));
	std::cout << "Player 1 Score: " << players.at(0)->getScore() << std::endl << std::endl;
	IOClass::printPlayerWall(players.at(1));
	std::cout << "Player 2 Score: " << players.at(1)->getScore() << std::endl << std::endl;

	//print out who won and quit
	if (players.at(0)->getScore() == players.at(1)->getScore()) {
		std::cout << "It's a draw!" << std::endl << std::endl;
	}
	else if (players.at(0)->getScore() > players.at(1)->getScore()) {
		std::cout << players.at(0)->getName() << " Wins!" << std::endl << std::endl;
	}
	else {
		std::cout << players.at(1)->getName() << " Wins!" << std::endl << std::endl;
	}
	std::cout << std::endl << std::endl << "Game Over." << std::endl << std::endl;
}

void GameEngine::reset() {
	players.resize(2, nullptr);

	for (int i = 0; i < 2; i++) {
		players.at(i) = new Player("Bad Save Greg");
	}

	factoryCount = NUM_FACTORIES_2;

	for (int i = 0; i < factoryCount; i++) {
		factories[i] = new Factory();
	}

	tables.resize(1, nullptr);

	tables.at(0) = new Factory();

	bag = new Bag();
	bag->fill();
	lid = new Bag();
}

Bag* GameEngine::getBag()
{
	return bag;
}

Bag* GameEngine::getLid()
{
	return lid;
}

Factory* GameEngine::getFactory(int factoryNumber)
{
	return factories[factoryNumber];
}

Factory* GameEngine::getTable()
{
	return tables.at(0);
}

Factory* GameEngine::getTable(int tableNumber)
{
	return tables.at(tableNumber);
}

Player* GameEngine::getPlayer(int playerNumber)
{
	return players.at(playerNumber);
}


//tell the gameengine wether we loaded a game
void GameEngine::setLoaded(bool loaded) {
	isLoaded = loaded;
}

void GameEngine::setPlayer(int player, std::string playerName) {
	players.at(player) = new Player(playerName);
}

int GameEngine::loadGame() {
	//get user input for which file to load
	std::string input;
	std::cout << "Enter the name of the file which you want to load."
		<< std::endl << "> ";
	std::cin >> input;

	int turn = -1;

	lid = new Bag();
	bag = new Bag();

	//open specified file
	std::string line;
	std::ifstream loadFile(input);
	int currentLine = 0;
	int numPlayers = 0;
	int numFactories = 0;
	int numTables = 0;
	if (loadFile.is_open()) {

		//we preload the sizes so we can decide how many players
		//factories and tables to load
		//load players
		getline(loadFile, line);

		try {
			numPlayers = std::stoi(line);
		}
		catch (...) {
			numPlayers = 2;
		}
		players.resize(numPlayers, nullptr);

		for (int i = 0; i < numPlayers; i++) {
			players.at(i) = new Player("");
		}

		if (numPlayers == 2) {
			numFactories = NUM_FACTORIES_2;
		}
		else if (numPlayers == 3) {
			numFactories = NUM_FACTORIES_3;
		}
		else {
			numFactories = NUM_FACTORIES_4;
		}

		for (int i = 0; i < numFactories; i++) {
			factories[i] = new Factory();
		}

		factoryCount = numFactories;

		//load table count
		getline(loadFile, line);

		try {
			numTables = std::stoi(line);
		}
		catch (...) {
			numTables = 1;
		}
		tables.resize(numTables, new Factory());

		//read lines, discarding # or empty lines


		while (getline(loadFile, line)) {
			std::string lineCheck = line;
			if (line[0] == '#' || line.length() == 0) {
				//do nothin
			}

			//read in the data
			else {
				int dataIndex = 0;

				if (currentLine == 0) {
					bag->load(line);
				}
				else if (currentLine == 1) {
					lid->load(line);
				}
				else if (currentLine >= 2 && currentLine < factoryCount + 2) {
					dataIndex = currentLine - 2;
					factories[dataIndex]->load(line);
				}
				else if (currentLine >= factoryCount + 2 && currentLine <
					factoryCount + 2 + numTables) {

					tables.at(currentLine - (factoryCount + 2))->load(line);
				}
				else if (currentLine >= factoryCount + 2 + numTables &&
					currentLine < factoryCount + 2 + numTables + (numPlayers * 13)) {

					dataIndex = currentLine - (numFactories + 2 + numTables);
					players.at(0)->load(line, dataIndex % 13);
				}
				else if (currentLine == factoryCount + 2 + numTables + (numPlayers * 13)) {
					try {
						turn = std::stoi(line);
					}
					catch (...) {
						std::cout << "Current Turn could not be read, assuming player 1" << std::endl;
							turn = 1;
					}
				}
				currentLine++;
			}
		}
		loadFile.close();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
		turn = -1;
	}

	return turn;
}

void GameEngine::saveGame(std::string saveName) {
	std::ofstream savefile;
	savefile.open(saveName);
	int currentLine = 0;
	int dataIndex = 0;


	//5 static data lines
	//bag, lid, player count, table count, turn

	//playercount * 13 lines of player data
	//factoryCount lines of factory data
	//tableCount lines of table data
	//oh god


	//i am in pain writing this but i dont have time to 
	//figure out a better way of determining the data line
	bool atEnd = false;
	while (!atEnd) {
		std::string data = "";
		int tableSize = (int)tables.size();
		int playerSize = (int)players.size();


		if (currentLine == 0) {
			data = std::to_string(playerSize);
		}
		else if (currentLine == 1) {
			data = std::to_string(tableSize);
		}
		else if (currentLine == 2) {
			data = bag->getAllTiles();
		}
		else if (currentLine == 3) {
			data = lid->getAllTiles();
		}
		else if (currentLine >= 4 && currentLine <= factoryCount + 3) {
			dataIndex = currentLine - 4;
			data = factories[dataIndex]->getTilesString();
		}
		else if (currentLine >= factoryCount + 4 && currentLine <= 
			factoryCount + 3 + tableSize) {

			data = tables.at(currentLine - (factoryCount + 4))->getTilesString();
		}
		else if (currentLine >= factoryCount + 4 + tableSize && currentLine <=
			factoryCount + 3 + tableSize + (playerSize * 13)) {
			//13 lines of data for a player, * playerCount
			dataIndex = currentLine - (factoryCount + 4 + tableSize);
			int playerNumber = dataIndex / 13;
			Player* playerForData = players.at(playerNumber);
			data = playerForData->getDataString(dataIndex % 13);
		}
		else if (currentLine == factoryCount + 4 + tableSize + (playerSize * 13)) {
			try {
				data = std::to_string(turn);
			}
			catch (...) {
				data = "0";
			}
			atEnd = true;
		}

		savefile << data + "\n";
		currentLine++;
	}

	savefile.close();
}
