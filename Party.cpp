//Samuel Weber

#include "Party.h"

Party::Party()
{
  for (int i = 0; i < 4; i++)
    active[i] = NULL;
  passive.clear();
  sprite = NULL;
}//default Party constructor

Party::Party(SDL_Renderer *ren)
{
  for (int i = 0; i < 4; i++)
    active[i] = NULL;
  passive.clear();
  sprite = new Sprite(ren, "Hero.png");
}//Party::Party(SDL_Renderer *ren)

Party::~Party()
{
  for (int i = 0; i < 4; i++)
    delete active[i];
}//Party destructor

Sprite* Party::getSprite()
{
  return sprite;
}//Sprite* Party::getSprite()

void Party::setSprite(SDL_Renderer *ren, const string &str)
{
  if (sprite)
    delete sprite;
  sprite = new Sprite(ren, str);
}//void Party::setSprite(SDL_Renderer *ren, const string &str)

