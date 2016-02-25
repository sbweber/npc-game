// Samuel Weber

#include "Warp.h"


Warp::Warp(shared_ptr<Tile> tile, const string &dT, int dX, int dY) : Tile(tile->getTex())
{
  x = tile->getX();
  y = tile->getY();
  tex = tile->getTex();
  type = tile->getType();
  isPassable = tile->getIsPassable();
  E = tile->getTile(DIR_EAST);
  N = tile->getTile(DIR_NORTH);
  S = tile->getTile(DIR_SOUTH);
  W = tile->getTile(DIR_WEST);
  angle = tile->getAngle();
  flip = tile->getFlip();
  
  destTerr = dT;
  destX = dX;
  destY = dY;
}  // Warp::Warp(shared_ptr<Tile> tile, string &nT)


const string Warp::enterTile()
{
  return "LOAD-STATE_MAP: " + destTerr + " " + to_string(destX) + " " + to_string(destY);
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

