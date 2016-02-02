// Samuel Weber

#include "Party.h"


Party::Party(SDL_Renderer *ren)
{
  for (int i = 0; i < 4; i++)
    active[i] = nullptr;
  passive.clear();
  if (ren)
    sprite.reset(new Sprite(ren, "Hero.png", "Hero"));
  terr.reset(new Terr(ren, ""));
}  // Party::Party(SDL_Renderer *ren)


Party::~Party()
{
}  // Party destructor


void Party::changeTerr(const string& newTerr)
{
  terr->loadMap(newTerr);
}  // void Party::changeTerr(string& newTerr)


SDL_Renderer* Party::getRen()
{
  return terr->getRen();
}  // SDL_Renderer* Party::getRen()


shared_ptr<Sprite> Party::getSprite()
{
  return sprite;
}  // shared_ptr<Sprite> Party::getSprite()


gameState Party::getState()
{
  return state;
}  // gameState Party::getState()


unique_ptr<Terr>& Party::getTerr()
{
  return terr;
}  // unique_ptr<Terr> Party::getTerr()


void Party::move(dir d, bool interrupt)
{
  if (interrupt)
    sprite->clearActs();
  terr->moveSprite(sprite, d);
}  // void Party::move(dir d)


void Party::setLocation(int x, int y)
{
  terr->setSprite(sprite, terr->getTile(x, y));
}  // void Party::setLocation(int x, int y)


void Party::setLocation(shared_ptr<Tile> tile)
{
  terr->setSprite(sprite, tile);
}  // void Party::setLocation(shared_ptr<Tile> tile)


void Party::setSprite(SDL_Renderer *ren, const string &str)
{
  sprite.reset(new Sprite(ren, str));
}  // void Party::setSprite(SDL_Renderer *ren, const string &str)


void Party::setState(gameState gs)
{
  state = gs;
}  // void Party::setState(gameState gs)


shared_ptr<Tile> Party::tileClick(SDL_MouseButtonEvent &click)
{
  return terr->tileClick(click, sprite);
}  // shared_ptr<Tile> Party::tileClick(SDL_MouseButtonEvent &click)

