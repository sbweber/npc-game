// Samuel Weber

#ifndef TERR_H
  #define TERR_H

  #include "Globals.h"
  #include "Sprite.h"
  #include "Tile.h"
  #include "Warp.h"


  typedef boost::bimap<boost::bimaps::unordered_set_of<Sprite*>,
          boost::bimaps::unordered_set_of<Tile*> > spritelog;
  typedef spritelog::value_type location;


  //! Describes an overworld map. All maps currently must be rectangles.
  class Terr
  {
  public:
    Terr(const string &str = "");
    //!< Calls loadMap unless the string is empty.
    ~Terr();
    //!< Default destructor. Because Tiles use new, they must be deleted.
    int getHeight();
    //!< Returns the height (h) of Terr.
    Sprite* getSprite(Tile* tile);
    //!< Returns the Sprite* on the given Tile.
    Tile* getTile(int i, int j);
    //!< Returns a pointer to the tile in position map[i][j].
    Tile* getTile(Sprite* sprite);
    //!< Returns the Tile under the given Sprite.
    int getWidth();
    //!< Returns the width (w) of Terr.
    bool isOccupied(Tile* tile);
    //!< Return true if the tile* has a Sprite associated with it.
    void loadMap(const string &str);
    //!< Loads a map described in filename str.
    void setSprite(Sprite* sprite, Tile* tile);
    //!< Sets a Sprite-Tile relationship.
    void setTile(Tile* tile, Sprite* sprite);
    //!< Sets a Sprite-Tile relationship. Alias for setSprite.
  protected:
    vector< vector<Tile*> > map;
    //!< 2D dyanmically resized array of Tiles defining Terr.
    spritelog sprites;
    //!< bi-directional map relating each Sprite to its Tile (and vice versa).
    int w;
    //!< map width (in Tiles)
    int h;
    //!< map height (in Tiles)
  };  // class Terr

#endif