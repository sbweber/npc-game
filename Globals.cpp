// Samuel Weber

#include "Globals.h"


void eventQuit()
{
  SDL_Event *q = new SDL_Event();
  q->type = SDL_QUIT;
  SDL_FlushEvents(0, UINT32_MAX);
  SDL_PushEvent(q);
}  // void eventQuit()


void logError(const string &msg, ostream &os)
{
	os << msg << endl << endl;
}  // void logError(const string &msg, ostream &os)


void logSDLError(const string &msg, ostream &os)
{
	os << msg << endl << " Error: " << SDL_GetError() << endl;
}  // void logSDLError(const string &msg, ostream &os)


SDL_Event pressAnyKey()
{
  SDL_Event e;
  SDL_FlushEvent(SDL_KEYDOWN);
  SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
  SDL_WaitEvent(&e);
  while (e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN)
    SDL_WaitEvent(&e);
  return e;
}  // void pressAnyKey()


void quit(const string err, int errtype, SDL_Renderer *ren, SDL_Window *win)
{
  // destroy renderer and window if they exist, then quit
  if (ren)
    SDL_DestroyRenderer(ren);
  if (win)
    SDL_DestroyWindow(win);
  SDL_Quit();
  if (errtype)
    logSDLError(err);
  exit (errtype);
}  // void quit()


dir randDir(mt19937_64& rng)
{
  uniform_int_distribution<int> dist(0, 3);
  int randomResult = dist(rng);
  switch (randomResult)
  {
  case 0:
    return EAST;
    break;
  case 1:
    return NORTH;
    break;
  case 2:
    return SOUTH;
    break;
  case 3:
    return WEST;
    break;
  default:
    return UNDEFINED_DIRECTION;
    break;
  }
}  // dir randDir(mt19937_64& rng)


long randNum(mt19937_64& rng, long min, long max)
{
  uniform_int_distribution<long> randNum(min, max);
  return randNum(rng);
}  // long rng(mt19937_64& rng, long min, long max)

