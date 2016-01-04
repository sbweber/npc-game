// Samuel Weber

#ifndef WARP_H
  #define WARP_H

  #include "Tile.h"

  class Warp : public Tile
  {
  protected:
    string newTerr;
            // Important information: Name of new Terr to load, position on
            // new Terr to place character
  };  // A Warp is a kind of Tile that loads a new map (e.g. leaving a building)

#endif