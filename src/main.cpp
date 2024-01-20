#include <iostream>
#include "SDL_Headers.h"
#include <string>
#include <vector>
#include <cmath>

#include "gameMenu.hpp"

using namespace std;

bool init()
{
  if (SDL_Init(SDL_INIT_VIDEO) > 0)
    std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
  if (!(IMG_Init(IMG_INIT_PNG)))
    std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
  //  if (!(TTF_Init()))
  //    std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  return true;
}

bool SDLinit = init();

const int WIDTH = 750, HIGHT = 1000;

int main(int argc, char const *argv[])
{
  // initializeSDL
  if (!SDLinit)
    return 1;
  SDL_Window *window = SDL_CreateWindow("Bouncing Balls Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  bool running = true;

  button startButton = {WIDTH / 2 - 100, HIGHT / 2 - 50, 200, 100, 0, 0, 0, 255, false, false};

  while (running)
  {
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    drawButton(renderer, startButton, event);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000 / 60);
    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_KEYDOWN)
      {
        int key = event.key.keysym.sym;
        if (key == SDLK_ESCAPE)
          running = false;
      }
      if (event.type == SDL_QUIT)
        running = false;
    }
  }
  return 0;
}
