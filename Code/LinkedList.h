
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "TileNode.h"
#include "Tile.h"

class LinkedList 
{
   public:
   //Consructor
   LinkedList();

   //Deconstructor
   ~LinkedList();

   //will be used in Deconstructor
   void clear();

   //Returns the size of the linked list 
   int getSize();

   //This returns the tile in the linked list based on the index 
   Tile* getTile(int index);

   //Based on the index a Tile in the linked list will return this toString
   std::string getTileToString(int index);

   //Adding a tile on either front or back of linked list
   void addFront(TileType tileInfo);
   void addBack(TileType tileinfo);

   //Deleting/Removing a tile from a linked list
   void deleteAt(int index);
   void deleteFront();
   void deleteBack();

   //Transferring a tile from one linked list to another
   void transferTo(int index, LinkedList* list);
   TileType transferFront();
   TileType transferBack();

   private:
   TileNode* head;
   TileNode* tail;

   //The Temp transfer for when a tile that's used to define the tile to be transferred from one linked list to another
   Tile* transferTemp;
};
#endif //LINKEDLIST_H//