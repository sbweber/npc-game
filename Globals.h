// Samuel Weber

#ifndef INCLUDE_H
  #define INCLUDE_H

  // c++ core includes
  #include <algorithm>
  #include <chrono>
  #include <cstddef>  // implicitly added on non-windows machines
  #include <cstdlib>
  #include <ctime>
  #include <iostream>
  #include <fstream>
  #include <memory>
  #include <queue>
  #include <random>
  #include <string>
  #include <tuple>
  #include <unordered_map>
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
  const int SCREEN_WIDTH = 1024;
  const int SCREEN_HEIGHT = 768;
  const int TILE_WIDTH = 32;
  const int TILE_HEIGHT = 32;
  const int NUM_TILES_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
  const int NUM_TILES_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;
  const int TICK_MS = 1000;
  const double NUM_TICKS_SEC = 1000 / TICK_MS;
  const int NUM_FRAMES_SPLINE = 8;

  using namespace std;


  enum DIR
  {
    DIR_EAST,
    DIR_NORTH,
    DIR_SOUTH,
    DIR_WEST,
    DIR_UNDEFINED
  };


  enum GAME_STATE
  {
    STATE_BATTLE,
    STATE_MAP,
    STATE_REBIND,
    STATE_TITLE
  };


  // Globals.cpp
  void eventQuit();
  //!< Pushes an SDL_Quit type event onto the event queue and clears the queue.
  void logError(const string &msg, ostream &os = cerr);
  //!< Logs an error to the output of choice, cerr by default.
  void logSDLError(const string &msg, ostream &os = cerr);
  //!< Logs an error to the output of choice, cerr by default. Also prints out
  //!< last error encountered by SDL, which may not be relevant to the problem.
  SDL_Event pressAnyKey();
  //!< Make the program wait for a key (any key) to be pressed.
  //!< Alternately, mouseclicks count. Returns the key/click event.
  void quit(const string err = "", int errtype = -1,
          SDL_Renderer *ren = nullptr, SDL_Window *win = nullptr);
  //!< Quits the program, logging an error if given one.
  DIR randDir(mt19937_64& rng);
  //!< Returns a random direction (DIR_UNDEFINED if there's an error).
  long long rng(mt19937_64& rng, long long min, long long max);
  //!< Returns a random number from min to max, uniform distribution.

#endif

