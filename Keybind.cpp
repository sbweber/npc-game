// Samuel Weber

#include "Keybind.h"


// debug key commands
SDL_Keysym stateMap1;
SDL_Keysym stateMap2;
SDL_Keysym stateRebind;
// permanent key commands
SDL_Keysym stateTitle;
SDL_Keysym stateQuit;
SDL_Keysym back;
SDL_Keysym dirUp;
SDL_Keysym dirDown;
SDL_Keysym dirLeft;
SDL_Keysym dirRight;
SDL_Keysym interact;


bool operator==(SDL_Keysym lhs, SDL_Keysym rhs)
{
  lhs.mod |= (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  rhs.mod |= (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  if ((lhs.sym == rhs.sym) && (lhs.mod == rhs.mod))
    return true;
  return false;
}  // bool operator==(const SDL_Keysym &lhs, const SDL_Keysym &rhs)


void loadKeys()
{
  // debug key commands
  stateMap1.sym = SDLK_1;
  stateMap1.mod = (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  stateMap2.sym = SDLK_2;
  stateMap2.mod = (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  stateRebind.sym = SDLK_9;
  stateRebind.mod = (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  // permanent key commands
  stateTitle.sym = SDLK_F12;
  stateTitle.mod = (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  #ifdef __APPLE__
    stateQuit.sym = SDLK_q;
    stateQuit.mod = KMOD_GUI;
  #elif WIN32
    stateQuit.sym = SDLK_F4;
    stateQuit.mod = KMOD_ALT;
  #else
    stateQuit.sym = SDLK_c;
    stateQuit.mod = KMOD_CTRL;
  #endif
  back.sym = SDLK_ESCAPE;
  back.mod = (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  dirUp.sym = SDLK_UP;
  dirUp.mod = (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  dirDown.sym = SDLK_DOWN;
  dirDown.mod = (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  dirLeft.sym = SDLK_LEFT;
  dirLeft.mod = (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  dirRight.sym = SDLK_RIGHT;
  dirRight.mod = (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
  interact.sym = SDLK_z;
  interact.mod = (KMOD_NONE | KMOD_NUM | KMOD_CAPS | KMOD_MODE);
}  // void loadKeys()


void rebind(SDL_Keysym &keytype, SDL_Keysym newkey)
{
  keytype = newkey;
}  // void rebind(SDL_Keycode &keytype, SDL_Keycode newkey)

