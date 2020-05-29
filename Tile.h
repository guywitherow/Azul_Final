#ifndef TILE_H
#define TILE_H

#include "Types.h"
#include <iostream>
#include <string>

class Tile {
   public:
   //Tile constructor 
   Tile();
   Tile(TileType type);

   //Deconstructor
   ~Tile();

   //clear function used in Deconstructor
   void tileClear();

   //Returns a toString of the Tile
   std::string tileToString();
   char tileToChar();

   static TileType charToTile(char givenChar);
   static std::string tileToString(TileType givenType);

   TileType getType();
   static TileType stringToType(std::string type);

   private:
   TileType type;

};

#endif
