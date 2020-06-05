#include "PlayerWall.h"

//set up empty buffer and wall and floor
PlayerWall::PlayerWall() {
	//wall needs to have all tiles init, otherwise we will get a
	//nullptr
	for (int i = 0; i < WALL_DIM; i++) {
		for (int j = 0; j < WALL_DIM; j++) {
			wall[i][j] = Tile();
		}
	}

	for (int i = 0; i < WALL_DIM; i++) {
		int size = i + 1;
		storage[i].resize(size, Tile());
	}

	//so we can check it's size against the expected
	//floor does need to be init
	for (int i = 0; i < FLOOR_DIM; i++) {
		floor[i] = Tile();
	}

	tilesOnFloor = 0;
}

PlayerWall::~PlayerWall() {
}

//get a line of the storage
std::vector<Tile> PlayerWall::getStorageLine(int line) {
	return storage[line];
}

//get a buffer line
void PlayerWall::setBufferLine(std::vector<Tile> added, int line) {
	for (int i = 0; i < line + 1; i++) {
		storage[line][i] = added.at(i);
	}
}

//set a wall line
void PlayerWall::setWallLine(std::vector<Tile> added, int line) {
	for (int i = 0; i < WALL_DIM; i++) {
		wall[line][i] = added.at(i);
	}
}

//checks a line of the wall for a certain type, and returns
//true if it finds any, false if it doesnt
bool PlayerWall::checkWallLineForType(int line, TileType type) {
	bool found = false;
	for (int i = 0; i < WALL_DIM; i++) {
		if (wall[line][i].getType() == type) {
			found = true;
		}
	}
	return found;
}

//get a line of the wall by direction
Tile* PlayerWall::getWallLine(int line, Direction direction) {
	static Tile returnLine[WALL_DIM];

	if (direction == Direction::VERTICAL) {
		for (int i = 0; i < WALL_DIM; i++) {
			returnLine[i] = wall[i][line];
		}
	}
	else {
		for (int i = 0; i < WALL_DIM; i++) {
			returnLine[i] = wall[line][i];
		}
	}

	return returnLine;
}

//get the floor line, for scoring and for changing
Tile* PlayerWall::getFloorLine() {
	static Tile returnLine[FLOOR_DIM];
	for (int i = 0; i < FLOOR_DIM; i++) {
		returnLine[i] = floor[i];
	}

	return returnLine;
}

//return a string representation for printing of the buffer, wall and floor
//as shown in the documentation
std::string PlayerWall::getPlayerWallString(int i) {
	std::string returnString = "";
	if (i < WALL_DIM) {
		returnString += std::to_string(i);
		returnString += ":";
		//get data
		std::vector<Tile> bufferLine = getStorageLine(i);
		Tile* wallLine = getWallLine(i, Direction::HORIZONTAL);

		//construct string
		for (int j = i; j < 5; j++) {
			returnString += " ";
		}

		for (std::size_t i = 0; i < bufferLine.size(); ++i) {
			returnString += bufferLine.at(i).tileToChar();
		}
		returnString += " || ";

		std::string wallLineString = "";
		for (int j = 0; j < WALL_DIM; j++) {
			returnString += wallLine[j].tileToChar();
		}

		returnString += wallLineString;
	}
	else {
		std::string floor = "";

		Tile* floorTiles = getFloorLine();

		returnString += "broken: ";
		for (int j = 0; j < 7; j++) {
			returnString += floorTiles[j].tileToChar();
		}
	}
	return returnString;
}

