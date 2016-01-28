// Samuel Weber

#include "Tile.h"


Tile::Tile(tileType t)
{
  x = 0;
  y = 0;
  setType(t);
  angle = 0;
  flip = SDL_FLIP_NONE;
}  // Tile::Tile(tileType t)


Tile::~Tile()
{
}  // Tile::~Tile()


void Tile::connectTile(dir d, shared_ptr<Tile> t)
{
  switch (d)
  {
  case NORTH:
    N = t;
    break;
  case SOUTH:
    S = t;
    break;
  case WEST:
    W = t;
    break;
  case EAST:
    E = t;
    break;
  default:
    break;
  }  // set tile pointer for correct direction
}  // void Tile::connectTile(dir d, shared_ptr<Tile> t)


const string Tile::enterTile()
{
  return "";
}  // const string Tile::enterTile()


double Tile::getAngle()
{
  return angle;
}  // double Tile::getAngle()


SDL_RendererFlip Tile::getFlip()
{
  return flip;
}  // SDL_RendererFlip Tile::getFlip()


bool Tile::getIsPassable()
{
  return isPassable;
}  // bool Tile::getIsPassable()


shared_ptr<Tile> Tile::getTile(dir d)
{
  switch (d)
  {
  case NORTH:
    return N.lock();
    break;
  case SOUTH:
    return S.lock();
    break;
  case WEST:
    return W.lock();
    break;
  case EAST:
    return E.lock();
    break;
  default:
    break;
  }
  return nullptr;  // should be impossible to get here
}  // shared_ptr<Tile> Tile::getTile(dir d)


tileType Tile::getType()
{
  return type;
}  // tileType Tile::getType()


int Tile::getX()
{
  return x;
}  // int Tile::getX()

int Tile::getY()
{
  return y;
}  // int Tile::getY()


void Tile::setAngle(double a)
{
  angle = a;
}  // void Tile::setAngle(double a)


void Tile::setFlip(SDL_RendererFlip f)
{
  flip = f;
}  // void Tile::setFlip(SDL_RendererFlip f)


void Tile::setPos(int xPos, int yPos)
{
  x = xPos;
  y = yPos;
}  // void Tile::setPos(int xPos, int yPos)


void Tile::setType(tileType t)
{
  type = t;
  // move the following into a resource file with information about the
  // various tile types eventually
  if (type == VOID)
    isPassable = false;
  else
    isPassable = true;
}  // void Tile::setType(tileType t)

