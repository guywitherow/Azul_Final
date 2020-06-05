#include "Bag.h"

Bag::Bag() {
	tempLinkedList = new LinkedList();
	bagLinkedList = new LinkedList();
}

Bag::~Bag() {
	clear();
}

void Bag::clear() {
	delete tempLinkedList;
	delete bagLinkedList;
}

void Bag::empty() {
	bagLinkedList->clear();
}

//how many tiles left?
int Bag::size() {
	return bagLinkedList->getSize();
}

//returns a string of all the tiles in the bag
std::string Bag::getAllTiles() {
	std::string returnString = "";
	if (size() == 0) {
		returnString = "_";
	}
	else {
		Tile* current = bagLinkedList->getTile(0);
		for (int i = 0; i < size(); i++) {
			current = bagLinkedList->getTile(i);
			returnString += current->tileToChar();
		}
	}
	return returnString;
}

//adds in each tile in a row from a string character
void Bag::load(std::string data) {

	//an underscore represents an empty bag
	if (data != "_") {
		for (unsigned i = 0; i < data.length(); ++i)
		{
			bagLinkedList->addBack(Tile::charToTile(data.at(i)));
		}
	}
	
}

//merhawi's shuffle function
void Bag::shuffle(int seed) {
	int min = 0;
	int max = tempLinkedList->getSize();
	int index = -1;
	std::default_random_engine generator(seed);

	for (int i = 0; i < max; ++i) {
		int size = tempLinkedList->getSize() - 1;
		std::uniform_int_distribution<int> uniform_dist(min, size);
		index = uniform_dist(generator);
		tempLinkedList->transferTo(index, bagLinkedList);
	}
}

//fill the bag with even amounts of tiles
//100 total
void Bag::fill() {
	for (int i = 0; i < 100; i++) {
		TileType tileData;
		if (i < 20) {
			tileData = TileType::RED;
		}
		else if (i < 40) {
			tileData = TileType::DARK_BLUE;
		}
		else if (i < 60) {
			tileData = TileType::LIGHT_BLUE;
		}
		else if (i < 80) {
			tileData = TileType::BLACK;
		}
		else {
			tileData = TileType::YELLOW;
		}
		tempLinkedList->addBack(tileData);
	}
}

void Bag::fillFromBag(Bag* takeFrom) {
	if (takeFrom != nullptr) {
		for (int i = 0; i < takeFrom->size(); i++) {
			Tile topTile = takeFrom->getTopTile();
			if (topTile.getType() != TileType::FIRST_PLAYER) {
				bagLinkedList->addFront(takeFrom->getTopTile().getType());
			}
		}
		takeFrom->clear();
	}
}

void Bag::addTile(TileType type) {
	bagLinkedList->addFront(type);
}

//get the backmost tile, or the front
Tile Bag::getTopTile() {
	Tile returnTile = Tile(bagLinkedList->transferBack());
	return returnTile;
}