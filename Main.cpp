#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h> 
#include <cstdlib>
#include <iterator>
#include "Player.h"
#include "Factory.h"
#include "Types.h"
#include "Tile.h"
#include "Bag.h"
#include <windows.h>
#include <vld.h>


int loadGame(Factory* (*factories)[NUM_FACTORIES], Factory* table, Bag* bag, Player* player1, Player* player2);
void saveGame(std::string saveName, Factory* (*factories)[NUM_FACTORIES], Factory* table, Bag* bag, Player* player1, Player* player2);

void game(int seed, bool load);
void takePlayerTurn(Factory* (*factories)[NUM_FACTORIES], Factory* table, Player* player, Bag* bag, Player* player2);

void printMenu();
void printCredits();
void printReferenceBoard();
void printFactories(Factory* (*factories)[NUM_FACTORIES], Factory* table);
void printPlayerWall(Player* player);

std::vector<std::string> takeUserInput();
std::vector<std::string> takeUserInput(Factory* (*factories)[NUM_FACTORIES],
   Factory* table, Bag* bag, Player* player1, Player* player2);



int main(int argc, char const *argv[])
{
   //loop until we quit
   bool quit = false;
   printMenu();
   while (!quit) {
      std::string input = takeUserInput().at(0);
      
      //handle input
      if (input == "1") {
         int seed = 0;
         //new game
         if (argc == 2) {
            //TODO check seed is a valid seed
            seed = atoi(argv[1]);
            srand(seed);
         }
         else {
            //generate a random seed
            // srand(time(0));
            srand(1);
         }

         seed = rand();
         game(seed, false);
         
      } 
      else if (input == "2") {
         //load game
         game(0, true);
      } 
      else if (input == "3") {
         //credits
         printCredits();
      } 
      else if (input == "4") {
         //quit
         quit = true;
      } 
      else {
         //bad input, try again
         std::cout << "Choice not recognised, please choose an option from the list" << std::endl;
      }
   }

   return 0;
}

void game(int seed, bool load) {
   std::cout << "Starting a new game" << std::endl;
   int turn = 0;
   //bag init
   Bag* bag = new Bag(seed);

   Factory* table = new Factory();

   Factory* factories[NUM_FACTORIES];
   for (int i = 0; i < NUM_FACTORIES; i++) {
      factories[i] = new Factory();
   }
   //player names
   Player* player1 = new Player("");
   Player* player2 = new Player("");
   if (load) {
      turn = loadGame(&factories,table,bag,player1,player2);
      if (turn == -1) {
         load = false;
         std::cout << "Load Failed. Starting a fresh game." << std::endl << std::endl;
      }
      else {
         std::cout << "Loaded" << std::endl << std::endl;
      }
   }
   if (!load) {
      bag->fill();
      bag->shuffle();

      std::cout << std::endl << "Enter a name for player 1" << std::endl ;
      std::string name = takeUserInput().at(0);
      player1 = new Player(name);

      std::cout << std::endl << "Enter a name for player 2" << std::endl ;
      name = takeUserInput().at(0);
      player2 = new Player(name);

      std::cout << "Let's Play!" << std::endl << std::endl;
   }


   
   int round = 0;
   bool roundLoop = true;
   while (roundLoop) {
      //ensure clear factories, then fill
      if (round != 0 || load != true) {
         for (int i = 0; i < NUM_FACTORIES; i++) {
            factories[i]->clearFactory();
            for (int j = 0; j < TILES_PER_FAC; j++) {
               factories[i]->addTile(bag->getTopTile());
            }
         }
         //ensure clean table, add first player tile
         table->clearFactory();
         table->addTile(Tile(TileType::FIRST_PLAYER));
      }

      bool tileLoop = true;
      turn = 0;
      while (tileLoop) {
         Player* currentPlayer = player1;
         Player* secondPlayer = player2;
         if (turn % 2 == 0) {
            currentPlayer = player2;
            secondPlayer = player1;
         }
         takePlayerTurn(&factories, table, currentPlayer,bag, secondPlayer );
         
         std::cout << "Here is your board after that move." << std::endl;

         printPlayerWall(currentPlayer);

         //check if any tiles are left
         int factoryTileCount = 0;
         int tableTileCount = 0;
         tableTileCount = table->getTileCount();
         for (int i = 0; i < NUM_FACTORIES; i++) {
            //check all factories
            factoryTileCount = factories[i]->getTileCount();
            if (factoryTileCount != 0) {
               i = 5;
            }
         }
         //if any factory has tiles left, let the players go again.
         if (tableTileCount > 0 || factoryTileCount > 0) {
            tileLoop = true;
         }
         else {
            tileLoop = false;
         }
         turn++;
      }

      //SCORE HERE

      int player1Win = player1->getWall()->moveStorageToWall();
      int player2Win = player2->getWall()->moveStorageToWall();

      player1->setScore(player1->getScore() + player1Win);
      player2->setScore(player2->getScore() + player2Win);

      printPlayerWall(player1);
      std::cout << "Player 1 Score: " << player1->getScore() << std::endl << std::endl;
      printPlayerWall(player2);
      std::cout << "Player 2 Score: " << player2->getScore() << std::endl << std::endl;
      //move tiles for both players to the board
      //check if either player has a full board line
      bool playersDone = player1->getWall()->checkIfDone();
      if (playersDone == false) {
         playersDone = player2->getWall()->checkIfDone();
      }

      if (playersDone == true) {
         roundLoop = false;
         std::cout << "game over, a player has 5 in a row." << std::endl;
      }
      else {
         std::cout << "next round starting" << std::endl;
      }

      //if they do, end, and compare scores
      //otherwise, go again

      //stop looping the rounds when a player wins
      
      round++;
   }
   std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;

   int player1Final = player1->getWall()->endOfGameScore();
   int player2Final = player2->getWall()->endOfGameScore();

   player1->setScore(player1->getScore() + player1Final);
   player2->setScore(player2->getScore() + player2Final);

   std::cout << "Final Boards:" << std::endl;

   printPlayerWall(player1);
   std::cout << "Player 1 Score: " << player1->getScore() << std::endl << std::endl;
   printPlayerWall(player2);
   std::cout << "Player 2 Score: " << player2->getScore() << std::endl << std::endl;

   std::cout << std::endl << std::endl << "Game Over." << std::endl << std::endl;
   delete bag;
   delete player1;
   delete player2;
   delete table;
   for (int i = 0; i < 5; i++) {
      delete factories[i];
   }
}

