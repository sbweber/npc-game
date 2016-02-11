// Samuel Weber

#ifndef EVENTLOOPS_H
  #define EVENTLOOPS_H

  #include "Draw.h"
  #include "Globals.h"
  #include "Keybind.h"
  #include "Party.h"

  // EventLoops.cpp
  void loopAnyState(SDL_Event &e, unique_ptr<Party> &party);
  //!< Loop to handle input that’s treated the same in all states.
  void loopBattle(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party,
          vector<unique_ptr<Unit> > &enemies);
  //!< Loop to process actions in battle state.
  void loopBattleFight(TTF_Font *font, unique_ptr<Party> &party,
          vector<unique_ptr<Unit> > &enemies);
  //!< Subloop for the 'fight' command in battle.
  void loopMap(SDL_Event &e, unique_ptr<Party> &party,
          vector<unique_ptr<Unit> > &enemies);
  //!< Loop to process inputs while on overworld map.
  void loopRebind(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font);
  //!< Loop to process inputs and events while in the key binding menu.
  void loopTitle(SDL_Event &e, TTF_Font *font,
          unique_ptr<Party> &party, vector<unique_ptr<Button> > buttons);
  //!< Loop to process inputs on the title screen.
  void mainLoop(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party,
          vector<unique_ptr<Unit> > &enemies);
  //!< Loop to select which event loop to run.

#endif
