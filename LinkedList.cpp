#include <iostream>
#include "LinkedList.h"

LinkedList::LinkedList() {
	this->head = nullptr;
	this->tail = nullptr;
	this->transferTemp = nullptr;
}

LinkedList::~LinkedList() {
	clear();
}

void LinkedList::clear() {
	if (getSize() != 0) {
		while (head != nullptr) {
			TileNode* current = head;
			head = current->getNextTileNode();
			delete current;
		}
	}
}

int LinkedList::getSize() {
	int size = 0;

	if (head == nullptr) {
		size = 0;
	}
	else {
		TileNode* current = head;
		while (current != nullptr) {
			current = current->getNextTileNode();
			size = size + 1;
		}
	}
	return size;
}

Tile* LinkedList::getTile(int index) {
	Tile* tile;
	TileNode* current = head;

	if (index == 0) {
		tile = current->getTileData();
	}
	else if (index == (getSize() - 1)) {
		current = tail;
		tile = tail->getTileData();
	}
	else {
		int count = 0;

		while (count < index) {
			current = current->getNextTileNode();
			count = count + 1;
		}

		tile = current->getTileData();
	}
	return tile;
}

void LinkedList::addFront(TileType tileData) {
	TileNode* newTile = new TileNode(new Tile(tileData), head);

	if (tail == nullptr) {
		tail = newTile;
	}
	head = newTile;
}

void LinkedList::addBack(TileType tileData) {
	TileNode* newTile = new TileNode(new Tile(tileData), nullptr);
	if (head == nullptr) {
		head = newTile;
	}
	else {
		TileNode* current = head;
		while (current->getNextTileNode() != nullptr) {
			current = current->getNextTileNode();
		}
		current->setNextTileNode(newTile);
	}
	tail = newTile;
}

void LinkedList::deleteAt(int index) {
	if (index == 0) {
		deleteFront();
	}
	else if (index == (getSize() - 1)) {
		deleteBack();
	}
	else {
		int count = 0;
		TileNode* current = head;
		while (count < index) {
			current = current->getNextTileNode();
			count = count + 1;
		}
		TileNode* before = head;
		while (before->getNextTileNode() != current) {
			before = before->getNextTileNode();
		}
		before->setNextTileNode(current->getNextTileNode());
		delete current;
	}
}

void LinkedList::deleteFront() {
	if (head != nullptr) {
		TileNode* current = head;
		head = current->getNextTileNode();
		delete current;
	}
}

void LinkedList::deleteBack() {
	TileNode* end = tail;
	if (tail != nullptr) {
		TileNode* current = head;
		while (current->getNextTileNode() != tail) {
			current = current->getNextTileNode();
		}
		current->setNextTileNode(nullptr);
		tail = current;
	}

	delete end;
}

void LinkedList::transferTo(int index, LinkedList* randomList) {
	TileType transferType = TileType::NO_TILE;

	if (index == 0) {
		transferType = transferFront();
	}
	else if (index == (getSize() - 1)) {
		transferType = transferBack();
	}
	else {
		int count = 0;
		TileNode* before = head;
		TileNode* current = head;
		while (count < index) {
			before = current;
			current = current->getNextTileNode();
			count = count + 1;
		}

		before->setNextTileNode(current->getNextTileNode());
		transferType = current->getTileData()->getType();
		delete current;
	}
	randomList->addBack(transferType);
}

TileType LinkedList::transferFront() {
	TileType tileToTransfer = TileType::NO_TILE;

	if (head != nullptr) {
		TileNode* current = head;
		head = current->getNextTileNode();
		tileToTransfer = current->getTileData()->getType();
		delete current;
	}
	return tileToTransfer;
}

TileType LinkedList::transferBack() {
	TileType tileToTransfer = TileType::NO_TILE;

	TileNode* end = tail;
	if (tail != nullptr) {
		TileNode* current = head;
		while (current->getNextTileNode() != tail) {
			current = current->getNextTileNode();
		}
		tileToTransfer = end->getTileData()->getType();
		current->setNextTileNode(nullptr);
		tail = current;
		delete end;
	}

	return tileToTransfer;
}