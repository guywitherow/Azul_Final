#ifndef BAG_H
#define BAG_H

#include "Tile.h"
#include "LinkedList.h"
#include <random>

class Bag
{
public:
   Bag();
   ~Bag();

   void clear();

   int size();
   void empty();
   void fill();
   void shuffle(int seed);
   void load(std::string data);
   void fillFromBag(Bag* takeFrom);
   void addTile(TileType type);
   Tile getTopTile();
   std::string getAllTiles();

private:

   // This Generates a temporary linkedlist that contains all the tiles in order 
   LinkedList* tempLinkedList;

   // Randomizes the Tiles in the temporary linkedlist and stores them in this linkedlist.
   // This linkedlist will be the main linkedlist where players take tiles from.
   LinkedList* bagLinkedList;
};
#endif