// Samuel Weber

#ifndef TILE_H
  #define TILE_H

  #include "Globals.h"
  #include "Sprite.h"

  class Sprite;

  //! Describes the Tiles.png spritesheet in column-major order.  enum tileType
  {
    VOID,  //!< black, impassable
    OPEN,  //!< white, passable
    CARPET_CORE,  //!< fancy carpet, non-edge piece
    CARPET_CORNER_SW,  //!< fancy carpet, two edges (corner)
    CARPET_CORNER_NW,
    CARPET_CORNER_NE,
    CARPET_CORNER_SE,
    CARPET_EDGE_W,  //!< fancy carpet, one edge (side)
    CARPET_EDGE_N,
    CARPET_EDGE_E,
    CARPET_EDGE_S,
    THRONE  //!< royal golden throne
  };

  //! Data about individual tiles from a Terr.
  class Tile
  {
  public:
    Tile(tileType t = VOID);
    //!< Tile constructor. Makes a VOID Tile by default.
    void connectTile(dir d, Tile* t);
    //!< Specifies an adjacent Tile. Not reciprocal -- may be one-way!
    double getAngle();
    //!< Get the current angle of sprite rotation for the Tile’s art.
    SDL_RendererFlip getFlip();
    //!< Gets current flip status of Tile’s sprite.
    bool getIsPassable();
    //!< Returns true if the Tile may be entered by a Unit.
    Sprite* getSprite();
    //!< Returns the Sprite on the Tile. NULL if none.
    Tile* getTile(dir d);
    //!< Returns pointer to the adjacent tile in specified direction. May be NULL.
    tileType getType();
    //!< Returns the Tile’s type.
    int getX();
    //!< Returns the Tile’s x position on its Terr.
    int getY();
    //!< Returns the Tile’s y position on its Terr.
    bool isOccupied();
    //!< Returns true if the Tile has a Sprite on it.
    void setAngle(double a);
    //!< Sets the rotational angle of the Tile’s sprite.
    void setFlip(SDL_RendererFlip f);
    //!< Sets whether the Tile’s sprite is flipped.
    void setPos(int xPos, int yPos);
    //!< Sets where Tile thinks it is on a Terr.
    void setSprite(Sprite* u);
    //!< Sets the Sprite currently occupying the Tile (NULL for none).
    void setType(tileType t);
    //!< Sets what kind of tile it is.
  protected:
    int x;
    //!< X-position Tile believes it holds on a Terr.
    int y;
    //!< Y-position Tile believes it holds on a Terr.
    tileType type;
    //!< Which type of tile to draw.
    bool isPassable;
    //!< True if tile can be entered. May change later to enterable from each side.
    Sprite* sprite;
    //!< Sprite currently on the tile. NULL if none.
    Tile* E;
    //!< Adjacent Tile to the East/Right.
    Tile* N;
    //!< Adjacent Tile to the North/Up.
    Tile* S;
    //!< Adjacent Tile to the South/Down.
    Tile* W;
    //!< Adjacent Tile to the West/Left.
    double angle;
    //!< Angle at which Tile’s sprite is rotated
    SDL_RendererFlip flip;
    //!< Flip status of Tile’s sprite
  };

#endif