#ifndef settings_hpp
#define settings_hpp

#include "SDL_Headers.h"
#include "gameMenu.hpp"

void setVolume(button volumeUp, button volumeDown, int &volume)
{
  if (volumeUp.wasClicked && volume < 100)
  {
    volume += 20;
    Mix_Volume(-1, volume);
    volumeUp.wasClicked = false;
  }
  if (volumeDown.wasClicked && volume > 0)
  {
    volume -= 20;
    Mix_Volume(-1, volume);
    volumeDown.wasClicked = false;
  }
}

#endif // !settings_hpp
