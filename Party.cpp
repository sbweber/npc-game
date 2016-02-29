// Samuel Weber

#include "Party.h"


Party::Party(SDL_Renderer *ren)
{
  if (!ren)
    quit("Renderer not found!", 3);
  active.emplace_back(new Unit("Roland", "Hero", 10));
  passive.clear();
  sprite.reset(new Sprite(ren, 0, 0, "Hero.png", 0, "Roland", "Hero"));
  sprite->clearActs();
}  // Party::Party(SDL_Renderer *ren)


Party::~Party()
{
}  // Party destructor


shared_ptr<Sprite> Party::getSprite()
{
  return sprite;
}  // shared_ptr<Sprite> Party::getSprite()


shared_ptr<Unit> Party::getUnit(int i)
{
  if (i < 0)
    return nullptr;
  unsigned int j = i;
  if (j < active.size())
    return active[j];
  return nullptr;
}  // shared_ptr<Unit> Party::getUnit(int i)


vector<shared_ptr<Unit> >& Party::getUnits()
{
  return active;
}  // vector<shared_ptr<Unit> >& Party::getUnits()


bool Party::keepMoving()
{
  if ((get<1>(sprite->topAct()) == ACT_MOVE) && (sprite->getQSize() == 1))
    return true;
  return false;
}  // void Party::keepMoving()


void Party::move(dir moveDir)
{
  sprite->pushAct(action(moveDir, ACT_MOVE));
}  // void Party::setMoveButtonHeld(bool mbh)


void Party::setSprite(SDL_Renderer *ren, const string &str)
{
  sprite.reset(new Sprite(ren, 0, 0, str, 0));
}  // void Party::setSprite(SDL_Renderer *ren, const string &str)

