// Samuel Weber

#include "GameState.h"


GameState::GameState(SDL_Renderer *ren, TTF_Font *f, Uint32 et) :
eventTick(et)
{
  if (!ren)
    quit("Renderer not found!", 3);
  randNumGen.seed(unsigned long(chrono::system_clock::now().time_since_epoch().count()));
  terr.reset(new Terr(ren, randNumGen, ""));
  cursorPos = 0;
  timerID = 0;
  party.reset(new Party(ren));
  font = f;
}  // GameState::GameState(SDL_Renderer *ren)


GameState::~GameState()
{
}  // GameState::~GameState()


void GameState::actionMessageHandler(string &message)
{
  size_t strpos = message.find(':');
  if (strpos != string::npos)
  {
    if (message.substr(0, strpos) == "CHANGE-STATE")
    {
      strpos = message.find(' ', strpos);
      size_t strposnew = message.find(' ', strpos + 1);
      string state = message.substr(strpos + 1, strposnew - strpos - 1);
      if (state == "STATE_BATTLE")
        setState(STATE_BATTLE);
    }
    else if (message.substr(0, strpos) == "PARTY")
    {
      strpos = message.find(' ', strpos);
      size_t strposnew = message.find(' ', strpos + 1);
      string command = message.substr(strpos + 1, strposnew - strpos - 1);
      if (command == "full-heal")
      for (int i = 0; i < 4; i++)
      if (party->getUnit(i))
        party->getUnit(i)->fullHeal();
    }
    else if (message.substr(0, strpos) == "LOAD-STATE_MAP")
    {
      strpos = message.find(' ', strpos);
      size_t strposnew = message.find(' ', strpos + 1);
      string destTerr = message.substr(strpos + 1, strposnew - strpos - 1);
      strpos = strposnew;
      strposnew = message.find(' ', strpos + 1);
      int destX = stoi(message.substr(strpos + 1, strposnew - strpos - 1));
      strpos = strposnew;
      strposnew = message.find(' ', strpos + 1);
      int destY = stoi(message.substr(strpos + 1, strposnew - strpos - 1));
      changeTerr(destTerr);
      terr->setSprite(party->getSprite(), terr->getTile(destX, destY));
    }
  }  // No colon found: do default behavior (nothing)
}  // void interactMessageHandler(unique_ptr<Party> &party, string &message)


void GameState::advance(SDL_Event &e)
{
  loopAnyState(e);
  switch (state)
  {
  case STATE_BATTLE:
    loopBattle(e);
    break;
  case STATE_MAP:
    loopMap(e);
    break;
  case STATE_REBIND:
    loopRebind(e);
    setState(STATE_TITLE);
    break;
  case STATE_TITLE:
    loopTitle(e);
    break;
  default:
    break;
  }  // switch (state)
}  // void mainLoop()


void GameState::changeTerr(const string& newTerr)
{
  party->getSprite()->setSpline(0);
  party->getSprite()->clearActs();
  terr->loadMap(newTerr, randNumGen);
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


void GameState::loopAnyState(SDL_Event &e)
{
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == stateBattle)
      setState(STATE_BATTLE);  // debug command startbattle
    else if (e.key.keysym == stateMap1)
    {
      changeTerr("0,0.txt");
      setState(STATE_MAP);
      terr->setSprite(party->getSprite(), terr->getTile(4, 3));
    }  // debug command map1
    else if (e.key.keysym == stateMap2)
    {
      changeTerr("0,1.txt");
      setState(STATE_MAP);
      terr->setSprite(party->getSprite(), terr->getTile(1, 1));
    }  // debug command map2
    else if (e.key.keysym == stateRebind)
      setState(STATE_REBIND);  // debug command go to rebind menu
    else if (e.key.keysym == stateTitle)
      setState(STATE_TITLE);
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
  queue<shared_ptr<Unit> > units;
  vector<shared_ptr<Unit> > liveParty;
  vector<shared_ptr<Unit> > liveEnemies;
  for (shared_ptr<Unit> unit : party->getUnits())
    if (!unit->isDead())
      liveParty.emplace_back(unit);
  for (shared_ptr<Unit> unit : enemies)
    if (!unit->isDead())
      liveEnemies.emplace_back(unit);
  turnOrder(units);
  while (!units.empty())
  {
    shared_ptr<Unit> attacker = units.front();
    units.pop();
    if (!attacker->isDead())
    {
      shared_ptr<Unit> target;
      if (vectorFind(liveEnemies, attacker) != liveEnemies.end())
        target = liveParty[rng(liveParty.size() - 1)];
      else
        target = liveEnemies[rng(liveParty.size() - 1)];
      Attack result = target->receiveAttack(attacker->attack(randNumGen),
              randNumGen);
      drawBattleAttackText(terr->getRen(), font, result, attacker->getName(),
              target->getName());
      drawBattleUpdate(terr->getRen(), party, font, enemies);
      if (target->isDead())
      {
        if (vectorFind(liveEnemies, target) != liveEnemies.end())
          liveEnemies.erase(vectorFind(liveEnemies, target));
        else if (vectorFind(liveParty, target) != liveParty.end())
          liveParty.erase(vectorFind(liveParty, target));
        string defeatString = target->getName() + " was defeated!";
        drawTextbox(terr->getRen(), font, defeatString);
      }
    }  // have unit check its own team and attack a target on the other team
  }
  if (liveEnemies.empty())
  {
    setState(STATE_MAP);
    long gold = 0, xp = 0;
    for (shared_ptr<Unit> unit : enemies)
    {
      gold += unit->getGold();
      xp += unit->getXP();
    }
    string str = "You won the battle!";
    str += "\nYou gained " + to_string(gold) + " gold, ";
    str += "and a total of " + to_string(xp) + " experience points";
    if (party->getUnits().size() > 1)
    {
      str += ", split " + to_string(party->getUnits().size()) + "ways for ";
      str += to_string(xp / party->getUnits().size()) + " experience points ";
      str += "to each party member!";
    }
    else
      str += "!";
    for (shared_ptr<Unit> unit : party->getUnits())
      unit->gainXP(xp / party->getUnits().size());
    party->transactGold(gold);
    drawTextbox(terr->getRen(), font, str);
    enemies.clear();
  }  // Announce victory, give party gold/xp
  else if (liveParty.empty())
  {
    setState(STATE_MAP);
    for (shared_ptr<Unit> unit : party->getUnits())
      unit->fullHeal();
    string str = "You were defeated... but at least you fully healed afterwards!";
    drawTextbox(terr->getRen(), font, str);
  }
}  // void GameState::loopBattleFight()