std::string PlayerWall::getPlayerWallString() {
	std::string returnString = "";
	for (int i = 0; i < WALL_DIM; i++) {
		int lineNumber = i + 1;
		returnString += std::to_string(lineNumber);
		returnString += ":";
		//get data
		std::vector<Tile> bufferLine = getStorageLine(i);
		Tile* wallLine = getWallLine(i, Direction::HORIZONTAL);

		//construct string
		for (int j = i; j < 5; j++) {
			returnString += " ";
		}

		for (std::size_t i = 0; i < bufferLine.size(); ++i) {
			returnString += bufferLine.at(i).tileToChar();
		}
		returnString += " || ";

		std::string wallLineString = "";
		for (int j = 0; j < WALL_DIM; j++) {
			returnString += wallLine[j].tileToChar();
		}

		returnString += wallLineString;
		returnString += '\n';
	}

	std::string floor = "";

	Tile* floorTiles = getFloorLine();

	returnString += "broken: ";
	for (int j = 0; j < 7; j++) {
		returnString += floorTiles[j].tileToChar();
	}
	returnString += '\n';
	return returnString;
}

//add tiles to storage line, any excess go to the floor
void PlayerWall::addToStorageLine(TileType type, int count, int line) {
	int currentTilesOnBuffer = 0;
	TileType typeOnLine = TileType::NO_TILE;
	for (int j = 0; j < line + 1; j++) {
		if (storage[line].at(j).getType() != TileType::NO_TILE) {
			currentTilesOnBuffer++;
			typeOnLine = storage[line].at(j).getType();
		}
	}
	if (type == typeOnLine || typeOnLine == TileType::NO_TILE) {
		for (int i = 0; i < count; i++) {
			if (currentTilesOnBuffer > line) {
				addToFloorLine(type, 1);
			}
			else {
				storage[line][currentTilesOnBuffer++] = Tile(type);
			}
		}
	}
	else {
		for (int i = 0; i < count; i++) {
			addToFloorLine(type, 1);
		}
	}


}


//adds tiles of a type to the floor
void PlayerWall::addToFloorLine(TileType type, int count) {
	for (int i = 0; i < count; i++) {
		if (tilesOnFloor < 7) {
			floor[tilesOnFloor++] = Tile(type);
		}
		if (type == TileType::NO_TILE) {
			tilesOnFloor--;
		}
		//if there are 7, discard
	}
}

//will be complex, follow steps on the flowchart for scoring
//vertical lines
//horizontal lines
//adjacent tiles
//if a complete horizontal line is reached, we need to flag game over
//(BASICLY A SCORE FUNCTION)
int PlayerWall::moveStorageToWall(Bag* lid) {
	int score = 0;

	//for each buffer
	for (int i = 0; i < WALL_DIM; i++) {
		//get the tiles in that buffer
		std::vector<Tile> currentBuffer = storage[i];
		bool bufferFull = false;
		if (currentBuffer.at(i).getType() != TileType::NO_TILE) {
			bufferFull = true;
		}


		//full line in buffer?
		//move them
		if (bufferFull) {

			//start at 0 score
			int scoreToAdd = 0;
			TileType addToWall = currentBuffer.at(0).getType();

			//check where the tile is going
			char tiles[WALL_DIM] = { 'B','L', 'U','R','Y' };

			//get the position dependant on the row
			for (int j = 0; j < WALL_DIM; j++) {
				int absoluteTileNumber = i - j;
				if (absoluteTileNumber < 0) {
					absoluteTileNumber = 5 + absoluteTileNumber;
				}

				//if we have the right tile type for the position,
				//add it and calculate score
				if (addToWall == Tile::charToTile(tiles[absoluteTileNumber])) {
					wall[i][j] = Tile(addToWall);
					scoreToAdd = checkScore(i, j);
				}
			}
			//add score for this buffer to the total score
			score += scoreToAdd;
			//add the tiles from the buffer to the bag 
			//minus one that goes on the board
			for (int j = 0; j < i; j++) {
				lid->addTile(addToWall);
			}
			storage[i].clear();
			resetStorageLine(i);
		}
	}

	//if we have any on the floor, clear them and
	//minus the correct amount of points
	int floorTotal = 0;
	for (int i = 0; i < FLOOR_DIM; i++) {
		if (floor[i].getType() != TileType::NO_TILE) {
			floorTotal++;
			floor[i] = Tile();
			lid->addTile(floor[i].getType());
			//lose the correct amount for each tile
			if (floorTotal > 0) {
				score -= (floorTotal / 3) + 1;
			}
		}
	}
	
	return score;
}