void takePlayerTurn(Factory* (*factories)[NUM_FACTORIES], Factory* table, Player* player, Bag* bag, Player* player2) {
   printFactories(factories, table);
   printPlayerWall(player);
   bool valid = false;
   while (!valid) {
      std::vector<std::string> turn = takeUserInput(factories,
         table, bag, player, player2);
      if (turn.size() > 0) {
         std::string command = turn.at(0);
         if (command == "help" || command == "h") {
            std::cout << "This is where you take your turn. Use the command 'turn', ";
            std::cout << "along with your selections to take your turn!" << std::endl;
            std::cout << "eg: '> turn 3 L 3' moves all light blue tiles, from ";
            std::cout << "factory 3, and puts them in storage line 3" << std::endl << std::endl;
         }
         else if (command == "turn") {
            if (turn.size() != 4) {
               std::cout << "Please use the command format, 'turn <factory> <color_code> <buffer>" << std::endl << std::endl;
            }
            else {
               //check turn input is valid
               int factory = -1;
               int bufferLine = -1;

               //factory int interp
               try {
                  factory = std::stoi(turn.at(1)) -1;
               }
               //catch oob or invalid
               catch (...) {
                  std::cout << "Please input a valid factory ID, between 0 and 5." << std::endl << std::endl;
               }
               //bufferline in interp
               try {
                  bufferLine = std::stoi(turn.at(3)) -1;
               }
               //same as above
               catch (...) {
                  std::cout << "Please input a valid buffer ID, between 1 and 5." << std::endl << std::endl;
               }

               //change tile string to TileType
               TileType userTileType = Tile::stringToType(turn.at(2));

               std::string errorCheck = "";

               //errorCheck += checkValidFactory(factory);
               //errorCheck += checkValidTile(tileChar);
               //errorCheck += checkValidBuffer(bufferLine);

               if (player->getWall()->checkWallLineForType(bufferLine, userTileType) == true) {
                  errorCheck += '\n' + "Error. This line already has a tile of that type.";
               }

               if (errorCheck == "") {
                  int tiles = -1;
                  if (factory == -1) {
                     tiles = table->removeTile(userTileType);
                     if (table->removeTile(TileType::FIRST_PLAYER) != 0) {
                        player->getWall()->addToFloorLine(TileType::FIRST_PLAYER, 1);
                     }
                  }
                  else {
                     tiles = (*factories)[factory]->removeTile(userTileType);
                  }

                  if (tiles > 0) {
                     valid = true;
                     player->getWall()->addToStorageLine(userTileType, tiles, bufferLine);
                     std::vector<Tile> moveToTable;
                     if (factory != -1) {
                        moveToTable = (*factories)[factory]->getTiles();
                        (*factories)[factory]->clearFactory();
                        for (int i = 0; i < TILES_PER_FAC; i++) {
                           (*factories)[factory]->addTile(Tile());
                        }
                     }
                     
                     if (moveToTable.size() > 0) {
                        for (auto i = moveToTable.begin(); i != moveToTable.end(); i++) {
                           if (i->getType() != TileType::NO_TILE) {
                              table->addTile(Tile(i->getType()));
                           }
                        }
                     }
                     
                  }
                  else {
                     std::cout << "Factory number " + std::to_string(factory + 1) + " has no tiles of type ";
                     std::cout << Tile::tileToString(userTileType) << ". Please make a valid selection.";
                     //the player has chosen to take a type of tile that is not in that factory
                  }

               }
               else {
                  //some blah about valid tile types, as well as buffers and factories
                  std::cout << errorCheck << std::endl;
               }
            }
         }
         else if (command == "quit") {
            //exit the game
         }
         else if (command == "save") {
            //save the game
         }
      }
      else {
         std::cout << "Please input a command." << std::endl << std::endl;
      }
   }
   
      
   
}

