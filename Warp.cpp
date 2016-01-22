// Samuel Weber

#include "Warp.h"


Warp::Warp(Tile* tile, const string &dT, int dX, int dY,
            bool replaceTile)
{
  x = tile->getX();
  y = tile->getY();
  type = tile->getType();
  isPassable = tile->getIsPassable();
  E = tile->getTile(EAST);
  N = tile->getTile(NORTH);
  S = tile->getTile(SOUTH);
  W = tile->getTile(WEST);
  angle = tile->getAngle();
  flip = tile->getFlip();
  
  destTerr = dT;
  destX = dX;
  destY = dY;
  
  if (replaceTile)
  {
    if (E && E->getTile(WEST) == tile)
      E->connectTile(WEST, this);
    if (N && N->getTile(SOUTH) == tile)
      N->connectTile(SOUTH, this);
    if (S && S->getTile(NORTH) == tile)
      S->connectTile(NORTH, this);
    if (W && W->getTile(EAST) == tile)
      W->connectTile(EAST, this);
    delete tile;
  }  // If I'm replacing a tile, tell the neighbors I'm him, then delete him.
}  // Warp::Warp(Tile* tile, string &nT)


const string Warp::enterTile()
{
  return "LOAD-MAP: " + destTerr + " " + to_string(destX) + " " + to_string(destY);
}  // const string Warp::enterTile()


int Warp::getDestX()
{
  return destX;
}  // int Warp::getDestX()


int Warp::getDestY()
{
  return destY;
}  // int Warp::getDestY()


string Warp::getDestTerr()
{
  return destTerr;
}  // string Warp::getDestTerr()