//check if a player is done
bool PlayerWall::checkIfDone() {
	//assume not done
	bool done = false;

	//loop through the board, look for one full row
	for (int i = 0; i < WALL_DIM; i++) {
		for (int j = 0; j < WALL_DIM; j++) {
			done = true;
			//if we ever encounter an empty spot, this row is not
			//finished, skip
			if (wall[i][j].getType() == TileType::NO_TILE) {
				j = 5;
				done = false;
			}
		}
		//if the row is full, we return the done value as true
		//break the loop
		if (done == true) {
			i = WALL_DIM;
		}
	}
	return done;
}

//reset the wall, uses a std::vector fill command
void PlayerWall::resetStorageLine(int line) {
	storage[line].resize(line + 1, Tile());
}

//check a player's score from placeing a certain tile
int PlayerWall::checkScore(int y, int x) {

	std::vector<Tile> tileLine;
	int loopNum = 0;
	int score = 1;

	//first we check the row, the the collum, but its the same process
	while (loopNum < 2) {
		//first loop, check the row
		if (loopNum == 0) {
			for (int i = 0; i < WALL_DIM; i++) {
				tileLine.push_back(wall[y][i]);
			}
		}
		//second, check the col
		else {
			for (int i = 0; i < WALL_DIM; i++) {
				tileLine.push_back(wall[i][x]);
			}
		}

		//check how many in a row there are
		int streak = 0;
		bool seenTarget = false;
		for (int i = 0; i < WALL_DIM; i++) {

			//if there is a tile that isnt a break, add to the streak
			if (tileLine.at(i).getType() != TileType::NO_TILE) {
				streak++;
				//if we see the target type (there is only one per row / col)
				//mark this streak as the one we are using
				if (tileLine.at(i).getType() == wall[y][x].getType()) {
					seenTarget = true;
				}
			}
			//otherwise, check if we have found the type we are looking for,
			//and either break, or reset the streak
			else {
				if (seenTarget) {
					i = WALL_DIM;
				}
				else {
					streak = 0;
				}
			}
		}
		//if we just saw the tile, dont give the player a streak
		if (streak == 1) {
			streak = 0;
		}

		//add the streak to the score
		score += streak;
		loopNum++;
		tileLine.clear();
	}

	return score;
}

int PlayerWall::endOfGameScore() {
	int finalScoreAdd = 0;

	//very similar to the above scoring,
	//but checking for a streak of 5 tiles
	//and it doesnt matter if there is a target in there

	//check horz

	for (int i = 0; i < WALL_DIM; i++) {
		bool lineCheck = true;
		for (int j = 0; j < WALL_DIM; j++) {
			if (wall[i][j].getType() == TileType::NO_TILE) {
				lineCheck = false;
			}
		}
		//if we got to 5 in a row, add the score bonus
		if (lineCheck) {
			finalScoreAdd += SCORE_BONUS_HORZ;
		}
	}

	//check vert

	for (int i = 0; i < WALL_DIM; i++) {
		bool lineCheck = true;
		for (int j = 0; j < WALL_DIM; j++) {
			if (wall[j][i].getType() == TileType::NO_TILE) {
				lineCheck = false;
			}
		}
		if (lineCheck) {
			finalScoreAdd += SCORE_BONUS_VERT;
		}
	}

	//check color

	//check in horizontal patterns,
	//seeing if we ever get a streak of 5
	//v similar to above, just in a cool new direction
	int starterX = 0;
	for (int i = 0; i < WALL_DIM; i++) {
		bool colorCheck = true;
		for (int j = 0; j < WALL_DIM; j++) {
			int x = (j + starterX) % 5;
			if (wall[j][x].getType() == TileType::NO_TILE) {
				colorCheck = false;
			}
		}
		if (colorCheck) {
			finalScoreAdd += SCORE_BONUS_COLOR;
		}
		starterX++;
	}


	return finalScoreAdd;
}