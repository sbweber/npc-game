// Samuel Weber

#ifndef TERR_H
  #define TERR_H

  #include "Globals.h"
  #include "Tile.h"
  #include "Warp.h"

  class Tile;

  //! Describes an overworld map. All maps currently must be rectangles.
  class Terr
  {
  public:
    Terr(const string &str = "");
    //!< Calls loadMap unless the string is empty.
    ~Terr();
    //!< Default destructor. Because Tiles use new, they must be destroyed after use.
    int getHeight();
    //!< Returns the height (h) of Terr.
    Tile* getTile(int i, int j);
    //!< Returns a pointer to the tile in position map[i][j].
    int getWidth();
    //!< Returns the width (w) of Terr.
    void loadMap(const string &str);
    //!< Loads a map described in filename str
  protected:
    vector< vector<Tile*> > map;
    //!< 2D dyanmically resized array of Tiles defining Terr.
    int32_t w;
    //!< map width (in Tiles)
    int32_t h;
    //!< map height (in Tiles)
  };  // class Terr

#endif