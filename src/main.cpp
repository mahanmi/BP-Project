#include <iostream>
#include "SDL_Headers.h"
#include <string>
#include <vector>
#include <cmath>
#include <string>

#include "gameMenu.hpp"
#include "leaderboard.hpp"
#include "game.hpp"

using namespace std;

bool init()
{
  if (SDL_Init(SDL_INIT_VIDEO) > 0)
    std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
  if (!(IMG_Init(IMG_INIT_PNG)))
    std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
  if (!(TTF_Init()))
    std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  cout << "Game Started" << endl;
  return true;
}

bool SDLinit = init();

bool quitGame(SDL_Event event)
{
  if (event.type == SDL_QUIT)
    return true;
  return false;
}

bool goBack(SDL_Event event)
{
  if (event.type == SDL_KEYDOWN)
    if (event.key.keysym.sym == SDLK_ESCAPE)
      return true;
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

void render(SDL_Renderer *renderer)
{
  SDL_RenderPresent(renderer);
  SDL_Delay(1000 / 60);
  SDL_RenderClear(renderer);
}

const int WIDTH = 625, HIGHT = 1000;

SDL_Window *window = SDL_CreateWindow("Bouncing Balls Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
SDL_Event event;

SDL_Texture *Background = IMG_LoadTexture(renderer, "assets/Menu/BG.png");
SDL_Texture *StartScreen = IMG_LoadTexture(renderer, "assets/Menu/StartingScreen.png");

TTF_Font *font45 = TTF_OpenFont("assets/Fonts/Poppins-Bold.ttf", 45);

int main(int argc, char const *argv[])
{
  if (!SDLinit)
    return 1;

  bool running = true;

  button close = {55, 58, 20, 20, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Menu/close.png"), IMG_LoadTexture(renderer, "assets/Menu/closeHovered.png"), 0};
  button back = {55, 58, 20, 20, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Menu/Back.png"), IMG_LoadTexture(renderer, "assets/Menu/BackHovered.png"), 0};
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
      IMG_LoadTexture(renderer, "assets/Menu/PlayButton.png"),
      IMG_LoadTexture(renderer, "assets/Menu/PlayButtonHovered.png"),
      IMG_LoadTexture(renderer, "assets/Menu/PlayButtonClicked.png")};
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
      IMG_LoadTexture(renderer, "assets/Menu/LeaderboardButton.png"),
      IMG_LoadTexture(renderer, "assets/Menu/LeaderboardButtonHovered.png"),
      IMG_LoadTexture(renderer, "assets/Menu/LeaderboardButtonClicked.png")};
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
      IMG_LoadTexture(renderer, "assets/Menu/SettingsButton.png"),
      IMG_LoadTexture(renderer, "assets/Menu/SettingsButtonHovered.png"),
      IMG_LoadTexture(renderer, "assets/Menu/SettingsButtonClicked.png")};
  button timer = {151, 54, 63, 291, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Leaderboard/LeaderboardTimer.png"), 0, 0};
  button classic = {151, 54, 234, 291, 0, 0, 0, 0, 0, true, false, IMG_LoadTexture(renderer, "assets/Leaderboard/LeaderboardClassic.png"), 0, 0};
  button infinite = {151, 54, 405, 291, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Leaderboard/LeaderboardInfinite.png"), 0, 0};

  vector<player> players;
  getLeaderboard(players);
  printPlayers(players);

  showStartScreen(renderer, StartScreen, event);

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (quitGame(event) || close.wasClicked)
        quit(running);
      SDL_RenderCopy(renderer, Background, 0, 0);
      drawButton(renderer, play, event);
      drawButton(renderer, leaderboard, event);
      drawButton(renderer, settings, event);
      drawButton(renderer, close, event);
      render(renderer);
      /* while (play.wasClicked)
      {
      } */
      while (leaderboard.wasClicked)
      {
        string sort;
        back.wasClicked = false;
        if (classic.wasClicked)
        {
          sortPlayersBasedOnClassicScore(players);
          sort = "Classic";
        }
        while (classic.wasClicked && leaderboard.wasClicked)
        {
          while (SDL_PollEvent(&event))
          {
            if (quitGame(event))
            {
              leaderboard.wasClicked = false;
              quit(running);
              break;
            }

            if (goBack(event) || back.wasClicked)
              leaderboard.wasClicked = false;

            if (isClicked(event, timer.x, timer.y, timer.w, timer.h, timer.wasClicked))
              classic.wasClicked = false, timer.wasClicked = true;
            if (isClicked(event, infinite.x, infinite.y, infinite.w, infinite.h, infinite.wasClicked))
              classic.wasClicked = false, infinite.wasClicked = true;
            drawLeaderboard(players, renderer, classic.image, font45, sort);
            drawButton(renderer, back, event);
            render(renderer);
          }
        }
        if (timer.wasClicked)
        {
          sortPlayersBasedOnTimerScore(players);
          sort = "Timer";
        }
        while (timer.wasClicked && leaderboard.wasClicked)
        {
          while (SDL_PollEvent(&event))
          {
            if (quitGame(event))
            {
              leaderboard.wasClicked = false;
              quit(running);
              break;
            }

            if (goBack(event) || back.wasClicked)
              leaderboard.wasClicked = false;

            if (isClicked(event, classic.x, classic.y, classic.w, classic.h, classic.wasClicked))
              timer.wasClicked = false, classic.wasClicked = true;
            if (isClicked(event, infinite.x, infinite.y, infinite.w, infinite.h, infinite.wasClicked))
              timer.wasClicked = false, infinite.wasClicked = true;
            drawLeaderboard(players, renderer, timer.image, font45, sort);
            drawButton(renderer, back, event);
            render(renderer);
          }
        }
        if (infinite.wasClicked)
        {
          sortPlayersBasedOnInfiniteScore(players);
          sort = "Infinite";
        }
        while (infinite.wasClicked && leaderboard.wasClicked)
        {
          while (SDL_PollEvent(&event))
          {
            if (quitGame(event))
            {
              leaderboard.wasClicked = false;
              quit(running);
              break;
            }

            if (goBack(event) || back.wasClicked)
              leaderboard.wasClicked = false;

            if (isClicked(event, classic.x, classic.y, classic.w, classic.h, classic.wasClicked))
              infinite.wasClicked = false, classic.wasClicked = true;
            if (isClicked(event, timer.x, timer.y, timer.w, timer.h, timer.wasClicked))
              infinite.wasClicked = false, timer.wasClicked = true;
            drawLeaderboard(players, renderer, infinite.image, font45, sort);
            drawButton(renderer, back, event);
            render(renderer);
          }
        }
      }
      /* while (settings.wasClicked)
      {
      } */
    }
  }
  return 0;
}
