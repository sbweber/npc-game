// Samuel Weber

#ifndef WARP_H
  #define WARP_H

  #include "Tile.h"

  //! Warp is a kind of Tile that loads a new Terr (e.g. leaving a building).
  class Warp : public Tile
  {
  public:
    Warp(Tile* tile, string &nT, bool replaceTile = false);
    //!< Upgrade constructor: copies an existing Tile, adding new functionality
    void setSprite(Sprite* u);
    //!< Overloads setSprite to also load a new map.
  protected:
    string newTerr;
    //!< filename of new Terr
    int destX;
    //!< X-position of character on new Terr
    int destY;
    //!< Y-position of character on new Terr
  };

#endif