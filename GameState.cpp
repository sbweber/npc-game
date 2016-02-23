// Samuel Weber

#include "GameState.h"


GameState::GameState(SDL_Renderer *ren, TTF_Font *f, Uint32 et) :
eventTick(et)
{
  if (!ren)
    quit("Renderer not found!", 3);
  randNumGen.seed(chrono::system_clock::now().time_since_epoch().count());
  terr.reset(new Terr(ren, ""));
  cursorPos = 0;
  timerID = 0;
  party.reset(new Party(ren));
  font = f;
}  // GameState::GameState(SDL_Renderer *ren)


GameState::~GameState()
{
}  // GameState::~GameState()


void GameState::advance(SDL_Event &e)
{
  loopAnyState(e);
  switch (state)
  {
  case BATTLE:
    loopBattle(e);
    break;
  case MAP:
    loopMap(e);
    break;
  case REBIND:
    loopRebind(e);
    setState(TITLE);
    break;
  case TITLE:
    loopTitle(e);
    break;
  default:
    break;
  }  // switch (state)
}  // void mainLoop()


void GameState::changeTerr(const string& newTerr)
{
  terr->loadMap(newTerr);
}  // void GameState::changeTerr(string& newTerr)


void GameState::decCursorPos(unsigned int max)
{
  if (cursorPos == 0)
    cursorPos = max - 1;
  else
    cursorPos--;
}  // void GameState::decCursorPos(int max)


void GameState::incCursorPos(unsigned int max)
{
  if (cursorPos >= (max - 1))
    cursorPos = 0;
  else
    cursorPos++;
}  // void GameState::incCursorPos(int max)


void GameState::interactMessageHandler(string &message)
{
  size_t strpos = message.find(':');
  if (strpos != string::npos)
  {
    if (message.substr(0, strpos) == "CHANGE-STATE")
    {
      strpos = message.find(' ', strpos);
      size_t strposnew = message.find(' ', strpos + 1);
      string state = message.substr(strpos + 1, strposnew - strpos - 1);
      if (state == "BATTLE")
        setState(BATTLE);
    }
    if (message.substr(0, strpos) == "PARTY")
    {
      strpos = message.find(' ', strpos);
      size_t strposnew = message.find(' ', strpos + 1);
      string command = message.substr(strpos + 1, strposnew - strpos - 1);
      if (command == "full-heal")
        for (int i = 0; i < 4; i++)
          if (party->getUnit(i))
            party->getUnit(i)->fullHeal();
    }
  }  // No colon found: do default behavior (nothing)
}  // void interactMessageHandler(unique_ptr<Party> &party, string &message)


void GameState::loopAnyState(SDL_Event &e)
{
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == stateBattle)
      setState(BATTLE);  // debug command startbattle
    else if (e.key.keysym == stateMap1)
    {
      changeTerr("0,0.txt");
      setState(MAP);
      terr->setSprite(party->getSprite(), terr->getTile(4, 3));
    }  // debug command map1
    else if (e.key.keysym == stateMap2)
    {
      changeTerr("0,1.txt");
      setState(MAP);
      terr->setSprite(party->getSprite(), terr->getTile(1, 1));
    }  // debug command map2
    else if (e.key.keysym == stateRebind)
      setState(REBIND);  // debug command go to rebind menu
    else if (e.key.keysym == stateTitle)
      setState(TITLE);
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
}  // void GameState::loopAnyState()


void GameState::loopBattle(SDL_Event &e)
{
  vector<unique_ptr<Button> > buttons;
  int x, y;
  buttons.emplace_back(new Button(terr->getRen(), "Button.png",
    100, 643, 240, 100, font, "Fight"));
  buttons.emplace_back(new Button(terr->getRen(), "Button.png",
    684, 643, 240, 100, font, "Run"));
  SDL_GetMouseState(&x, &y);
  if (cursorPos > buttons.size())
    cursorPos = 0;
  drawBattle(terr->getRen(), party, font, buttons, x, y, cursorPos, enemies);
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == dirLeft)
      decCursorPos(buttons.size());
    else if (e.key.keysym == dirRight)
      incCursorPos(buttons.size());
    else if (e.key.keysym == interact)
    {
      if (cursorPos == 0)
        loopBattleFight();
      else if (cursorPos == 1)
        loopBattleRun();
    }
    SDL_PushEvent(new SDL_Event());  // push empty event to cause immediate state update
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (buttons[0]->buttonClick(terr->getRen(), e.button))
      loopBattleFight();
    else if (buttons[1]->buttonClick(terr->getRen(), e.button))
      loopBattleRun();
    // click on button to depress button
    SDL_PushEvent(new SDL_Event());  // push empty event to cause immediate state update
    break;
  default:
    break;
  }
}  // void GameState::loopBattle()


