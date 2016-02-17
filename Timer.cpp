// Samuel Weber

#include "Timer.h"


Uint32 mapTimerCallback(Uint32 interval, void* param)
{
  SDL_Event e;
  e.type = SDL_USEREVENT;
  SDL_PushEvent(&e);
  return interval;
}  // Uint32 mapTimerCallback(Uint32 interval, void* param)
