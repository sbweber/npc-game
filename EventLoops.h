// Samuel Weber

#ifndef EVENTLOOPS_H
  #define EVENTLOOPS_H

  #include "Draw.h"
  #include "Globals.h"
  #include "Keybind.h"
  #include "Party.h"

  // EventLoops.cpp
  bool loopAnyState(SDL_Event &e, unique_ptr<Party> &party, gameState &state);
  //!< Loop to handle input that’s treated the same in all states.
  bool loopBattle(SDL_Event &e);
  //!< Loop to process actions in battle state.
  bool loopMap(SDL_Event &e, unique_ptr<Party> &party);
  //!< Loop to process inputs while on overworld map.
  bool loopRebind(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font,
          gameState &state);
  //!< Loop to process inputs and events while in the key binding menu.
  bool loopTitle(SDL_Event &e, TTF_Font *font,
          gameState &state, unique_ptr<Party> &party,
          vector<unique_ptr<Button> > buttons);
  //!< Loop to process inputs on the title screen.
  bool mainLoop(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party,
          gameState &state);
  //!< Loop to select which event loop to run.

#endif
