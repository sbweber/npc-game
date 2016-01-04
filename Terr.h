//Samuel Weber

#ifndef TERR_H
  #define TERR_H

  #include "Globals.h"
  #include "Tile.h"

  class Tile;

  class Terr
  {//note: all maps currently must be rectangles.
    vector< vector<Tile*> > map;
    int w; //map width (in tiles)
    int h; //map height (in tiles)
  public:
    Terr(const string &str = ""); //calls loadMap unless the string has length 0
    ~Terr(); //because Tiles use New, they must be destroyed after use
    int getHeight();
    Tile* getTile(int i, int j); //returns a pointer to the tile in position map[i][j]
    int getWidth();
    void loadMap(const string &str);
  };//class Terr

#endif