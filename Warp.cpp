// Samuel Weber

#include "Warp.h"


Warp::Warp(Tile* tile, string &nT, bool replaceTile)
{
  x = tile->getX();
  y = tile->getY();
  type = tile->getType();
  isPassable = tile->getIsPassable();
  sprite = tile->getSprite();
  E = tile->getTile(EAST);
  N = tile->getTile(NORTH);
  S = tile->getTile(SOUTH);
  W = tile->getTile(WEST);
  angle = tile->getAngle();
  flip = tile->getFlip();
  
  destTerr = nT;
  
  if (replaceTile)
  {
    if (E)
      E->connectTile(WEST, this);
    if (N)
      N->connectTile(SOUTH, this);
    if (S)
      S->connectTile(NORTH, this);
    if (W)
      W->connectTile(EAST, this);
    delete tile;
  }  // If I'm replacing a tile, tell the neighbors I'm him and then delete him.
}//Warp::Warp(Tile* tile, string &nT)


uint8_t Warp::getDestX()
{
  return destX;
}  // uint8_t Warp::getDestX()


uint8_t Warp::getDestY()
{
  return destY;
}  // uint8_t Warp::getDestY()


string Warp::getDestTerr()
{
  return destTerr;
}  // string Warp::getDestTerr()


bool Warp::isWarp()
{
  return true;
}  // bool Warp::isWarp()

