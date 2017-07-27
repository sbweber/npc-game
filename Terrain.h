// Samuel Weber

#ifndef TERR_H
  #define TERR_H

  #include "Globals.h"
  #include "Sprite.h"
  #include "Tile.h"
  #include "Unit.h"
  #include "Warp.h"


//! Describes an overworld map. All maps currently must be rectangles.
class Terrain
{
public:
  Terrain(SDL_Renderer *r, mt19937_64 &randNumGen, const string &str = "");
  //!< Calls loadMap unless the string is empty.
  ~Terrain();
  //!< Default destructor. Because Tiles use new, they must be deleted.
  string actSprite(shared_ptr<Sprite> partySprite,
          shared_ptr<Sprite> sprite, vector<shared_ptr<Unit> > &enemies);
  //!< A single, specific Sprite pops an action (if any) and enacts it.
  string actSprites(shared_ptr<Sprite> partySprite,
          vector<shared_ptr<Unit> > &enemies);
  //!< All Sprites pop an action (if any) and enact it.
  void findPath(shared_ptr<Tile> start, shared_ptr<Tile> dest,
          shared_ptr<Sprite> sprite);
  //!< A* to find a path from start to dest, then enqueue in sprite.
  int getHeight();
  //!< Returns the height (h) of Terr.
  SDL_Renderer* getRen();
  //!< Returns the Renderer.
  shared_ptr<Sprite> getSprite(shared_ptr<Tile> tile);
  //!< Returns the shared_ptr<Sprite> on the given Tile.
  shared_ptr<Tile> getTile(int i, int j);
  //!< Returns a pointer to the tile in position map[i][j].
  shared_ptr<Tile> getTile(shared_ptr<Sprite> sprite);
  //!< Returns the Tile under the given Sprite.
  int getWidth();
  //!< Returns the width (w) of Terr.
  string interactSprite(shared_ptr<Sprite> sprite,
          vector<shared_ptr<Unit> > &enemies);
  //!< Intelligently interact with Sprite being faced, if any.
  string interactSprites(shared_ptr<Sprite> sprite,
          shared_ptr<Sprite> target, vector<shared_ptr<Unit> > &enemies);
  //!< Intelligently interact between two sprites.
  bool isOccupied(shared_ptr<Tile> tile);
  //!< Return true if the Tile has a Sprite associated with it.
  void loadMap(const string &str, mt19937_64 &randNumGen);
  //!< Loads a map described in filename str.
  string moveSprite(shared_ptr<Sprite> sprite, dir d);
  //!< Move specified Sprite in specified direction (if possible).
  void setSprite(shared_ptr<Sprite> sprite, shared_ptr<Tile> tile);
  //!< Sets a Sprite-Tile relationship.
  void setTile(shared_ptr<Tile> tile, shared_ptr<Sprite> sprite);
  //!< Sets a Sprite-Tile relationship. Alias for setSprite.
  void tickSprites(mt19937_64& rng);
  //!< Tells all Sprites that a tick has occurred.
  shared_ptr<Tile> tileClick(SDL_MouseButtonEvent &click,
          shared_ptr<Sprite> sprite);
  //!< Returns pointer to the Tile clicked. Needs Sprite the map is watching.
protected:
  int h;
  //!< map height (in Tiles)
  vector<vector<shared_ptr<Tile> > > map;
  //!< 2D dyanmically resized array of Tiles defining Terr.
  SDL_Renderer *ren;
  //!< The SDL_Renderer to which everything is drawn.
  unordered_map<string, SDL_Texture*> tileSS;
  //!< All Tile spritesheets, stored by key.
  int w;
  //!< map width (in Tiles)

  bool findCheckRoute(dir d, unordered_map<shared_ptr<Tile>, int> &tiles, shared_ptr<Tile> tile);
  //!< Returns true if a tile in dir d exists and gets you closer on your
  //!< route. Requires a fully loaded table of tuples showing how close the
  //!< tiles are on potential routes.
  void findEnqueue(dir d, priority_queue<tuple<int, shared_ptr<Tile> >,
          vector<tuple<int, shared_ptr<Tile> > >,
          greater<tuple<int, shared_ptr<Tile> > > > &searchQ,
          unordered_map<shared_ptr<Tile>, int> &tiles,
          tuple<int, shared_ptr<Tile> > t, shared_ptr<Tile> target);
  //!< Utility function for findPath(): marks/enqueues tiles.
  int findDistance(shared_ptr<Tile> start, shared_ptr<Tile> end);
  //!< Returns taxicab distance to get to end.
  void loadSprite(ifstream &file, mt19937_64 &randNumGen);
  //!< Loads a Sprite to the map.
  void loadWarpTile(ifstream &file);
  //!< Loads a Warp Tile to the map.
}; // class Terr

#endif