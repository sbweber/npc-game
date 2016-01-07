// Samuel Weber

#include "EventLoops.h"

bool loopAnyState(SDL_Renderer *ren, SDL_Event &e, Terr *terr, Party* party,
        Sprite *npc, gameState &state)
{
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == stateBattle)
      state = BATTLE;  // debug command startbattle
    if (e.key.keysym == stateMap1)
    {
      delete terr;
      terr = new Terr("Map1.txt");
      state = MAP;
      party->getSprite()->setTile(terr->getTile(4, 3));
      npc->setTile(terr->getTile(5, 4));
    }  // debug command map1
    if (e.key.keysym == stateMap2)
    {
      delete terr;
      terr = new Terr("Map2.txt");
      state = MAP;
      party->getSprite()->setTile(terr->getTile(1, 1));
    }  // debug command map2
    if (e.key.keysym == stateRebind)
      state = REBIND;  // debug command go to rebind menu
    if (e.key.keysym == stateQuit)
      return true;
    break;
  default:
    break;
  }  // switch (event type)
  return false;
}  // bool loopAnyState()


bool loopBattle(SDL_Event &e)
{
  switch (e.type)
  {
  default:
    break;
  }
  return false;
}  // bool loopBattle(SDL_Event &e)


bool loopMap(SDL_Renderer *ren, SDL_Texture* tiles, Terr* terr, SDL_Event &e,
        Party* party)
{
  drawMap(ren, tiles, terr, party->getSprite());
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == dirUp)
      party->getSprite()->move(NORTH);
    if (e.key.keysym == dirLeft)
      party->getSprite()->move(WEST);
    if (e.key.keysym == dirDown)
      party->getSprite()->move(SOUTH);
    if (e.key.keysym == dirRight)
      party->getSprite()->move(EAST);
    if (e.key.keysym == interact)
      party->getSprite()->interact();
    break;
  default:
    break;
  }
  return false;
}  // bool loopMap(SDL_Event &e, Party* party)


bool loopRebind(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font,
        gameState &state)
{
  drawRebind(ren, font);
  switch (e.type)
  {
  default:
    break;
  }
  state = TITLE;
  return false;
}  // bool loopRebind()


bool loopTitle(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font, Terr *terr,
        gameState &state, Party *party, Sprite *npc, vector<Button *> buttons)
{
  Button *toGame = new Button(ren, "Button.png",
          SCREEN_WIDTH / 2 - 120, 300, 240, 100, font, "To Game");
  drawTitle(ren, toGame);
  switch (e.type)
  {
  case SDL_KEYDOWN:
    // TODO: use arrow keys to select button; enter/interact to depress button
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (toGame->buttonClick(e.button))
    {
      delete terr;
      terr = new Terr("Map1.txt");
      state = MAP;
      party->getSprite()->setTile(terr->getTile(4, 3));
      npc->setTile(terr->getTile(5, 4));
      SDL_Event* wait = new SDL_Event();
      SDL_PushEvent(wait);  // push empty event to cause immediate state update
    }
    // click on button to depress button
    break;
  case SDL_MOUSEBUTTONUP:
    // TODO: follow through on buttonclick iff buttonup is also in button area
    break;
  case SDL_MOUSEMOTION:
    // TODO: mouseover button to select button 
    break;
  default:
    break;
  }
  return false;
}  // bool loopTitle()


bool mainLoop(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font,
        SDL_Texture *tiles, Terr *terr, Party* party, Sprite *npc,
        gameState &state, vector<Button *> buttons)
{
  bool quit = false;
  quit = loopAnyState(ren, e, terr, party, npc, state);
  if (quit)
    return true;
  switch (state)
  {
  case BATTLE:
    quit = loopBattle(e);
    break;
  case MAP:
    quit = loopMap(ren, tiles, terr, e, party);
    break;
  case REBIND:
    quit = loopRebind(ren, e, font, state);
    break;
  case TITLE:
    quit = loopTitle(ren, e, font, terr, state, party, npc, buttons);
    break;
  default:
    break;
  }  // switch (state)
  return quit;
}  // bool mainLoop()