void GameState::loopBattleRun()
{
  string str = "Ran from battle!";
  drawTextbox(terr->getRen(), font, str);
  enemies.clear();
  setState(STATE_MAP);
}  // void GameState::loopBattleRun()


void GameState::loopMap(SDL_Event &e)
{
  shared_ptr<Tile> tile;
  action act(DIR_UNDEFINED, ACT_UNDEFINED);
  string message;
  drawMap(terr, party);
  switch (e.type)
  {
  case SDL_USEREVENT:
    if (e.user.type == eventTick)
      terr->tickSprites(randNumGen);
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
      party->move(DIR_EAST);
    else if (e.key.keysym == dirUp)
      party->move(DIR_NORTH);
    else if (e.key.keysym == dirDown)
      party->move(DIR_SOUTH);
    else if (e.key.keysym == dirLeft)
      party->move(DIR_WEST);
    else if (e.key.keysym == interact)
      party->getSprite()->pushAct(action(DIR_UNDEFINED, ACT_INTERACT));
    break;
  case SDL_KEYUP:
    party->getSprite()->clearActs();
    break;
  default:
    break;
  }
  if (party->keepMoving())
    party->getSprite()->pushAct(action(get<0>(party->getSprite()->topAct()), ACT_MOVE));
  if (e.type != SDL_USEREVENT || e.user.type != eventTick)
  {
    message = terr->actSprites(party->getSprite(), enemies);
    actionMessageHandler(message);
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
        setState(STATE_MAP);
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
      setState(STATE_MAP);
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


long long GameState::rng(long long min, long long max)
{
  uniform_int_distribution<long long> dist(min, max);
  return dist(randNumGen);
}  // long GameState::rng(long min, long max)


size_t GameState::rng(size_t max)
{
  uniform_int_distribution<size_t> dist(0, max);
  return dist(randNumGen);
}  // long GameState::rng(long min, long max)


void GameState::setState(gameState gs)
{
  if (gs == STATE_MAP)
    timerID = SDL_AddTimer(TICK_MS, mapTimerCallback, (void*)eventTick);
  else if (state == STATE_MAP)
    SDL_RemoveTimer(timerID);
  state = gs;
}  // void GameState::setState(gameState gs)


shared_ptr<Tile> GameState::tileClick(SDL_MouseButtonEvent &click)
{
  return terr->tileClick(click, party->getSprite());
}  // shared_ptr<Tile> GameState::tileClick(SDL_MouseButtonEvent &click)


void GameState::turnOrder(queue<shared_ptr<Unit> > &order)
{
  long long max = 0;
  vector<shared_ptr<Unit> > units;
  for (shared_ptr<Unit> unit : party->getUnits())
    if (!unit->isDead())
      units.emplace_back(unit);
  for (shared_ptr<Unit> unit : enemies)
    if (!unit->isDead())
      units.emplace_back(unit);
  for (shared_ptr<Unit> unit : units)
    max += unit->getAgi();
  while (max)
  {
    long long roll = rng(0, max);
    for (vector<shared_ptr<Unit> >::iterator uItr = units.begin(); uItr != units.end(); uItr++)
    {
      roll -= (*uItr)->getAgi();
      if (roll <= 0)
      {
        order.push(*uItr);
        max -= (*uItr)->getAgi();
        units.erase(uItr);
        break;
      }
    }
  }
}  // void GameState::turnOrder(queue<shared_ptr<Unit> > &order)

