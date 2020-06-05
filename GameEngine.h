#ifndef GAMEENGINE_H
#define GAMEENGINE_H


#include <vector>
#include "Tile.h"
#include "Player.h"
#include "Factory.h"
#include <fstream>
#include "IOClass.h"

class GameEngine {

   public:
      GameEngine();
      GameEngine(int playerCount, int tableCount);
      ~GameEngine();

      Player* getPlayer(int playerNumber);

      Bag* getBag();
      Bag* getLid();

      Factory* getFactory(int factoryNumber);

      Factory* getTable();
      Factory* getTable(int tableNumber);

      bool doGameLoop(int seed);
      void doGameEnd();

      void setLoaded(bool loaded);
      void setPlayer(int player, std::string playerName);

      bool takePlayerTurn(int turn); 

      int loadGame();
      void saveGame(std::string saveName);

   private:
      std::vector<Player*> players;
      Factory* factories[MAX_FACTORIES];
      std::vector<Factory*> tables;
      Bag* bag = nullptr;
      Bag* lid = nullptr;

      bool isLoaded = false;
      int turn = 0;
      int factoryCount = 0;
};

#endif