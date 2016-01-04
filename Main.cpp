
#include "Main.h"

int main(int argc, char **argv)
{
  //Create window, do basic setup
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    quit("SDL_Init", 1);
  SDL_Window *win = SDL_CreateWindow("NPC-Game", 10, 30, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (win == nullptr)
    quit("SDL_CreateWindow", 2);
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr)
    quit("SDL_CreateRenderer", 3);
  if (TTF_Init() != 0)
    quit("TTF_Init", 4);
  TTF_Font *font = TTF_OpenFont("resources/ClearSans-Light.ttf", 20);
  if (font == nullptr)
    quit("TTF_OpenFont", 5);

  //SDL vars
  SDL_Event e;
  bool quit = false;

  //backend vars
  Terr* terr = new Terr("");
  Party* party = new Party(ren);
  Sprite* npc = new Sprite(ren, "npc.png");
  vector<Button *> buttons;
  npc->setType("test");
  gameState state = TITLE;

  //spritesheets
  SDL_Texture *tiles = loadTexture("tiles.png", ren);

  loadKeys();

  while (!quit)
    while (SDL_PollEvent(&e))
      quit = mainLoop(ren, e, font, tiles, terr, party, npc, state, buttons);

  SDL_DestroyTexture(tiles);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  TTF_CloseFont(font);
  delete terr;
  delete party;
  delete npc;

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  return 0;
}//int main(int argc, char **argv)

bool loopAnyState(SDL_Renderer *ren, SDL_Event &e, Terr *terr, Party* party, Sprite *npc, gameState &state)
{
  switch (e.type)
  {
  case SDL_KEYDOWN:
    if (e.key.keysym == stateBattle)
      state = BATTLE; //debug command startbattle
    if (e.key.keysym == stateMap1)
    {
      delete terr;
      terr = new Terr("map1.txt");
      state = MAP;
      party->getSprite()->setTile(terr->getTile(4, 3));
      npc->setTile(terr->getTile(5, 4));
    }//debug command map1
    if (e.key.keysym == stateMap2)
    {
      delete terr;
      terr = new Terr("map2.txt");
      state = MAP;
      party->getSprite()->setTile(terr->getTile(1, 1));
    }//debug command map2
    if (e.key.keysym == stateRebind)
      state = REBIND; //debug command go to rebind menu
    if (e.key.keysym == stateQuit)
      return true;
    break;
  default:
    break;
  }//switch (event type)
  return false;
}//bool loopAnyState(SDL_Renderer *ren, SDL_Event &e, Terr *terr, Party* party, Sprite *npc, gameState &state)

bool loopBattle(SDL_Event &e)
{
  switch (e.type)
  {
  default:
    break;
  }
  return false;
}//bool loopBattle(SDL_Event &e)

bool loopMap(SDL_Renderer *ren, SDL_Texture* tiles, Terr* terr, SDL_Event &e, Party* party)
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
}//bool loopMap(SDL_Event &e, Party* party)

bool loopRebind(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font, gameState &state)
{
  drawRebind(ren, font);
  switch (e.type)
  {
  default:
    break;
  }
  state = TITLE;
  return false;
}//bool loopRebind(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font, gameState &state)

bool loopTitle(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font, Terr *terr, gameState &state, Party *party, Sprite *npc, vector<Button *> buttons)
{
  Button *toGame = new Button(ren, "button.png", SCREEN_WIDTH / 2 - 120, 300, 240, 100, font, "To Game");
  drawTitle(ren, toGame);
  switch (e.type)
  {
  case SDL_KEYDOWN:
    //use arrow keys to select button; enter/interact to depress button
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (toGame->buttonClick(e.button))
    {
      delete terr;
      terr = new Terr("map1.txt");
      state = MAP;
      party->getSprite()->setTile(terr->getTile(4, 3));
      npc->setTile(terr->getTile(5, 4));
      SDL_Event* wait = new SDL_Event();
      SDL_PushEvent(wait); //push an empty event to cause an immediate state update
    }
    //click on button to depress button
    break;
  case SDL_MOUSEBUTTONUP:
    //actually follow through on buttonclick iff buttonup is also in button area
    break;
  case SDL_MOUSEMOTION:
    //mouseover button to select button 
    break;
  default:
    break;
  }
  return false;
}//bool loopTitle(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font, Terr *terr, gameState &state, Party *party, Sprite *npc)

bool mainLoop(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font, SDL_Texture *tiles, Terr *terr, Party* party, Sprite *npc, gameState &state, vector<Button *> buttons)
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
  }//switch (state)
  return quit;
}//bool mainLoop(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font, SDL_Texture *tiles, Terr *terr, Party* party, Sprite *npc, gameState &state)

