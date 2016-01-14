// Samuel Weber

#include "Sprite.h"

Sprite::Sprite(SDL_Renderer *ren, const string &spriteFile)
{
  spriteSheet = loadTexture(spriteFile, ren);
  sprite = DOWN;  // default all sprites as facing down
  pos = nullptr;  // default all Units as being off the board
  facing = SOUTH;
}  // Sprite::Sprite(SDL_Renderer *ren, const string &spriteFile)

Sprite::~Sprite()
{
  SDL_DestroyTexture(spriteSheet);
}  // Sprite::~Sprite()

Tile* Sprite::getPos()
{
  return pos;
}  // Tile* Sprite::getPos()

spriteType Sprite::getSprite()
{
  return sprite;
}  // SpriteType Sprite::getSprite()

SDL_Texture* Sprite::getSpriteSheet()
{
  return spriteSheet;
}  // SDL_Texture* Sprite::getSpriteSheet()

void Sprite::interact()
{
  if (!pos)
    return;  // break out of function and do nothing if unit is not on the map
  if (pos->getTile(facing) && pos->getTile(facing)->getSprite())
    if (pos->getTile(facing)->getSprite()->type == "test")
    {
      pos->getTile(facing)->getSprite()->setTile(nullptr);
      // current npc test type: kills self on interaction. Test successful.
      pos->getTile(facing)->setSprite(nullptr);
    }
    // do nothing unless there's a tile in the correct direction with a unit
    // on it (can only ineract with units right now)
}  // void Sprite::interact(dir d)

void Sprite::move(dir d)
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
  }  // always change sprite direction
  if (pos->getTile(d) && pos->getTile(d)->getIsPassable() &&
          !pos->getTile(d)->getSprite())
  {
    pos->setSprite(nullptr);
    pos = pos->getTile(d);
    pos->setSprite(this);
  }  // move if target tile exists, can be entered, doesn't have a unit in it
}  // void Sprite::move(dir d)

void Sprite::setSprite(spriteType st)
{
  sprite = st;
}  // void Sprite::setSprite()

void Sprite::setTile(Tile* tile)
{
  pos = tile;
  if (pos)
    pos->setSprite(this);  // places unit even if tile impassible or occupied!
}  // void Sprite::setTile(Tile* tile)

void Sprite::setType(const string &str)
{
  type = str;
}  // void Sprite::setType(string str)

