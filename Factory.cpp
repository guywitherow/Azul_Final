#include <iostream>
#include "Factory.h"

Factory::Factory() {
	//tile vector created, no inital values, depends on the size
}

//use the clear factory command
Factory::~Factory() {
	clearFactory();
}

void Factory::toString() {
	for (auto i = tiles.begin(); i != tiles.end(); i++) {
		std::cout << i->tileToChar();
	}
}

void Factory::load(std::string data) {
	for (unsigned i = 0; i < data.length(); ++i)
	{
		addTile(Tile::charToTile(data.at(i)));
	}
}

//removes all tiles of a type, and returns the amount of that tile removed
int Factory::removeTile(TileType type) {
	int removedTileCount = 0;
	std::vector<int> toDelete;
	toDelete.resize(16);
	int currentTile = 0;
	for (auto i = tiles.begin(); i != tiles.end(); i++) {
		if (i->getType() == type) {
			toDelete[removedTileCount] = currentTile;
			removedTileCount++;
		}
		currentTile++;
	}

	for (int i = 0; i < removedTileCount; i++) {
		int index = toDelete[i] - i;
		tiles.erase(tiles.begin() + index);
	}

	if (removedTileCount > 0) {
		tileCount = 0;
	}
	return removedTileCount;
}

//adds a tile to the back of the list
void Factory::addTile(Tile tile) {
	tiles.push_back(tile);
	tileCount++;
}

void Factory::clearFactory() {
	//remove all from the vector
	tiles.clear();
}

std::vector<Tile> Factory::getTiles() {
	return tiles;
}

//how many tiles are in the factory
int Factory::getTileCount() {
	tileCount = 0;
	int tileSize = tiles.size();
	for (int i = 0; i < tileSize; i++) {
		if (tiles[i].getType() != TileType::NO_TILE) {
			tileCount++;
		}
	}

	return tileCount;
}

//get all the tiles in the factory as a string
std::string Factory::getTilesString() {
	std::string returnString = "";
	int tileSize = tiles.size();
	for (int i = 0; i < tileSize; i++) {
		returnString += tiles.at(i).tileToChar();
	}
	return returnString;
}