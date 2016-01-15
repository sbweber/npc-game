// Samuel Weber

#ifndef INCLUDE_H
  #define INCLUDE_H

  // c++ core includes
  #include <cstddef>  // implicitly added on non-windows machines
  #include <ctime>
  #include <iostream>
  #include <fstream>
  #include <new>
  #include <string>
  #include <vector>

  // SDL includes
  #ifdef __APPLE__
    #include <SDL2/SDL.h>
    #include <SDL2_image/SDL_image.h>
    #include <SDL2_ttf/SDL_ttf.h>
  #else
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>
  #endif

  // global constants
  const int SCREEN_WIDTH = 640;
  const int SCREEN_HEIGHT = 480;
  const int TILE_SIZE = 32;
  const int TILE_WIDTH = 32;
  const int TILE_HEIGHT = 32;
  const int NUM_TILES_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
  const int NUM_TILES_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;
  const int NUM_FRAMES_SPLINE = 8;

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

  // Globals.cpp
  void logError(const string &msg, ostream &os = cerr);
  //!< Logs an error to the output of choice, cerr by default.
  void logSDLError(const string &msg, ostream &os = cerr);
  //!< Logs an error to the output of choice, cerr by default. Also prints out
  //!< last error encountered by SDL, which may not be relevant to the problem.
  void pressAnyKey();
  //!< Make the program wait for a key (any key) to be pressed.
  void quit(const string err = "", int errtype = -1,
          SDL_Renderer *ren = nullptr, SDL_Window *win = nullptr);
  //!< Quits the program, logging an error if given one.
  long int rng(long int min, long int max);
  //!< My wrapper for rand(). Better than a naked call, still flawed.

#endif

