// Samuel Weber

#ifndef TILE_H
  #define TILE_H

  #include "Globals.h"
  #include "Sprite.h"


//! Describes Tile spritesheets in column-major order.
enum tileType
{
  TILE_CORE, // non-edge piece
  TILE_CORNER_NE,
  TILE_CORNER_NW,
  TILE_CORNER_SE,
  TILE_CORNER_SW, // two edges (corner)
  TILE_EDGE_E,
  TILE_EDGE_N,
  TILE_EDGE_S,
  TILE_EDGE_W, // one edge (side)
  TILE_THIN_EW,
  TILE_THIN_NS, // two edges (opposite sides)
  TILE_ISOLATED, // four edges
  TILE_END_E,
  TILE_END_N, // three edges, all but N
  TILE_END_S,
  TILE_END_W
};


//! Data about individual tiles from a Terr.
class Tile
{
public:
  Tile(SDL_Texture* t, bool p = true, tileType tt = TILE_CORE, shared_ptr<Sprite> spr = nullptr);
  //!< Tile constructor.
  virtual ~Tile();
  //!< Virtual Tile destructor.
  void clearTile();
  //!< Remove Sprite from this Tile
  void connectTile(DIR d, shared_ptr<Tile> t);
  //!< Specifies an adjacent Tile. Not reciprocal -- may be one-way!
  virtual const string enterTile();
  //!< Overridable base function for derived tiles doing something special
  //!< when entered. Base functionality just sets Sprite.
  double getAngle();
  //!< Get the current angle of sprite rotation for the Tile’s art.
  SDL_RendererFlip getFlip();
  //!< Gets current flip status of Tile’s sprite.
  bool getIsPassable();
  //!< Returns true if the Tile may be entered by a Unit.
  shared_ptr<Sprite> getSprite();
  //!< Returns Sprite* on Tile
  SDL_Texture* getTex();
  //!< Returns the spritesheet for this Tile
  shared_ptr<Tile> getTile(DIR d);
  //!< Returns pointer to adjacent tile in specified direction. May be nullptr.
  tileType getType();
  //!< Returns the Tile’s type.
  int getX();
  //!< Returns the Tile’s x position on its Terr.
  int getY();
  //!< Returns the Tile’s y position on its Terr.
  void setAngle(double a);
  //!< Sets the rotational angle of the Tile’s sprite.
  void setFlip(SDL_RendererFlip f);
  //!< Sets whether the Tile’s sprite is flipped.
  void setIsPassable(bool p);
  //!< Sets whether the Tile may be entered.
  void setPos(int xPos, int yPos);
  //!< Sets where Tile thinks it is on a Terr.
  void setSprite(shared_ptr<Sprite> spr);
  //!< Sets the Sprite on the Tile
  void setTex(SDL_Texture* t);
  //!< Sets the tile spritesheet
  void setType(tileType t);
  //!< Sets what kind of tile it is.
protected:
  double angle;
  //!< Angle at which Tile’s sprite is rotated
  weak_ptr<Tile> E;
  //!< Adjacent Tile to the East/Right.
  SDL_RendererFlip flip;
  //!< Flip status of Tile’s sprite
  shared_ptr<Sprite> sprite;
  //!< Sprite on tile. nullptr if none.
  bool isPassable;
  //!< True if tile can be entered.
  //!< May change later to enterable from each side.
  weak_ptr<Tile> N;
  //!< Adjacent Tile to the North/Up.
  weak_ptr<Tile> S;
  //!< Adjacent Tile to the South/Down.
  SDL_Texture* tex;
  //!< Pointer to the spritesheet for this Tile.
  tileType type;
  //!< Which type of tile to draw.
  weak_ptr<Tile> W;
  //!< Adjacent Tile to the West/Left.
  int x;
  //!< X-position Tile believes it holds on a Terr.
  int y;
  //!< Y-position Tile believes it holds on a Terr.
};

#endif