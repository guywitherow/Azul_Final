#ifndef TILENODE_H
#define TILENODE_H

#include "Tile.h"
#include <string>

class TileNode
{
    
public:
    //Constructor
    TileNode(Tile *tileData, TileNode *nextTileNode);

    // Deconstructor
    ~TileNode();

    // Returns the Tile
    Tile *getTileData();

    //Returns the next TileNode
    TileNode *getNextTileNode();

    //Sets the next TileNode
    void setNextTileNode(TileNode *nextTileNode);
    // Returns the details of the TileNode
    std::string getTileDataToString(Tile *tileData);

private:
    Tile *tileData;
    TileNode *nextTileNode;
};

#endif // TILENODE_H