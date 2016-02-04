// Samuel Weber

#include "Globals.h"

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
  while (true)
    if (SDL_PollEvent(&e))
      if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
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


long rng(long min, long max)
{
  int randNum;
  if (min > max)
  {
    long temp = min;
    min = max;
    max = temp;
  }  // if (min > max)
  if ((max > RAND_MAX) || ((max - min) > RAND_MAX))
    return -1;  // ERROR: random number maximum is too big or range is too big
  randNum = rand();
  for (int counter = 0; (RAND_MAX - randNum) < (max - min); counter++)
  {
    randNum = rand();
    if (counter > 10)
    {
      srand((unsigned int)time(NULL));
      counter = 0;
    }  // If rerolled too many times, reseed and reset counter.
  }  // while die cannot finish iteration, reroll and increment reseed counter.
  randNum = randNum % (max - min + 1) + min;
  return randNum;
}  // const long rng(long min, long max)

