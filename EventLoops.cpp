// Samuel Weber

#include "EventLoops.h"

void loopAnyState(SDL_Event &e, unique_ptr<Party> &party)
{
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == stateBattle)
      party->setState(BATTLE);  // debug command startbattle
    else if (e.key.keysym == stateMap1)
    {
      party->changeTerr("0,0.txt");
      party->setState(MAP);
      party->setLocation(4, 3);
    }  // debug command map1
    else if (e.key.keysym == stateMap2)
    {
      party->changeTerr("0,1.txt");
      party->setState(MAP);
      party->setLocation(1, 1);
    }  // debug command map2
    else if (e.key.keysym == stateRebind)
      party->setState(REBIND);  // debug command go to rebind menu
    else if (e.key.keysym == stateTitle)
      party->setState(TITLE);
    else if (e.key.keysym == stateQuit)
      eventQuit();
    break;
  case SDL_QUIT:
    SDL_FlushEvents(0, UINT32_MAX);
    eventQuit();
    break;  // It should be impossible to be here
  default:
    break;
  }  // switch (event type)
}  // void loopAnyState()


void loopBattle(SDL_Event &e, TTF_Font* font, unique_ptr<Party> &party)
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
}  // void loopBattle(SDL_Event &e)


void loopMap(SDL_Event &e, unique_ptr<Party> &party)
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
      party->setState(party->getTerr()->interactSprite(party->getSprite()));
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
      party->setState(party->getTerr()->interactSprite(party->getSprite()));
      SDL_PushEvent(wait);  // push empty event to cause immediate state update
      break;
    case BAD_ACTION:
    default:
      break;
    }
    break;
  }
}  // void loopMap(SDL_Event &e, unique_ptr<Party> &party)


void loopRebind(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font)
{
  drawRebind(ren, font);
  switch (e.type)
  {
  case SDL_KEYDOWN:
    break;
  default:
    break;
  }
}  // void loopRebind()


void loopTitle(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party)
{
  vector<unique_ptr<Button> > buttons;
  int x, y;
  buttons.emplace_back(new Button(party->getRen(), "Button.png",
          SCREEN_WIDTH / 2 - 120, 300, 240, 100, font, "To Game"));
  buttons.emplace_back(new Button(party->getRen(), "Button.png",
          SCREEN_WIDTH / 2 - 120, 450, 240, 100, font, "Quit"));
  SDL_GetMouseState(&x, &y);
  drawTitle(party->getRen(), buttons, x, y, party->getCursorPos());
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == dirUp)
      party->decCursorPos(buttons.size());
    else if (e.key.keysym == dirDown)
      party->incCursorPos(buttons.size());
    else if (e.key.keysym == interact)
    {
      if (party->getCursorPos() == 0)
      {
        party->changeTerr("0,0.txt");
        party->setState(MAP);
        party->setLocation(6, 10);
        SDL_PushEvent(new SDL_Event());  // push empty event to cause immediate state update
      }
      else if (party->getCursorPos() == 1)
        eventQuit();
    }
    SDL_PushEvent(new SDL_Event());  // push empty event to cause immediate state update
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (buttons[0]->buttonClick(party->getRen(), e.button))
    {
      party->changeTerr("0,0.txt");
      party->setState(MAP);
      party->setLocation(6, 10);
      SDL_PushEvent(new SDL_Event());  // push empty event to cause immediate state update
    }
    else if (buttons[1]->buttonClick(party->getRen(), e.button))
      eventQuit();
    // click on button to depress button
    break;
  default:
    break;
  }
}  // void loopTitle()


void mainLoop(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party)
{
  loopAnyState(e, party);
  switch (party->getState())
  {
  case BATTLE:
    loopBattle(e, font, party);
    break;
  case MAP:
    loopMap(e, party);
    break;
  case REBIND:
    loopRebind(party->getRen(), e, font);
    party->setState(TITLE);
    break;
  case TITLE:
    loopTitle(e, font, party);
    break;
  default:
    break;
  }  // switch (state)
}  // void mainLoop()

