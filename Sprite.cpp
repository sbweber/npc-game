// Samuel Weber

#include "Sprite.h"


Sprite::Sprite(SDL_Renderer *ren, const string &spriteFile)
{
  spriteSheet = loadTexture(spriteFile, ren);
  sprite = DOWN;  // default all sprites as facing down
  facing = SOUTH;
  spline = 0;
}  // Sprite::Sprite(SDL_Renderer *ren, const string &spriteFile)


Sprite::~Sprite()
{
  SDL_DestroyTexture(spriteSheet);
}  // Sprite::~Sprite()


bool Sprite::decSpline()
{
  if (spline > 0)
  {
    if (facing == NORTH || facing == SOUTH)
      spline -= (TILE_HEIGHT / NUM_FRAMES_SPLINE);
    else
      spline -= (TILE_WIDTH / NUM_FRAMES_SPLINE);
    if (spline < 0)
      spline = 0;
    return true;
  }
  return false;
}  // void Sprite::decSpline()


void Sprite::changeDir(dir d)
{
  facing = d;
  switch (d)
  {
  case NORTH:
    sprite = UP;
    break;
  case SOUTH:
    sprite = DOWN;
    break;
  case WEST:
    sprite = LEFT;
    break;
  case EAST:
    sprite = RIGHT;
    break;
  default:
    break;  // default should be impossible
  }  // changes Sprite direction
}  // void Sprite::changeDir(dir d)


dir Sprite::getFacing()
{
  return facing;
}  // dir Sprite::getFacing()


int Sprite::getSpline()
{
  return spline;
}  // int Sprite::getSpline()


spriteType Sprite::getSprite()
{
  return sprite;
}  // SpriteType Sprite::getSprite()


SDL_Texture* Sprite::getSpriteSheet()
{
  return spriteSheet;
}  // SDL_Texture* Sprite::getSpriteSheet()


void Sprite::interact(Sprite* target)
{
  if (target->type == "test")
  {
    // TODO: Do Something. Previous interaction: Kill target.
  }
}  // void Sprite::interact(Sprite* target)


void Sprite::move(dir d)
{
  changeDir(d);

  if (d == NORTH || d == SOUTH)
    spline = TILE_HEIGHT;
  else
    spline = TILE_WIDTH;
  // TODO: FIX HERE. Splining will be relevant somewhere, only do it if the sprite actually moved. Sprites should no longer move themselves; tell the Terr to move a Sprite.
}  // void Sprite::move(dir d)


void Sprite::setSpline(int s)
{
  spline = s;
}  // void Sprite::setSpline(int s)


void Sprite::setSprite(spriteType st)
{
  sprite = st;
}  // void Sprite::setSprite()


void Sprite::setType(const string &str)
{
  type = str;
}  // void Sprite::setType(string str)

