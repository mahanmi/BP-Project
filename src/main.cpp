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
  cout << "Game Started" << endl;
  return true;
}

bool SDLinit = init();

bool quitGame(SDL_Event event)
{
  if (event.type == SDL_QUIT)
    return true;
  if (event.type == SDL_KEYDOWN)
  {
    int key = event.key.keysym.sym;
    if (key == SDLK_ESCAPE)
      return true;
  }
  return false;
}

void quit(bool &running)
{
  running = false;
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  cout << "Game Exited" << endl;
}

const int WIDTH = 625, HIGHT = 1000;

SDL_Window *window = SDL_CreateWindow("Bouncing Balls Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
SDL_Event event;

SDL_Texture *Background = IMG_LoadTexture(renderer, "assets/BG.png");
SDL_Texture *StartScreen = IMG_LoadTexture(renderer, "assets/StartingScreen.png");

int main(int argc, char const *argv[])
{
  if (!SDLinit)
    return 1;

  bool running = true;

  button play = {
      387,
      87,
      113,
      370,
      0,
      0,
      0,
      255,
      6,
      false,
      false,
      IMG_LoadTexture(renderer, "assets/PlayButton.png"),
      IMG_LoadTexture(renderer, "assets/PlayButtonHovered.png"),
      IMG_LoadTexture(renderer, "assets/PlayButtonClicked.png")};
  button leaderboard = {
      387,
      87,
      113,
      537,
      0,
      0,
      0,
      255,
      6,
      false,
      false,
      IMG_LoadTexture(renderer, "assets/LeaderboardButton.png"),
      IMG_LoadTexture(renderer, "assets/LeaderboardButtonHovered.png"),
      IMG_LoadTexture(renderer, "assets/LeaderboardButtonClicked.png")};
  button settings = {
      387,
      87,
      113,
      704,
      0,
      0,
      0,
      255,
      6,
      false,
      false,
      IMG_LoadTexture(renderer, "assets/SettingsButton.png"),
      IMG_LoadTexture(renderer, "assets/SettingsButtonHovered.png"),
      IMG_LoadTexture(renderer, "assets/SettingsButtonClicked.png")};

  showStartScreen(renderer, StartScreen, event);

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (quitGame(event))
        quit(running);
      SDL_RenderCopy(renderer, Background, NULL, NULL);
      drawButton(renderer, play, event);
      drawButton(renderer, leaderboard, event);
      drawButton(renderer, settings, event);
      SDL_RenderPresent(renderer);
      SDL_Delay(1000 / 60);
      SDL_RenderClear(renderer);
    }
  }
  return 0;
}