void GameState::loopBattleFight()
{
  long damage = enemies[0]->receiveAttack(party->getUnit(0)->attack());
  string str = "You attacked the enemy for " + to_string(damage) + " damage!";
  renderTextbox(terr->getRen(), font, str);
  SDL_RenderPresent(terr->getRen());
  pressAnyKey();
  drawBattleUpdate(terr->getRen(), party, font, enemies);
  if (enemies[0]->isDead())
  {
    enemies.pop_back();
    str = "Enemy defeated!";
    renderTextbox(terr->getRen(), font, str);
    SDL_RenderPresent(terr->getRen());
    pressAnyKey();
  }
  else
  {
    damage = party->getUnit(0)->receiveAttack(enemies[0]->attack());
    str = "You were attacked for " + to_string(damage) + " damage!";
    renderTextbox(terr->getRen(), font, str);
    SDL_RenderPresent(terr->getRen());
    pressAnyKey();
    drawBattleUpdate(terr->getRen(), party, font, enemies);
  }
  if (enemies.empty())
  {
    setState(MAP);
    str = "You won the battle!";
    renderTextbox(terr->getRen(), font, str);
    SDL_RenderPresent(terr->getRen());
    pressAnyKey();
  }
  else if (party->getUnit(0)->isDead())
  {
    setState(MAP);
    party->getUnit(0)->fullHeal();
    str = "You were defeated... but at least you fully healed afterwards!";
    renderTextbox(terr->getRen(), font, str);
    SDL_RenderPresent(terr->getRen());
    pressAnyKey();
  }
}  // void GameState::loopBattleFight()


void GameState::loopBattleRun()
{
  string str = "Ran from battle!";
  renderTextbox(terr->getRen(), font, str);
  SDL_RenderPresent(terr->getRen());
  pressAnyKey();
  enemies.clear();
  setState(MAP);
}  // void GameState::loopBattleRun()


void GameState::loopMap(SDL_Event &e)
{
  shared_ptr<Tile> tile;
  action act(UNDEFINED_DIRECTION, BAD_ACTION);
  string message;
  drawMap(terr, party);
  switch (e.type)
  {
  case SDL_USEREVENT:
    if (e.user.type == eventTick)
    {
      terr->tickSprites(randNumGen);
      message = terr->actSprites(party->getSprite(), enemies);
      interactMessageHandler(message);
    }
    break;
  case SDL_MOUSEBUTTONDOWN:
    tile = tileClick(e.button);
    if (tile && tile->getIsPassable())
    {
      terr->findPath(terr->getTile(party->getSprite()),
              tile, party->getSprite());
    }  // If tile can be legally entered, path to it
    break;
  case SDL_KEYDOWN:
    party->getSprite()->clearActs();
    if (e.key.keysym == dirRight)
      party->move(EAST);
    else if (e.key.keysym == dirUp)
      party->move(NORTH);
    else if (e.key.keysym == dirDown)
      party->move(SOUTH);
    else if (e.key.keysym == dirLeft)
      party->move(WEST);
    else if (e.key.keysym == interact)
      party->getSprite()->pushAct(action(UNDEFINED_DIRECTION, INTERACT));
    break;
  case SDL_KEYUP:
    party->move(UNDEFINED_DIRECTION);
    party->getSprite()->clearActs();
    break;
  default:
    break;
  }
  if (party->getMoveButtonHeld())
    party->getSprite()->pushAct(action(party->getMoveButtonDir(), MOVE));
  if (e.type != SDL_USEREVENT || e.user.type != eventTick)
  {
    message = terr->actSprite(party->getSprite(), party->getSprite(), enemies);
    interactMessageHandler(message);
  }
}  // void GameState::loopMap()


void GameState::loopRebind(SDL_Event &e)
{
  drawRebind(terr->getRen(), font);
  switch (e.type)
  {
  case SDL_KEYDOWN:
    break;
  default:
    break;
  }
}  // void GameState::loopRebind()


void GameState::loopTitle(SDL_Event &e)
{
  vector<unique_ptr<Button> > buttons;
  int x, y;
  buttons.emplace_back(new Button(terr->getRen(), "Button.png",
          SCREEN_WIDTH / 2 - 120, 300, 240, 100, font, "To Game"));
  buttons.emplace_back(new Button(terr->getRen(), "Button.png",
          SCREEN_WIDTH / 2 - 120, 450, 240, 100, font, "Quit"));
  SDL_GetMouseState(&x, &y);
  if (cursorPos > buttons.size())
    cursorPos = 0;
  drawTitle(terr->getRen(), buttons, x, y, cursorPos);
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == dirUp)
      decCursorPos(buttons.size());
    else if (e.key.keysym == dirDown)
      incCursorPos(buttons.size());
    else if (e.key.keysym == interact)
    {
      if (cursorPos == 0)
      {
        changeTerr("0,0.txt");
        setState(MAP);
        terr->setSprite(party->getSprite(), terr->getTile(6, 10));
      }
      else if (cursorPos == 1)
        eventQuit();
    }
    SDL_PushEvent(new SDL_Event());  // push empty event to cause immediate state update
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (buttons[0]->buttonClick(terr->getRen(), e.button))
    {
      changeTerr("0,0.txt");
      setState(MAP);
      terr->setSprite(party->getSprite(), terr->getTile(6, 10));
    }
    else if (buttons[1]->buttonClick(terr->getRen(), e.button))
      eventQuit();
    // click on button to depress button
    SDL_PushEvent(new SDL_Event());  // push empty event to cause immediate state update
    break;
  default:
    break;
  }
}  // GameState::void loopTitle()


long GameState::rng(long min, long max)
{
  uniform_int_distribution<long> randNum(min, max);
  return randNum(randNumGen);
}  // long GameState::rng(long min, long max)


void GameState::setState(gameState gs)
{
  if (gs == MAP)
    timerID = SDL_AddTimer(TICK_MS, mapTimerCallback, (void*)eventTick);
  else if (state == MAP)
    SDL_RemoveTimer(timerID);
  state = gs;
}  // void GameState::setState(gameState gs)


shared_ptr<Tile> GameState::tileClick(SDL_MouseButtonEvent &click)
{
  return terr->tileClick(click, party->getSprite());
}  // shared_ptr<Tile> GameState::tileClick(SDL_MouseButtonEvent &click)

