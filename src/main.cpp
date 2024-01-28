#include <iostream>
#include "SDL_Headers.h"
#include <string>
#include <vector>
#include <cmath>
#include <string>

#include "gameMenu.hpp"
#include "leaderboard.hpp"

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

Mix_Chunk *click = Mix_LoadWAV("/Users/mahan/Documents/Github/BP-Project/assets/Sounds/click.mp3");
Mix_Chunk *hover = Mix_LoadWAV("/Users/mahan/Documents/Github/BP-Project/assets/Sounds/hover.mp3");

TTF_Font *font45 = TTF_OpenFont("assets/Fonts/Poppins-Bold.ttf", 45);

int main(int argc, char const *argv[])
{
  if (!SDLinit)
    return 1;

  int theme = 0;

  int volume = 60;
  Mix_Volume(-1, volume);

  bool running = true;

  button close = {55, 58, 20, 20, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Menu/close.png"), IMG_LoadTexture(renderer, "assets/Menu/closeHovered.png"), 0, hover, click};
  button back = {55, 58, 20, 20, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Menu/Back.png"), IMG_LoadTexture(renderer, "assets/Menu/BackHovered.png"), 0, hover, click};
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
      IMG_LoadTexture(renderer, "assets/Menu/PlayButtonClicked.png"),
      hover,
      click};
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
      IMG_LoadTexture(renderer, "assets/Menu/LeaderboardButtonClicked.png"),
      hover,
      click};
  button settings = {387, 87, 113, 704, 0, 0, 0, 255, 6, false, false, IMG_LoadTexture(renderer, "assets/Menu/SettingsButton.png"), IMG_LoadTexture(renderer, "assets/Menu/SettingsButtonHovered.png"), IMG_LoadTexture(renderer, "assets/Menu/SettingsButtonClicked.png"), hover, click};
  button timer = {151, 54, 63, 291, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Leaderboard/LeaderboardTimer.png"), 0, 0, 0, click};
  button classic = {151, 54, 234, 291, 0, 0, 0, 0, 0, true, false, IMG_LoadTexture(renderer, "assets/Leaderboard/LeaderboardClassic.png"), 0, 0, 0, click};
  button infinite = {151, 54, 405, 291, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Leaderboard/LeaderboardInfinite.png"), 0, 0, 0, click};

  vector<player> players;
  getLeaderboard(players);

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
            {
              leaderboard.wasClicked = false;
              break;
            }

            if (isClicked(event, timer.x, timer.y, timer.w, timer.h, timer.wasClicked, timer.clickSound))
              classic.wasClicked = false, timer.wasClicked = true;
            if (isClicked(event, infinite.x, infinite.y, infinite.w, infinite.h, infinite.wasClicked, infinite.clickSound))
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
            {
              leaderboard.wasClicked = false;
              break;
            }

            if (isClicked(event, classic.x, classic.y, classic.w, classic.h, classic.wasClicked, classic.clickSound))
              timer.wasClicked = false, classic.wasClicked = true;
            if (isClicked(event, infinite.x, infinite.y, infinite.w, infinite.h, infinite.wasClicked, infinite.clickSound))
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
            {
              leaderboard.wasClicked = false;
              break;
            }

            if (isClicked(event, classic.x, classic.y, classic.w, classic.h, classic.wasClicked, classic.clickSound))
              infinite.wasClicked = false, classic.wasClicked = true;
            if (isClicked(event, timer.x, timer.y, timer.w, timer.h, timer.wasClicked, timer.clickSound))
              infinite.wasClicked = false, timer.wasClicked = true;
            drawLeaderboard(players, renderer, infinite.image, font45, sort);
            drawButton(renderer, back, event);
            render(renderer);
          }
        }
      }

      if (settings.wasClicked)
      {
        SDL_Texture *SettingsBG = IMG_LoadTexture(renderer, "assets/Settings/Settings.png");
        back.wasClicked = false;
        vector<SDL_Texture *> soundBar = {IMG_LoadTexture(renderer, "assets/Settings/sound0.png"), IMG_LoadTexture(renderer, "assets/Settings/sound20.png"), IMG_LoadTexture(renderer, "assets/Settings/sound40.png"), IMG_LoadTexture(renderer, "assets/Settings/sound60.png"), IMG_LoadTexture(renderer, "assets/Settings/sound80.png"), IMG_LoadTexture(renderer, "assets/Settings/sound100.png")};

        vector<button> adjustButtons = {
            {39, 46, 218, 226, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/L-SFXHovered.png"), 0, hover, click},
            {39, 46, 530, 226, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/R-SFXHovered.png"), 0, hover, click},
            {39, 46, 218, 324, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/L-MusicHovered.png"), 0, hover, click},
            {39, 46, 530, 324, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/R-MusicHovered.png"), 0, hover, click},
            {58, 68, 109, 663, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-THEME.png"), IMG_LoadTexture(renderer, "assets/Settings/L-THEMEHovered.png"), 0, hover, click},
            {58, 68, 512, 663, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-THEME.png"), IMG_LoadTexture(renderer, "assets/Settings/R-THEMEHovered.png"), 0, hover, click},
        };

        vector<SDL_Texture *> themes = {IMG_LoadTexture(renderer, "assets/Settings/Theme1.png"), IMG_LoadTexture(renderer, "assets/Settings/Theme2.png"), IMG_LoadTexture(renderer, "assets/Settings/Theme3.png")};

        while (settings.wasClicked)
        {

          while (SDL_PollEvent(&event))
          {
            if (quitGame(event))
            {
              settings.wasClicked = false;
              quit(running);
              break;
            }

            if (goBack(event) || back.wasClicked)
            {
              settings.wasClicked = false;
              break;
            }

            SDL_RenderCopy(renderer, SettingsBG, 0, 0);

            drawButton(renderer, adjustButtons[0], event);
            drawButton(renderer, adjustButtons[1], event);
            drawButton(renderer, adjustButtons[2], event);
            drawButton(renderer, adjustButtons[3], event);
            drawButton(renderer, adjustButtons[4], event);
            drawButton(renderer, adjustButtons[5], event);

            if (adjustButtons[1].wasClicked && volume < 100)
            {
              volume += 20;
              Mix_Volume(-1, volume);
              adjustButtons[1].wasClicked = false;
            }
            else if (adjustButtons[0].wasClicked && volume > 0)
            {
              volume -= 20;
              Mix_Volume(-1, volume);
              adjustButtons[0].wasClicked = false;
            }

            SDL_Rect volumeBar = {273, 225, 222, 43};
            SDL_RenderCopy(renderer, soundBar[volume / 20], 0, &volumeBar);

            if (adjustButtons[5].wasClicked)
            {
              theme++;
              adjustButtons[5].wasClicked = false;
            }
            else if (adjustButtons[4].wasClicked)
            {
              theme += themes.size();
              theme--;
              adjustButtons[4].wasClicked = false;
            }

            theme = theme % themes.size();

            SDL_Rect themeRect = {184, 482, 260, 416};
            SDL_RenderCopy(renderer, themes[theme], 0, &themeRect);

            drawButton(renderer, back, event);
            render(renderer);
          }
        }
      }
    }
  }
  return 0;
}
