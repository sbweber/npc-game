// Samuel Weber

#include "Tile.h"


Tile::Tile(SDL_Texture* t, bool p, tileType tt, shared_ptr<Sprite> spr)
{
  x = 0;
  y = 0;
  tex = t;
  type = tt;
  angle = 0;
  flip = SDL_FLIP_NONE;
  isPassable = p;
  sprite = spr;
} // Tile::Tile(tileType t)


Tile::~Tile()
{
} // Tile::~Tile()


void Tile::clearTile()
{
  setSprite(nullptr);
} // void Tile::clearTile()


void Tile::connectTile(DIR d, shared_ptr<Tile> t)
{
  switch (d)
  {
    case DIR_NORTH:
      N = t;
      break;
    case DIR_SOUTH:
      S = t;
      break;
    case DIR_WEST:
      W = t;
      break;
    case DIR_EAST:
      E = t;
      break;
    default:
      break;
  } // set tile pointer for correct direction
} // void Tile::connectTile(dir d, shared_ptr<Tile> t)


const string Tile::enterTile()
{
  return "";
} // const string Tile::enterTile()


double Tile::getAngle()
{
  return angle;
} // double Tile::getAngle()


SDL_RendererFlip Tile::getFlip()
{
  return flip;
} // SDL_RendererFlip Tile::getFlip()


bool Tile::getIsPassable()
{
  return isPassable;
} // bool Tile::getIsPassable()


shared_ptr<Sprite> Tile::getSprite()
{
  return sprite;
} // shared_ptr<Sprite> Tile::getSprite()


SDL_Texture* Tile::getTex()
{
  return tex;
} // SDL_Texture* Tile::getTex()


shared_ptr<Tile> Tile::getTile(DIR d)
{
  switch (d)
  {
    case DIR_NORTH:
      return N.lock();
      break;
    case DIR_SOUTH:
      return S.lock();
      break;
    case DIR_WEST:
      return W.lock();
      break;
    case DIR_EAST:
      return E.lock();
      break;
    default:
      break;
  }
  return nullptr; // should be impossible to get here
} // shared_ptr<Tile> Tile::getTile(dir d)


tileType Tile::getType()
{
  return type;
} // tileType Tile::getType()


int Tile::getX()
{
  return x;
} // int Tile::getX()


int Tile::getY()
{
  return y;
} // int Tile::getY()


void Tile::setAngle(double a)
{
  angle = a;
} // void Tile::setAngle(double a)


void Tile::setFlip(SDL_RendererFlip f)
{
  flip = f;
} // void Tile::setFlip(SDL_RendererFlip f)


void Tile::setIsPassable(bool p)
{
  isPassable = p;
} // void Tile::setIsPassable(bool p)


void Tile::setPos(int xPos, int yPos)
{
  x = xPos;
  y = yPos;
} // void Tile::setPos(int xPos, int yPos)


void Tile::setSprite(shared_ptr<Sprite> spr)
{
  sprite = spr;
} // void Tile::setSprite(shared_ptr<Sprite> spr)


void Tile::setTex(SDL_Texture* t)
{
  tex = t;
} // void Tile::setTex(SDL_Texture* t)


void Tile::setType(tileType t)
{
  type = t;
} // void Tile::setType(tileType t)

