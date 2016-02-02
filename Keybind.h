// Samuel Weber

#ifndef KEYBIND_H
  #define KEYBIND_H

  #include "Globals.h"

  // debug key commands
  extern SDL_Keysym stateBattle;
  extern SDL_Keysym stateMap1;
  extern SDL_Keysym stateMap2;
  extern SDL_Keysym stateRebind;
  extern SDL_Keysym stateTitle;
  // permanent key commands
  extern SDL_Keysym stateQuit;
  extern SDL_Keysym back;
  extern SDL_Keysym dirUp;
  extern SDL_Keysym dirDown;
  extern SDL_Keysym dirLeft;
  extern SDL_Keysym dirRight;
  extern SDL_Keysym interact;

  // Keybind.cpp
  bool operator==(const SDL_Keysym &lhs, const SDL_Keysym &rhs);
  //!< Returns true if the two keys are the same.
  void loadKeys();
  //!< Loads the default set of keys.
  //!< Not yet added: ability to save a set of key bindings.
  void rebind(SDL_Keysym &keytype, SDL_Keysym newKey);
  //!< Binds a new key to a command.

#endif