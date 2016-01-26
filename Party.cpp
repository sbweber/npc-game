// Samuel Weber

#include "Party.h"


Party::Party(SDL_Renderer *ren)
{
  for (int i = 0; i < 4; i++)
    active[i] = nullptr;
  passive.clear();
  if (ren)
    sprite = new Sprite(ren, "Hero.png", "Hero");
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


void Party::move(dir d, bool interrupt)
{
  if (interrupt)
    sprite->clearMoves();
  terr->moveSprite(sprite, d);
}  // void Party::move(dir d)


void Party::setLocation(int x, int y)
{
  terr->setSprite(sprite, terr->getTile(x, y));
}  // void Party::setLocation(int x, int y)


void Party::setLocation(Tile* tile)
{
  terr->setSprite(sprite, tile);
}  // void Party::setLocation(Tile* tile)


void Party::setSprite(SDL_Renderer *ren, const string &str)
{
  if (sprite)
    delete sprite;
  sprite = new Sprite(ren, str);
}  // void Party::setSprite(SDL_Renderer *ren, const string &str)


Tile* Party::tileClick(SDL_MouseButtonEvent &click)
{
  return terr->tileClick(click, sprite);
}  // Tile* Party::tileClick(SDL_MouseButtonEvent &click)

