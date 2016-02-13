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


void loopBattle(SDL_Event &e, TTF_Font* font, unique_ptr<Party> &party,
        vector<shared_ptr<Unit> > &enemies)
{
  vector<unique_ptr<Button> > buttons;
  int x, y;
  buttons.emplace_back(new Button(party->getRen(), "Button.png",
    100, 643, 240, 100, font, "Fight"));
  buttons.emplace_back(new Button(party->getRen(), "Button.png",
    684, 643, 240, 100, font, "Run"));
  SDL_GetMouseState(&x, &y);
  if (party->getCursorPos() > buttons.size())
    party->resetCursorPos();
  drawBattle(party, font, buttons, x, y, party->getCursorPos(), enemies);
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == dirLeft)
      party->decCursorPos(buttons.size());
    else if (e.key.keysym == dirRight)
      party->incCursorPos(buttons.size());
    else if (e.key.keysym == interact)
    {
      if (party->getCursorPos() == 0)
        loopBattleFight(font, party, enemies);
      else if (party->getCursorPos() == 1)
        loopBattleRun(font, party, enemies);
    }
    SDL_PushEvent(new SDL_Event());  // push empty event to cause immediate state update
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (buttons[0]->buttonClick(party->getRen(), e.button))
      loopBattleFight(font, party, enemies);
    else if (buttons[1]->buttonClick(party->getRen(), e.button))
      loopBattleRun(font, party, enemies);
    // click on button to depress button
    SDL_PushEvent(new SDL_Event());  // push empty event to cause immediate state update
    break;
  default:
    break;
  }
}  // void loopBattle(SDL_Event &e)


void loopBattleFight(TTF_Font *font, unique_ptr<Party> &party,
        vector<shared_ptr<Unit> > &enemies)
{
  long damage = enemies[0]->receiveAttack(party->getUnit(0)->attack());
  string str = "You attacked the enemy for " + to_string(damage) + " damage!";
  renderTextbox(party->getRen(), font, str);
  SDL_RenderPresent(party->getRen());
  pressAnyKey();
  drawBattleUpdate(party, font, enemies);
  if (enemies[0]->isDead())
  {
    enemies.pop_back();
    str = "Enemy defeated!";
    renderTextbox(party->getRen(), font, str);
    SDL_RenderPresent(party->getRen());
    pressAnyKey();
  }
  else
  {
    damage = party->getUnit(0)->receiveAttack(enemies[0]->attack());
    str = "You were attacked for " + to_string(damage) + " damage!";
    renderTextbox(party->getRen(), font, str);
    SDL_RenderPresent(party->getRen());
    pressAnyKey();
    drawBattleUpdate(party, font, enemies);
  }
  if (enemies.empty())
  {
    party->setState(MAP);
    str = "You won the battle!";
    renderTextbox(party->getRen(), font, str);
    SDL_RenderPresent(party->getRen());
    pressAnyKey();
  }
  else if (party->getUnit(0)->isDead())
  {
    party->setState(MAP);
    party->getUnit(0)->fullHeal();
    str = "You were defeated... but at least you fully healed afterwards!";
    renderTextbox(party->getRen(), font, str);
    SDL_RenderPresent(party->getRen());
    pressAnyKey();
  }
}  // void loopBattleFight(TTF_Font *font, unique_ptr<Party> &party)


void loopBattleRun(TTF_Font *font, unique_ptr<Party> &party,
       vector<shared_ptr<Unit> > &enemies)
{
  string str = "Ran from battle!";
  renderTextbox(party->getRen(), font, str);
  SDL_RenderPresent(party->getRen());
  pressAnyKey();
  enemies.clear();
  party->setState(MAP);
}  // void loopBattleRun(TTF_Font *font, unique_ptr<Party> &party)


void loopMap(SDL_Event &e, unique_ptr<Party> &party,
        vector<shared_ptr<Unit> > &enemies)
{
  shared_ptr<Tile> tile;
  action act(UNDEFINED_DIRECTION, BAD_ACTION);
  SDL_Event* wait = new SDL_Event();
  while(drawMap(party));
  switch (e.type)
  {
  case SDL_MOUSEBUTTONDOWN:
    tile = party->tileClick(e.button);
    if (tile && tile->getIsPassable())
    {
      party->getTerr()->findPath(party->getTerr()->getTile(party->getSprite()),
        tile, party->getSprite());
    }  // If tile can be legally entered, path to it
    break;
  case SDL_KEYDOWN:
    party->getSprite()->clearActs();
    if (e.key.keysym == dirUp)
      party->getSprite()->pushAct(action(NORTH, MOVE));
    if (e.key.keysym == dirLeft)
      party->getSprite()->pushAct(action(WEST, MOVE));
    if (e.key.keysym == dirDown)
      party->getSprite()->pushAct(action(SOUTH, MOVE));
    if (e.key.keysym == dirRight)
      party->getSprite()->pushAct(action(EAST, MOVE));
    if (e.key.keysym == interact)
      party->getSprite()->pushAct(action(UNDEFINED_DIRECTION, INTERACT));
  default:
    act = party->getSprite()->popAct();
    switch (get<1>(act))
    {
    case MOVE:
      party->move(get<0>(act), false);
      SDL_PushEvent(wait);  // push empty event to cause immediate state update
      break;
    case INTERACT:
      party->setState(move(party->getTerr()->interactSprite(
              party->getSprite(), enemies)));
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
  if (party->getCursorPos() > buttons.size())
    party->resetCursorPos();
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
    }
    else if (buttons[1]->buttonClick(party->getRen(), e.button))
      eventQuit();
    // click on button to depress button
    SDL_PushEvent(new SDL_Event());  // push empty event to cause immediate state update
    break;
  default:
    break;
  }
}  // void loopTitle()


void mainLoop(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party,
        vector<shared_ptr<Unit> > &enemies)
{
  loopAnyState(e, party);
  switch (party->getState())
  {
  case BATTLE:
    loopBattle(e, font, party, enemies);
    break;
  case MAP:
    loopMap(e, party, enemies);
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

