// Samuel Weber

#ifndef WARP_H
  #define WARP_H

  #include "Tile.h"

  //! Warp is a kind of Tile that loads a new Terr (e.g. leaving a building).
  class Warp : public Tile
  {
  protected:
    string newTerr;
    //!< filename of new Terr
    // This class still only stubbed.
    // Important information: Name of new Terr to load, position on
    // new Terr to place character.
  };

#endif