#ifndef FACTORY_H
#define FACTORY_H


#include <vector>
#include "Tile.h"

class Factory {

   public:
   Factory();
   ~Factory();
   int removeTile(TileType type);
   std::vector<Tile> getTiles();
   void addTile(Tile tile);
   void clearFactory();
   void printFactory();
   int getTileCount();

   std::string getTilesString();

   void load(std::string data);

   private:
   std::vector<Tile> tiles;
   int tileCount = 0;

};

#endif