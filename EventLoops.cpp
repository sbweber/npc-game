// Samuel Weber

#include "EventLoops.h"

bool loopAnyState(SDL_Event &e, unique_ptr<Party> &party)
{
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == stateBattle)
      party->setState(BATTLE);  // debug command startbattle
    if (e.key.keysym == stateMap1)
    {
      party->changeTerr("0,0.txt");
      party->setState(MAP);
      party->setLocation(4, 3);
    }  // debug command map1
    if (e.key.keysym == stateMap2)
    {
      party->changeTerr("0,1.txt");
      party->setState(MAP);
      party->setLocation(1, 1);
    }  // debug command map2
    if (e.key.keysym == stateRebind)
      party->setState(REBIND);  // debug command go to rebind menu
    if (e.key.keysym == stateQuit)
      return true;
    break;
  default:
    break;
  }  // switch (event type)
  return false;
}  // bool loopAnyState()


bool loopBattle(SDL_Event &e, TTF_Font* font, unique_ptr<Party> &party)
{
  drawBattle(party->getRen(), font);
  switch (e.type)
  {
  case SDL_KEYDOWN:
    break;
  case SDL_MOUSEBUTTONDOWN:
    break;
  default:
    break;
  }
  return false;
}  // bool loopBattle(SDL_Event &e)


bool loopMap(SDL_Event &e, unique_ptr<Party> &party)
{
  shared_ptr<Tile> tile;
  action act(UNDEFINED_DIRECTION, BAD_ACTION);
  SDL_Event* wait = new SDL_Event();
  while(drawMap(party));
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
    if (tile && tile->getIsPassable())
    {
      party->getTerr()->findPath(party->getTerr()->getTile(party->getSprite()),
              tile, party->getSprite());
    }  // If tile can be legally entered, path to it
    break;
  default:
    act = party->getSprite()->popAct();
    switch (get<1>(act))
    {
    case MOVE:
      party->move(get<0>(act), false);
      SDL_PushEvent(wait);  // push empty event to cause immediate state update
      break;
    case INTERACT:
      party->getTerr()->interactSprite(party->getSprite());
      SDL_PushEvent(wait);  // push empty event to cause immediate state update
      break;
    case BAD_ACTION:
    default:
      break;
    }
    break;
  }
  return false;
}  // bool loopMap(SDL_Event &e, unique_ptr<Party> &party)


bool loopRebind(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font)
{
  drawRebind(ren, font);
  switch (e.type)
  {
  case SDL_KEYDOWN:
    break;
  default:
    break;
  }
  return false;
}  // bool loopRebind()


bool loopTitle(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party)
{
  vector<unique_ptr<Button> > buttons;
  buttons.emplace_back(new Button(party->getRen(), "Button.png",
          SCREEN_WIDTH / 2 - 120, 300, 240, 100, font, "To Game"));
  buttons.emplace_back(new Button(party->getRen(), "Button.png",
          SCREEN_WIDTH / 2 - 120, 450, 240, 100, font, "Quit"));
  drawTitle(party->getRen(), buttons);
  switch (e.type)
  {
  case SDL_KEYDOWN:
    // TODO: use arrow keys to select button; enter/interact to depress button
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (buttons[0]->buttonClick(party->getRen(), e.button))
    {
      party->changeTerr("0,0.txt");
      party->setState(MAP);
      party->setLocation(6, 10);
      SDL_Event* wait = new SDL_Event();
      SDL_PushEvent(wait);  // push empty event to cause immediate state update
    }
    else if (buttons[1]->buttonClick(party->getRen(), e.button))
      return true;
    // click on button to depress button
    break;
  case SDL_MOUSEBUTTONUP:
    // TODO: follow through on buttonclick iff buttonup is also in button area
    // This functionality actually appropriate as part of buttonClick()
    break;
  case SDL_MOUSEMOTION:
    // TODO: mouseover button to select button 
    break;
  default:
    break;
  }
  return false;
}  // bool loopTitle()


bool mainLoop(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party)
{
  bool quit = false;
  quit = loopAnyState(e, party);
//  drawScreen(font, party);
  if (quit)
    return true;
  switch (party->getState())
  {
  case BATTLE:
    quit = loopBattle(e, font, party);
    break;
  case MAP:
    quit = loopMap(e, party);
    break;
  case REBIND:
    quit = loopRebind(party->getRen(), e, font);
    party->setState(TITLE);
    break;
  case TITLE:
    quit = loopTitle(e, font, party);
    break;
  default:
    break;
  }  // switch (state)
  return quit;
}  // bool mainLoop()

