#ifndef settings_hpp
#define settings_hpp

#include <iostream>
#include "SDL_Headers.h"
#include "gameMenu.hpp"

struct music
{
  string name, path;
};

void setMusic(SDL_Renderer *renderer, bool &next, bool &prev, vector<music> &musicList, int &musicIndex, Mix_Music *&music, TTF_Font *&font, int x, int y)
{
  if (next || prev)
  {
    if (next)
    {
      musicIndex++;
      musicIndex %= musicList.size();
    }
    else if (prev)
    {
      musicIndex--;
      if (musicIndex < 0)
        musicIndex = musicList.size() - 1;
    }
    Mix_FreeMusic(music);
    music = Mix_LoadMUS(musicList[musicIndex].path.c_str());
  }

  SDL_Color color = {255, 255, 255};
  SDL_Surface *surface = TTF_RenderText_Solid(font, musicList[musicIndex].name.c_str(), color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect rect = {x, y, surface->w, surface->h};
  SDL_RenderCopy(renderer, texture, NULL, &rect);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

#endif // !settings_hpp
