// Samuel Weber

#include "Keybind.h"


// debug key commands
SDL_Keysym stateBattle;
SDL_Keysym stateMap1;
SDL_Keysym stateMap2;
SDL_Keysym stateRebind;
SDL_Keysym stateTitle;
// permanent key commands
SDL_Keysym stateQuit;
SDL_Keysym back;
SDL_Keysym dirUp;
SDL_Keysym dirDown;
SDL_Keysym dirLeft;
SDL_Keysym dirRight;
SDL_Keysym interact;


bool operator==(const SDL_Keysym &lhs, const SDL_Keysym &rhs)
{
  if (lhs.sym == rhs.sym /*&& lhs.mod == rhs.mod*/)
    return true; //only checks keycode and keymod, and currently ignores keymod
  return false;
} // bool operator==(const SDL_Keysym &lhs, const SDL_Keysym &rhs)


void loadKeys()
{
  // debug key commands
  stateBattle.sym = SDLK_f;
  stateMap1.sym = SDLK_1;
  stateMap2.sym = SDLK_2;
  stateRebind.sym = SDLK_9;
  // permanent key commands
  stateTitle.sym = SDLK_F12;
  #ifdef __APPLE__
  stateQuit.sym = SDLK_w;
  stateQuit.mod = KMOD_GUI;
  #else
  stateQuit.sym = SDLK_F4;
  stateQuit.mod = KMOD_ALT;
  #endif
  back.sym = SDLK_ESCAPE;
  dirUp.sym = SDLK_UP;
  dirDown.sym = SDLK_DOWN;
  dirLeft.sym = SDLK_LEFT;
  dirRight.sym = SDLK_RIGHT;
  interact.sym = SDLK_z;
} // void loadKeys()


void rebind(SDL_Keysym &keytype, SDL_Keysym newkey)
{
  keytype = newkey;
} // void rebind(SDL_Keycode &keytype, SDL_Keycode newkey)

