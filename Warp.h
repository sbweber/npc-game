// Samuel Weber

#ifndef WARP_H
  #define WARP_H

  #include "Tile.h"

  //! Warp is a kind of Tile that loads a new Terr (e.g. leaving a building).
  class Warp : public Tile
  {
  public:
    Warp(Tile* tile, const string &dT, int32_t dX, int32_t dY,
            bool replaceTile = false);
    //!< Upgrade constructor: copies an existing Tile, adding new functionality
    int32_t getDestX();
    //!< Returns the Warp's destination X-position.
    int32_t getDestY();
    //!< Returns the Warp's destination Y-position.
    string getDestTerr();
    //!< Returns the filename of the destination Terr.
    bool isWarp() override;
    //!< Returns true. Base version from Tile class returns false.
  protected:
    string destTerr;
    //!< filename of destination Terr
    int32_t destX;
    //!< X-position of character on new Terr
    int32_t destY;
    //!< Y-position of character on new Terr
  };

#endif