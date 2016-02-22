// Samuel Weber

#include "Party.h"


Party::Party(SDL_Renderer *ren)
{
  if (!ren)
    quit("Renderer not found!", 3);
  for (int i = 0; i < 4; i++)
    active[i] = nullptr;
  active[0].reset(new Unit());
  passive.clear();
  sprite.reset(new Sprite(ren, 0, 0, "Hero.png", "Roland", "Hero"));
  sprite->clearActs();
  moveButtonHeld = false;
}  // Party::Party(SDL_Renderer *ren)


Party::~Party()
{
}  // Party destructor


dir Party::getMoveButtonDir()
{
  return moveButtonDir;
}  // bool Party::getMoveButtonDir()


bool Party::getMoveButtonHeld()
{
  return moveButtonHeld;
}  // bool Party::getMoveButtonHeld()


shared_ptr<Sprite> Party::getSprite()
{
  return sprite;
}  // shared_ptr<Sprite> Party::getSprite()


shared_ptr<Unit> Party::getUnit(int i)
{
  if (i >= 0 && i < 4)
    return active[i];
  return nullptr;
}  // shared_ptr<Unit> Party::getUnit(int i)


void Party::move(dir moveDir)
{
  if (moveDir == UNDEFINED_DIRECTION)
    moveButtonHeld = false;
  else
    moveButtonHeld = true;
  moveButtonDir = moveDir;
}  // void Party::setMoveButtonHeld(bool mbh)


void Party::setSprite(SDL_Renderer *ren, const string &str)
{
  sprite.reset(new Sprite(ren, 0, 0, str));
}  // void Party::setSprite(SDL_Renderer *ren, const string &str)

