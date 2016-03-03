// Samuel Weber

#include "Party.h"


Party::Party(SDL_Renderer *ren)
{
  if (!ren)
    quit("Renderer not found!", 3);
  active.emplace_back(new Unit("Roland", "King", 10));
  active.emplace_back(new Unit("Alexandrite", "Shopkeeper", 5));
  passive.clear();
  sprite.reset(new Sprite(ren, 0, 0, "Hero.png", 0, "Roland", "Hero"));
  sprite->clearActs();
  gold = 100;
  debt = 100;
}  // Party::Party(SDL_Renderer *ren)


Party::~Party()
{
}  // Party destructor


unsigned long long Party::addDebt(unsigned long long val)
{
  if ((ULLONG_MAX - val) < debt)
    debt = ULLONG_MAX;
  else
    debt += val;
  return debt;
}  // unsigned long long Party::addDebt(unsigned long long val)


unsigned long long Party::getDebt()
{
  return debt;
}  // unsigned long long Party::getDebt()


unsigned long Party::getGold()
{
  return gold;
}  // unsigned long Party::getGold()


unsigned long long Party::getScore()
{
  return (gold - debt);
}  // unsigned long long Party::getScore()


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


unsigned long Party::transactGold(long val)
{
  if (val > 0)
    if ((ULONG_MAX - val) < gold)
      gold = ULONG_MAX;
    else
      gold += val;
  else
    gold -= val;
  return gold;
}  // unsigned long Party::transactGold(long val)

