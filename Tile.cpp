// Samuel Weber

#include "Tile.h"


Tile::Tile(tileType t)
{
  x = 0;
  y = 0;
  setType(t);
  sprite = nullptr;
  E = nullptr;
  N = nullptr;
  S = nullptr;
  W = nullptr;
  angle = 0;
  flip = SDL_FLIP_NONE;
}  // Tile::Tile(tileType t)


Tile::~Tile()
{
  if (E && E->W == this)
    E->W = nullptr;
  if (N && N->S == this)
    N->S = nullptr;
  if (S && S->N == this)
    S->N = nullptr;
  if (W && W->E == this)
    W->E = nullptr;
  if (sprite && sprite->getPos() == this)
    sprite->setTile(nullptr);
}  // Tile::~Tile()


void Tile::connectTile(dir d, Tile* t)
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
}  // void Tile::connectTile(dir d, Tile* t)


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


Sprite* Tile::getSprite()
{
  return sprite;
}  // Sprite* Tile::getSprite()


Tile* Tile::getTile(dir d)
{
  switch (d)
  {
  case NORTH:
    return N;
    break;
  case SOUTH:
    return S;
    break;
  case WEST:
    return W;
    break;
  case EAST:
    return E;
    break;
  default:
    break;
  }
  return nullptr;  // should be impossible to get here
}  // Tile* Tile::getTile(dir d)


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


bool Tile::isOccupied()
{
  if (sprite)
    return true;
  return false;
}  // bool Tile::isOccupied()


bool Tile::isWarp()
{
  return false;
}  // bool Tile::isWarp()


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


void Tile::setSprite(Sprite* u)
{
  sprite = u;
  // tile thinks the unit is there, but unit may not know it's on that tile!
}  // void Tile::setUnit(Unit* u)


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

