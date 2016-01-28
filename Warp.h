// Samuel Weber

#ifndef WARP_H
  #define WARP_H

  #include "Tile.h"

  //! Warp is a kind of Tile that loads a new Terr (e.g. leaving a building).
  class Warp : public Tile
  {
  public:
    Warp(shared_ptr<Tile> tile, const string &dT, int dX, int dY);
    //!< Upgrade constructor: copies an existing Tile, adding new functionality
    virtual const string enterTile() override;
    //!< Loads a new Terr on entry.
    int getDestX();
    //!< Returns the Warp's destination X-position.
    int getDestY();
    //!< Returns the Warp's destination Y-position.
    string getDestTerr();
    //!< Returns the filename of the destination Terr.
    //!< Try to move Sprite in the indicated direction. Return Tile it ends on.
  protected:
    string destTerr;
    //!< filename of destination Terr
    int destX;
    //!< X-position of character on new Terr
    int destY;
    //!< Y-position of character on new Terr
  };

#endif