// Samuel Weber

#ifndef TILE_H
  #define TILE_H

  #include "Globals.h"
  #include "Sprite.h"

  class Sprite;

  enum tileType
  {
    VOID,  // black, impassable
    OPEN,  // white, passable
    CARPET_CORE,  // fancy carpet, non-edge piece
    CARPET_CORNER_SW,  // fancy carpet, two edges (corner)
    CARPET_CORNER_NW,
    CARPET_CORNER_NE,
    CARPET_CORNER_SE,
    CARPET_EDGE_W,  // fancy carpet, one edge (side)
    CARPET_EDGE_N,
    CARPET_EDGE_E,
    CARPET_EDGE_S,
    THRONE  // royal golden throne
  };  // describes the Tiles.png spritesheet in column-major order

  class Tile
  {
  protected:
    int x;  // tiles now know where they are in a terrain.
    int y;
    tileType type;
    bool isPassable;
            // tile can be entered. May change later to enterable from each side.
    Sprite* sprite;  // Sprite currently on the tile
    Tile* E;
    Tile* N;  // adjacent tiles
    Tile* S;  // to the North, South, West, and East
    Tile* W;  // aka Up, Down, Left, Right
    double angle;  // for sprite rotation
    SDL_RendererFlip flip;  // for sprite flipping
  public:
    Tile(tileType t = VOID);
    void connectTile(dir d, Tile* t);
    double getAngle();
    SDL_RendererFlip getFlip();
    bool getIsPassable();
    Sprite* getSprite();
    Tile* getTile(dir d);  // gets adjacent tile
    tileType getType();
    int getX();
    int getY();
    bool isOccupied();
    void setAngle(double a);
    void setFlip(SDL_RendererFlip f);
    void setPos(int xPos, int yPos);
    void setSprite(Sprite* u);
    void setType(tileType t);
  };

#endif