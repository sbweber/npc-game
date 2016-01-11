// Samuel Weber

#include "Party.h"


Party::Party(SDL_Renderer *ren)
{
  for (int i = 0; i < 4; i++)
    active[i] = NULL;
  passive.clear();
  if (ren)
    sprite = new Sprite(ren, "Hero.png");
  terr = new Terr("");
}  // Party::Party(SDL_Renderer *ren)


Party::~Party()
{
  for (int i = 0; i < 4; i++)
    delete active[i];
  delete terr;
}  // Party destructor


void Party::changeTerr(const string& newTerr)
{
  terr->loadMap(newTerr);
}  // void Party::changeTerr(string& newTerr)


Sprite* Party::getSprite()
{
  return sprite;
}  // Sprite* Party::getSprite()


Terr* Party::getTerr()
{
  return terr;
}  // Terr* Party::getTerr()


void Party::move(dir d)
{
  sprite->move(d);
  if (sprite->getPos()->isWarp())
  {
    
  }  // If the new Tile is a Warp, load the new Terr.
}  // void Party::move(dir d)


void Party::setLocation(uint8_t x, uint8_t y)
{
  sprite->setTile(terr->getTile(x, y));
}  // void Party::setLocation(uint8_t x, uint8_t y)


void Party::setSprite(SDL_Renderer *ren, const string &str)
{
  if (sprite)
    delete sprite;
  sprite = new Sprite(ren, str);
}  // void Party::setSprite(SDL_Renderer *ren, const string &str)

