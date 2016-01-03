//Samuel Weber

#ifndef INCLUDE_H
  #define INCLUDE_H

  //c++ core includes
  #include <cstddef> //implicitly added on non-windows machines, doesn't hurt to manually add
  #include <ctime>
  #include <iostream>
  #include <fstream>
  #include <new>
  #include <string>
  #include <vector>

  //SDL includes
  #include <SDL2/SDL.h>
  #include <SDL2_image/SDL_image.h>
  #include <SDL2_ttf/SDL_ttf.h>

  //global constants
  const int SCREEN_WIDTH = 640;
  const int SCREEN_HEIGHT = 480;
  const int TILE_SIZE = 32;
  const int TILE_WIDTH = 32;
  const int TILE_HEIGHT = 32;
  const int NUM_TILES_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
  const int NUM_TILES_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;

  using namespace std;

  enum dir
  {
    NORTH,
    SOUTH,
    WEST,
    EAST
  };

  enum gameState
  {
    BATTLE,
    MAP,
    REBIND,
    TITLE
  };

  //Library.cpp
  void logSDLError(const string &msg, ostream &os = cerr);
  void pressAnyKey();
  const long int rng(long int min, long int max);
  void quit(const string err = "", int errtype = -1, SDL_Renderer *ren = nullptr, SDL_Window *win = nullptr);

#endif

