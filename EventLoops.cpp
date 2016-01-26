// Samuel Weber

#include "EventLoops.h"

bool loopAnyState(SDL_Event &e, Party* party, Sprite *npc, gameState &state)
{
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == stateBattle)
      state = BATTLE;  // debug command startbattle
    if (e.key.keysym == stateMap1)
    {
      party->changeTerr("Map0,0.txt");
      state = MAP;
      party->setLocation(4, 3);
      party->getTerr()->setSprite(npc, party->getTerr()->getTile(5, 4));
    }  // debug command map1
    if (e.key.keysym == stateMap2)
    {
      party->changeTerr("Map0,1.txt");
      state = MAP;
      party->setLocation(1, 1);
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
  case SDL_KEYDOWN:
    break;
  default:
    break;
  }
  return false;
}  // bool loopBattle(SDL_Event &e)


bool loopMap(SDL_Renderer *ren, SDL_Texture* tiles, SDL_Event &e, Party* party)
{
  Tile* tile;
  dir d = NORTH;
  while(drawMap(ren, tiles, party));
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == dirUp)
      party->move(NORTH);
    if (e.key.keysym == dirLeft)
      party->move(WEST);
    if (e.key.keysym == dirDown)
      party->move(SOUTH);
    if (e.key.keysym == dirRight)
      party->move(EAST);
    if (e.key.keysym == interact)
      party->getTerr()->interactSprite(party->getSprite());
    break;
  case SDL_MOUSEBUTTONDOWN:
    tile = party->tileClick(e.button);
    if (tile && tile->getIsPassable() && !party->getTerr()->isOccupied(tile))
    {
      party->getTerr()->findPath(party->getTerr()->getTile(party->getSprite()),
              tile, party->getSprite());
    }  // If tile can be legally entered, warp to it
    break;
  default:
    d = party->getSprite()->popMove();
    if (d != UNDEFINED_DIRECTION)
    {
      party->move(d, false);
      SDL_Event* wait = new SDL_Event();
      SDL_PushEvent(wait);  // push empty event to cause immediate state update
    }
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
  case SDL_KEYDOWN:
    break;
  default:
    break;
  }
  state = TITLE;
  return false;
}  // bool loopRebind()


bool loopTitle(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font,
        gameState &state, Party *party, Sprite *npc)
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
      party->changeTerr("Map0,0.txt");
      state = MAP;
      party->setLocation(6, 10);
      party->getTerr()->setSprite(npc, party->getTerr()->getTile(5, 4));
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
  delete toGame;
  return false;
}  // bool loopTitle()


bool mainLoop(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font,
        SDL_Texture *tiles, Party* party, Sprite *npc,
        gameState &state)
{
  bool quit = false;
  quit = loopAnyState(e, party, npc, state);
  if (quit)
    return true;
  switch (state)
  {
  case BATTLE:
    quit = loopBattle(e);
    break;
  case MAP:
    quit = loopMap(ren, tiles, e, party);
    break;
  case REBIND:
    quit = loopRebind(ren, e, font, state);
    break;
  case TITLE:
    quit = loopTitle(ren, e, font, state, party, npc);
    break;
  default:
    break;
  }  // switch (state)
  return quit;
}  // bool mainLoop()