void printFactories(Factory* (*factories)[NUM_FACTORIES], Factory* table) {

   std::cout << "Factories: " << std::endl << "0: ";
   table->printFactory();
   std::cout << std::endl;

   for (int i = 0; i < NUM_FACTORIES; i++) {
      if (factories != nullptr) {
         std::cout << i + 1 << ": ";
         (*factories)[i]->printFactory();
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}

void printPlayerWall(Player* player) {
   std::string playerName = player->getName();
   std::string wallString = player->getWall()->getPlayerWallString();

   std::cout << "Mosaic for " << playerName << ":" << std::endl << std::endl;
   std::cout << wallString << std::endl;
   std::cout << std::endl;
}

void printMenu() {
   //just print these lines as the main menu
   std::cout << "Welcome to Azul!" << std::endl;
   std::cout << "-------------------" << std::endl;
   std::cout << "Menu" << std::endl;
   std::cout << "----" << std::endl;
   std::cout << "1. New Game" << std::endl;
   std::cout << "2. Load Game" << std::endl;
   std::cout << "3. Credits (Show student information)" << std::endl;
   std::cout << "4. Quit" << std::endl;
}

void printCredits() {

   HANDLE hConsole;

   hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
   std::cout << "----------------------------------" << std::endl;
   std::cout << "Name:        Jordan Tori" << std::endl;
   std::cout << "Student ID:  s3542275" << std::endl;
   std::cout << "Email:       s3542275@student.rmit.edu.au" << std::endl;
   std::cout << "Name:        Merhawi Minassi" << std::endl;
   std::cout << "Student ID:  s3668396" << std::endl;
   std::cout << "Email:       s3668396@student.rmit.edu.au" << std::endl;
   std::cout << "Name:        Guy Witherow" << std::endl;
   std::cout << "Student ID:  s3783428" << std::endl;
   std::cout << "Email:       s3783428@student.rmit.edu.au" << std::endl;
   std::cout << "----------------------------------" << std::endl;
   std::cout << "Individual Extension by:" << std::endl;
   std::cout << "Name:        Guy Witherow" << std::endl;
   std::cout << "Student ID:  s3783428" << std::endl;
   std::cout << "Email:       s3783428@student.rmit.edu.au" << std::endl;
   std::cout << "----------------------------------" << std::endl;
   SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void printReferenceBoard() {
   std::cout << "This is the pattern each board follows!" << std::endl;
   //maybe add command to bring this up?
   char tiles[WALL_DIM] = { 'B', 'Y', 'R', 'U', 'L' };
   for (int i = 0; i < WALL_DIM; i++) {
      for (int j = 0; j < WALL_DIM; j++) {
         int tileNumber = (j + i) % 5;
         std::cout << tiles[tileNumber] << " ";
      }
      std::cout << std::endl;
   }
   std::cout << std::endl << std::endl;
   std::cout << "Please make moves using the format 'turn <factory number> <tile color letter> <storage row>" << std::endl << std::endl;
}

std::vector<std::string> takeUserInput(Factory* (*factories)[NUM_FACTORIES], 
   Factory* table, Bag* bag, Player* player1, Player* player2) {
   //TODO Ensure user input is valid
   bool saveCheck = true;
   std::string input = "";
   std::vector<std::string> container;
   while (saveCheck) {
      
      //user input space is indicated by an arrow
      std::cout << std::endl << "> ";
      std::getline(std::cin, input);

      //break string into arguments
      std::istringstream stream{ input };
      using StrIt = std::istream_iterator<std::string>;
      std::vector<std::string> container{ StrIt{stream}, StrIt{} };

      if (container.size() == 0) {
         container.push_back(" ");
      }

      if (container.at(0) == "save") {
         if (container.size() == 2) {
            saveGame(container.at(1), factories, table, bag, player1, player2);
            std::cout << "Saved game." << std::endl << std::endl << "Resuming" << std::endl;
         }
         else {
         }

      }
      else {
         saveCheck = false;
      }
   }
   

   return container;
}

std::vector<std::string> takeUserInput() {
   //TODO Ensure user input is valid

   std::string input = "";


   //user input space is indicated by an arrow
   std::cout << std::endl << "> ";
   std::getline(std::cin, input);

   //break string into arguments
   std::istringstream stream{ input };
   using StrIt = std::istream_iterator<std::string>;
   std::vector<std::string> container{ StrIt{stream}, StrIt{} };

   if (container.size() == 0) {
      container.push_back(" ");
   }



   return container;
}

void saveGame(std::string saveName, Factory* (*factories)[NUM_FACTORIES], 
   Factory* table, Bag* bag, Player* player1, Player* player2) {

   std::ofstream savefile;
   savefile.open("tests/" + saveName + ".txt");
   int currentLine = 0;
   int dataIndex = 0;
   while (currentLine <= 35) {
      std::string data = "";

      if (currentLine == 0) {
         data = bag->getAllTiles();
      }
      else if (currentLine >= 1 && currentLine <= 5) {
         dataIndex = currentLine - 1;
         data = (*factories)[dataIndex]->getTilesString();
      }
      else if (currentLine == 6) {
         data = table->getTilesString();
      }
      else if (currentLine >= 7 && currentLine <= 32) {
         //13 lines of data for a player, * 2
         dataIndex = currentLine - 7;
         if (dataIndex / 13 == 0) {
            data = player1->getDataString(dataIndex);
         }
         else {
            data = player2->getDataString(dataIndex % 13);
         }
      }
      else if (currentLine == 33) {
         data = "0";
      }

      savefile << data + "\n";
      currentLine++;
   }



   savefile.close();

}

int loadGame(Factory* (*factories)[NUM_FACTORIES], Factory* table, Bag* bag, Player* player1, Player* player2) {
   //get user input for which file to load
   std::string input;
   std::cout << "Enter the name of the file which you want to load."
      << std::endl << "> ";
   std::cin >> input;

   int turnToReturn = -1;

   //open specified file
   std::string line;
   std::ifstream loadFile(input + ".txt");
   int currentLine = 0;
   if (loadFile.is_open()) {
      //read lines, discarding # or empty lines
      while (getline(loadFile, line)) {
         std::string lineCheck = line;
         if (line[0] == '#' || line.length() == 0) {
            //do nothin
         }

         //read in the data
         else {
            int dataIndex = 0;

            //identify what part of the file we are reading
            if (currentLine == 0) {
               bag->load(line);
            }
            else if (currentLine >= 1 && currentLine <= 5) {
               dataIndex = currentLine - 1;
               (*factories)[dataIndex]->load(line);
            }
            else if (currentLine == 6) {
               table->load(line);
            }
            else if (currentLine >= 7 && currentLine <= 32) {
               //13 lines of data for a player, * 2
               dataIndex = currentLine - 7;
               if (dataIndex / 13 == 0) {
                  player1->load(line, dataIndex % 13);
               }
               else {
                  player2->load(line, dataIndex % 13);
               }
            }
            else if (currentLine == 33) {
               try {
                  turnToReturn = std::stoi(line);
               }
               catch (...) {
                  std::cout << "Current Turn could not be read, assuming player 1" << std::endl;
                  turnToReturn = 1;
               }
            }
            currentLine++;
         }
      }

      loadFile.close();
   }
   else std::cout << "Unable to open file" << std::endl;
   return turnToReturn;
}