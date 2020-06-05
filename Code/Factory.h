#ifndef FACTORY_H
#define FACTORY_H


#include <vector>
#include <iostream>
#include <algorithm>
#include "Tile.h"

class Factory {

   public:
   Factory();
   ~Factory();
   int removeTile(TileType type);
   std::vector<Tile> getTiles();
   void addTile(Tile tile);
   void clearFactory();
   void toString();
   int getTileCount();
   void sortFactory();

   std::string getTilesString();

   void load(std::string data);

   private:
   std::vector<Tile> tiles;
   int tileCount = 0;

};

#endif