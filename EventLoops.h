//Samuel Weber

#ifndef EVENTLOOPS_H
  #define EVENTLOOPS_H

  #include "Draw.h"
  #include "Keybind.h"
  #include "Include.h"
  #include "Party.h"
  #include "Terr.h"

  //EventLoops.cpp
  bool loopAnyState(SDL_Renderer *ren, SDL_Event &e, Terr *terr, Party *party, Sprite *npc, gameState &state);
  bool loopBattle(SDL_Event &e);
  bool loopMap(SDL_Renderer *ren, SDL_Texture *tiles, Terr *terr, SDL_Event &e, Party *party);
  bool loopRebind(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font, gameState &state);
  bool loopTitle(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font, Terr *terr, gameState &state, Party *party, Sprite *npc, vector<Button *> buttons);
  bool mainLoop(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font, SDL_Texture *tiles, Terr *terr, Party* party, Sprite *npc, gameState &state, vector<Button *> buttons);

#endif
