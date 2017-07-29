// Samuel Weber

#include "Timer.h"


Uint32 mapTimerCallback(Uint32 interval, void* event)
{
  SDL_PushEvent((SDL_Event *)event);
  return interval;
}  // Uint32 mapTimerCallback(Uint32 interval, void* param)
