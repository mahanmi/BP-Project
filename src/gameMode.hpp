#ifndef gamemode_hpp
#define gamemode_hpp

#include <iostream>
#include <string>
#include <vector>
#include "SDL_Headers.h"
#include "leaderboard.hpp"

using namespace std;

int get_current_time(uint32_t start)
{
  return SDL_GetTicks() - start;
}

void showUserInput(SDL_Renderer *renderer, TTF_Font *font, string userInput, int x, int y)
{
  SDL_Color color = {0, 0, 0, 255};
  if (userInput == "")
  {
    userInput = "Enter username";
    color = {0, 0, 0, 128};
  }
  SDL_Surface *surface = TTF_RenderText_Solid(font, userInput.c_str(), color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect rect = {x, y, surface->w, surface->h};
  SDL_RenderCopy(renderer, texture, NULL, &rect);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void addNewPlayerToFile(player newPlayer)
{
  ofstream file;
  file.open("assets/leaderboard/leaderboard.txt", ios::app);
  file << newPlayer.name << " " << newPlayer.classicScore << " " << newPlayer.infiniteScore << " " << newPlayer.timerScore << endl;
  file.close();
}

#endif // !gamemode_hpp
