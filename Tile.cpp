#include "Tile.h"

Tile::Tile() {
	this->type = TileType::NO_TILE;
}

Tile::Tile(TileType type) {
	this->type = type;
}

Tile::~Tile() {
	//tile only has tiletype which is a char, automaticlly deleted
}

//prints a string of the given tile
std::string Tile::tileToString() {
	return tileToString(type);
}

//returns this tile's type as a string
std::string Tile::tileToString(TileType givenType) {
	std::string typeString;
	if (givenType == TileType::RED) {
		typeString = "Red";
	}
	else if (givenType == TileType::YELLOW) {
		typeString = "Yellow";
	}
	else if (givenType == TileType::LIGHT_BLUE) {
		typeString = "Light Blue";
	}
	else if (givenType == TileType::DARK_BLUE) {
		typeString = "Dark Blue";
	}
	else if (givenType == TileType::BLACK) {
		typeString = "Black";
	}
	else if (givenType == TileType::NO_TILE) {
		typeString = "No Tile";
	}
	else if (givenType == TileType::FIRST_PLAYER) {
		typeString = "First Player Tile";
	}
	return typeString;
}

//returns a char of the given tile
char Tile::tileToChar() {
	char typeChar = ' ';
	if (type == TileType::RED) {
		typeChar = 'R';
	}
	else if (type == TileType::YELLOW) {
		typeChar = 'Y';
	}
	else if (type == TileType::LIGHT_BLUE) {
		typeChar = 'L';
	}
	else if (type == TileType::DARK_BLUE) {
		typeChar = 'B';
	}
	else if (type == TileType::BLACK) {
		typeChar = 'U';
	}
	else if (type == TileType::NO_TILE) {
		typeChar = '.';
	}
	else if (type == TileType::FIRST_PLAYER) {
		typeChar = 'F';
	}
	return typeChar;
}

//static method for interpreting a char as a type
TileType Tile::charToTile(char givenChar) {
	TileType typeReturn = TileType::NO_TILE;
	if (givenChar == 'R') {
		typeReturn = TileType::RED;
	}
	else if (givenChar == 'Y') {
		typeReturn = TileType::YELLOW;
	}
	else if (givenChar == 'L') {
		typeReturn = TileType::LIGHT_BLUE;
	}
	else if (givenChar == 'B') {
		typeReturn = TileType::DARK_BLUE;
	}
	else if (givenChar == 'U') {
		typeReturn = TileType::BLACK;
	}
	else if (givenChar == '.') {
		typeReturn = TileType::NO_TILE;
	}
	else if (givenChar == 'F') {
		typeReturn = TileType::FIRST_PLAYER;
	}
	return typeReturn;
}

//gets the type of the current tile
TileType Tile::getType() {
	return type;
}

//takes a string of a character and gives back a matching type
TileType Tile::stringToType(std::string givenType) {
	//THIS ONLY NEEDS TO INTERP USER INPUT, NO FIRST TILE OR
	//NO_TILE NEEDED
	TileType typeTile = TileType::NO_TILE;
	if (givenType.compare("R") == 0) {
		typeTile = TileType::RED;
	}
	else if (givenType.compare("Y") == 0) {
		typeTile = TileType::YELLOW;
	}
	else if (givenType.compare("L") == 0) {
		typeTile = TileType::LIGHT_BLUE;
	}
	else if (givenType.compare("B") == 0) {
		typeTile = TileType::DARK_BLUE;
	}
	else if (givenType.compare("U") == 0) {
		typeTile = TileType::BLACK;
	}

	return typeTile;
}