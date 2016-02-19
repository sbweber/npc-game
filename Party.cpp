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
  if (i >= 0 && i < 4)
    return active[i];
  return nullptr;
}  // shared_ptr<Unit> Party::getUnit(int i)


void Party::setSprite(SDL_Renderer *ren, const string &str)
{
  sprite.reset(new Sprite(ren, 0, 0, str));
}  // void Party::setSprite(SDL_Renderer *ren, const string &str)

