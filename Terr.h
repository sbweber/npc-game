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
    void enterTileMessageHandler(const string &message, Tile* tile);
    //!< Takes in a message from a Tile being entered, reacts accordingly.
    void findPath(Tile* start, Tile* dest, Sprite* sprite);
    //!< A* to find a path from start to dest, then enqueue in sprite.
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
    void interactSprite(Sprite* sprite);
    //!< Intelligently interact with Sprite being faced, if any.
    void interactSprites(Sprite* sprite, Sprite* target);
    //!< Intelligently interact between two sprites.
    bool isOccupied(Tile* tile);
    //!< Return true if the tile* has a Sprite associated with it.
    void loadMap(const string &str);
    //!< Loads a map described in filename str.
    void moveSprite(Sprite* sprite, dir d);
    //!< Move specified Sprite in specified direction (if possible).
    void setSprite(Sprite* sprite, Tile* tile);
    //!< Sets a Sprite-Tile relationship.
    void setTile(Tile* tile, Sprite* sprite);
    //!< Sets a Sprite-Tile relationship. Alias for setSprite.
    Tile* tileClick(SDL_MouseButtonEvent &click, Sprite* sprite);
    //!< Returns pointer to the Tile clicked. Needs Sprite the map is watching.
  protected:
    int h;
    //!< map height (in Tiles)
    vector< vector<Tile*> > map;
    //!< 2D dyanmically resized array of Tiles defining Terr.
    spritelog sprites;
    //!< bi-directional map relating each Sprite to its Tile (and vice versa).
    int w;
    //!< map width (in Tiles)
    bool findCheckRoute(dir d, unordered_map<Tile*, int> *tiles, Tile* tile);
    //!< Returns true if a tile in dir d exists and gets you closer on your
    //!< route. Requires a fully loaded table of tuples showing how close the
    //!< tiles are on potential routes.
    void findEnqueue(dir d, priority_queue<tuple<int, Tile*>,
            vector<tuple<int, Tile*> >, greater<tuple<int, Tile*> > > *searchQ,
            unordered_map<Tile*, int> *tiles, tuple<int, Tile*> t,
            Tile* target);
    //!< Utility function for findPath(): marks/enqueues tiles.
    int findDistance(Tile* start, Tile* end);
    //!< Returns taxicab distance to get to end.
  };  // class Terr

#endif