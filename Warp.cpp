// Samuel Weber

#include "Warp.h"


Warp::Warp(shared_ptr<Tile> tile, const string &dT, int dX, int dY)
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
}  // Warp::Warp(shared_ptr<Tile> tile, string &nT)


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

