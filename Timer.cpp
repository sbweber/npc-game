// Samuel Weber

#include "Timer.h"


Uint32 mapTimerCallback(Uint32 interval, void* eventID)
{
  SDL_Event e;
  e.type = SDL_USEREVENT;
  e.user.type = Uint32(eventID);
  SDL_PushEvent(&e);
  return interval;
}  // Uint32 mapTimerCallback(Uint32 interval, void* param)
