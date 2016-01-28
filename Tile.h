// Samuel Weber

#ifndef TILE_H
  #define TILE_H

  #include "Globals.h"


  //! Describes the Tiles.png spritesheet in column-major order.
  enum tileType
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
    virtual ~Tile();
    //!< Virtual Tile destructor.
    void connectTile(dir d, shared_ptr<Tile> t);
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
    shared_ptr<Tile> getTile(dir d);
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
    void setPos(int xPos, int yPos);
    //!< Sets where Tile thinks it is on a Terr.
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
    //!< True if tile can be entered.
    //!< May change later to enterable from each side.
    weak_ptr<Tile> E;
    //!< Adjacent Tile to the East/Right.
    weak_ptr<Tile> N;
    //!< Adjacent Tile to the North/Up.
    weak_ptr<Tile> S;
    //!< Adjacent Tile to the South/Down.
    weak_ptr<Tile> W;
    //!< Adjacent Tile to the West/Left.
    double angle;
    //!< Angle at which Tile’s sprite is rotated
    SDL_RendererFlip flip;
    //!< Flip status of Tile’s sprite
  };

#endif