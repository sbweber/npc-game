// Samuel Weber

#include "Warp.h"


Warp::Warp(shared_ptr<Tile> tile, const string &dT, int dX, int dY,
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
  /*
  if (replaceTile)
  {
    if (E.lock() && (E.lock()->getTile(WEST) == tile))
      E.lock()->connectTile(WEST, shared_ptr<Tile>(this));
    if (N.lock() && (N.lock()->getTile(SOUTH) == tile))
      N.lock()->connectTile(SOUTH, shared_ptr<Tile>(this));
    if (S.lock() && (S.lock()->getTile(NORTH) == tile))
      S.lock()->connectTile(NORTH, shared_ptr<Tile>(this));
    if (W.lock() && (W.lock()->getTile(EAST) == tile))
      W.lock()->connectTile(EAST, shared_ptr<Tile>(this));
  }  // If I'm replacing a tile, tell the neighbors I'm him.
  */
  E.lock()->connectTile(WEST, shared_ptr<Tile>(this));
  N.lock()->connectTile(SOUTH, shared_ptr<Tile>(this));
  S.lock()->connectTile(NORTH, shared_ptr<Tile>(this));
  W.lock()->connectTile(EAST, shared_ptr<Tile>(this));
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

