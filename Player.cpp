#include "Player.h"
#include <string>

Player::Player(std::string name) {
	this->playerName = name;
	this->wall = new PlayerWall();
}

Player::~Player()
{
	delete this->wall;
}

//loads the player's data using a string of data and an ID number
void Player::load(std::string data, int dataID) {
	if (dataID == 0) {
		setName(data);
	}
	else if (dataID >= 1 && dataID <= 5) {
		std::vector<Tile> tilesToAdd;
		for (int i = 0; i < dataID; ++i)
		{
			tilesToAdd.push_back(Tile::charToTile(data.at(i)));
		}

		getWall()->setBufferLine(tilesToAdd, dataID - 1);
	}
	else if (dataID >= 6 && dataID <= 10) {
		std::vector<Tile> tilesToAdd;
		for (unsigned i = 0; i < data.length(); ++i)
		{
			tilesToAdd.push_back(Tile::charToTile(data.at(i)));
		}

		getWall()->setWallLine(tilesToAdd, dataID - 6);
	}
	else if (dataID == 11) {
		std::vector<Tile> tilesToAdd;
		int dataLength = data.length();
		for (int i = 0; i < dataLength; i++) {
			TileType current = Tile::charToTile(data.at(i));
			if (current == TileType::NO_TILE) {
				i = FLOOR_DIM;
			}
			else {
				getWall()->addToFloorLine(current, 1);
			}
		}
	}
	else if (dataID == 12) {
		try {
			score = std::stoi(data);
		}
		catch (...) {
			score = 0;
		}
	}
}

//returns all the player's data as a string
//depending on what is requested
std::string Player::getDataString(int dataID) {
	std::string data = "";
	if (dataID == 0) {
		data = playerName;
	}
	else if (dataID >= 1 && dataID <= 5) {
		std::vector<Tile> tilesToAdd = getWall()->getStorageLine(dataID - 1);
		for (int i = 0; i < dataID; ++i)
		{
			data += tilesToAdd.at(i).tileToChar();
		}
	}
	else if (dataID >= 6 && dataID <= 10) {
		Tile* tilesToAdd = getWall()->getWallLine(dataID - 6, Direction::HORIZONTAL);
		for (int i = 0; i < WALL_DIM; ++i)
		{
			data += tilesToAdd[i].tileToChar();
		}
	}
	else if (dataID == 11) {
		Tile* floor = getWall()->getFloorLine();
		for (int i = 0; i < FLOOR_DIM; i++) {
			data += floor[i].tileToChar();
		}
	}
	else if (dataID == 12) {
		data += std::to_string(score);
	}

	return data;
}

std::string Player::getName() {
	return playerName;
}

void Player::setName(std::string name) {
	playerName = name;
}

PlayerWall* Player::getWall() {
	return wall;
}

int Player::getScore() {
	return this->score;
}

void Player::setScore(int score) {
	this->score = score;
}

void Player::setWall(PlayerWall* wall) {
	delete this->wall;
	this->wall = wall;
}