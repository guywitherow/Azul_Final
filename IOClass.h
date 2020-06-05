#ifndef IO_H
#define IO_H


#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include "Tile.h"
#include "Player.h"
#include "Factory.h"

class IOClass {

   public:

      //prints the menu
      static void printMenu();
      //prints the credits
      static void printCredits();
      //prints a reference board for the player
      static void printReferenceBoard();
      //prints factories for the board, given pointers to these objects
      static void printFactories(Factory** factories, int factoriesSize, std::vector<Factory*> tables);
      //print's a players wall, given that player
      static void printPlayerWall(Player* player);


      static std::vector<std::string> takeUserInput();
   private:
   

};

#endif