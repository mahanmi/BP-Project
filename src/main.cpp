#include <iostream>
#include "SDL_Headers.h"
#include <string>
#include <vector>
#include <cmath>
#include <string>

#include "gameMenu.hpp"
#include "leaderboard.hpp"
#include "settings.hpp"
#include "gameMode.hpp"

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

int musicIndex = 1;

vector<music> musicList = {{"Off", ""}, {"Piano", "assets/Sounds/pianoMusic.mp3"}, {"Bache Nane", "assets/Sounds/MohsenLorestani-BacheNane.mp3"}};

Mix_Music *musicSound = Mix_LoadMUS(musicList[musicIndex].path.c_str());
Mix_Chunk *click = Mix_LoadWAV("assets/Sounds/click.mp3");
Mix_Chunk *hover = Mix_LoadWAV("assets/Sounds/hover.mp3");

TTF_Font *Leaderboard = TTF_OpenFont("assets/Fonts/Poppins-Bold.ttf", 45);
TTF_Font *Settings = TTF_OpenFont("assets/Fonts/Digitalt.ttf", 38);
TTF_Font *name = TTF_OpenFont("assets/Fonts/Digitalt.ttf", 28);

int main(int argc, char const *argv[])
{
  if (!SDLinit)
    return 1;

  showStartScreen(renderer, StartScreen, event);

  int theme = 0;

  int volume = 60;
  Mix_Volume(-1, volume);
  Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);

  string username = "";
  int playerIndex = -1;

  vector<player> players;
  getLeaderboard(players);

  bool running = true;

  Mix_PlayMusic(musicSound, -1);

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

  SDL_Delay(1000);

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
      if (play.wasClicked)
      {
        SDL_Texture *PlayBG = IMG_LoadTexture(renderer, "assets/GameMode/GameMode.png");
        button timerGM = {142, 270, 64, 432, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/GameMode/Timer.png"), IMG_LoadTexture(renderer, "assets/GameMode/TimerHover.png"), IMG_LoadTexture(renderer, "assets/GameMode/timerSelected.png"), hover, click, false};
        button classicGM = {142, 270, 243, 432, 0, 0, 0, 0, 0, true, false, IMG_LoadTexture(renderer, "assets/GameMode/Classic.png"), IMG_LoadTexture(renderer, "assets/GameMode/ClassicHover.png"), IMG_LoadTexture(renderer, "assets/GameMode/classicSelected.png"), hover, click, true};
        button infiniteGM = {142, 270, 422, 432, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/GameMode/Infinite.png"), IMG_LoadTexture(renderer, "assets/GameMode/InfiniteHover.png"), IMG_LoadTexture(renderer, "assets/GameMode/InfiniteSelected.png"), hover, click, false};
        button start = {198, 91, 219, 829, 0, 0, 0, 0, 5, false, false, IMG_LoadTexture(renderer, "assets/GameMode/startButton.png"), IMG_LoadTexture(renderer, "assets/GameMode/startButtonHover.png"), IMG_LoadTexture(renderer, "assets/GameMode/startButtonClick.png"), hover, click};
        button inputBox = {319, 38, 236, 296, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/GameMode/inputBox.png"), IMG_LoadTexture(renderer, "assets/GameMode/inputBoxHover.png"), IMG_LoadTexture(renderer, "assets/GameMode/inputBoxSelected.png"), hover, click};
        while (play.wasClicked)
        {
          while (SDL_PollEvent(&event) && play.wasClicked)
          {
            if (quitGame(event))
            {
              play.wasClicked = false;
              quit(running);
              break;
            }
            if (goBack(event) || back.wasClicked)
            {
              play.wasClicked = false;
              break;
            }

            if (isClicked(event, timerGM.x, timerGM.y, timerGM.w, timerGM.h, timerGM.wasClicked, timerGM.clickSound))
            {
              timerGM.isSelected = true;
              classicGM.isSelected = false;
              infiniteGM.isSelected = false;
            }
            else if (isClicked(event, classicGM.x, classicGM.y, classicGM.w, classicGM.h, classicGM.wasClicked, classicGM.clickSound))
            {
              timerGM.isSelected = false;
              classicGM.isSelected = true;
              infiniteGM.isSelected = false;
            }
            else if (isClicked(event, infiniteGM.x, infiniteGM.y, infiniteGM.w, infiniteGM.h, infiniteGM.wasClicked, infiniteGM.clickSound))
            {
              timerGM.isSelected = false;
              classicGM.isSelected = false;
              infiniteGM.isSelected = true;
            }

            if (timerGM.isSelected)
            {
              timerGM.image = IMG_LoadTexture(renderer, "assets/GameMode/TimerSelected.png");
              timerGM.hoveredImage = IMG_LoadTexture(renderer, "assets/GameMode/TimerSelected.png");
              classicGM.image = IMG_LoadTexture(renderer, "assets/GameMode/Classic.png");
              infiniteGM.image = IMG_LoadTexture(renderer, "assets/GameMode/Infinite.png");
            }
            else if (classicGM.isSelected)
            {
              timerGM.image = IMG_LoadTexture(renderer, "assets/GameMode/Timer.png");
              classicGM.image = IMG_LoadTexture(renderer, "assets/GameMode/ClassicSelected.png");
              classicGM.hoveredImage = IMG_LoadTexture(renderer, "assets/GameMode/ClassicSelected.png");
              infiniteGM.image = IMG_LoadTexture(renderer, "assets/GameMode/Infinite.png");
            }
            else if (infiniteGM.isSelected)
            {
              timerGM.image = IMG_LoadTexture(renderer, "assets/GameMode/Timer.png");
              classicGM.image = IMG_LoadTexture(renderer, "assets/GameMode/Classic.png");
              infiniteGM.image = IMG_LoadTexture(renderer, "assets/GameMode/InfiniteSelected.png");
              infiniteGM.hoveredImage = IMG_LoadTexture(renderer, "assets/GameMode/InfiniteSelected.png");
            }

            SDL_RenderCopy(renderer, PlayBG, 0, 0);

            drawButton(renderer, timerGM, event);
            drawButton(renderer, classicGM, event);
            drawButton(renderer, infiniteGM, event);
            drawButton(renderer, start, event);
            drawButton(renderer, back, event);
            drawButton(renderer, inputBox, event);
            showUserInput(renderer, Settings, username, 250, 295);
            render(renderer);

            if (inputBox.wasClicked)
            {
              inputBox.image = IMG_LoadTexture(renderer, "assets/GameMode/inputBoxSelected.png");
              inputBox.isSelected = true;
              while (inputBox.isSelected)
              {
                while (SDL_PollEvent(&event) && inputBox.isSelected)
                {
                  if (event.type == SDL_KEYDOWN)
                  {
                    if (get_current_time(-3000) > 3)
                    {
                      switch (event.key.keysym.sym)
                      {
                      case SDLK_SPACE:
                        inputBox.isSelected = false;
                        break;
                      case SDLK_BACKSPACE:
                        if (username.size() > 0)
                          username.pop_back();
                        break;
                      default:
                        if (event.key.keysym.sym >= 97 && event.key.keysym.sym <= 122 && username.size() <= 10)
                          username += event.key.keysym.sym;

                        break;
                      }
                    }
                  }
                  SDL_RenderCopy(renderer, PlayBG, 0, 0);
                  drawButton(renderer, inputBox, event);
                  drawButton(renderer, timerGM, event);
                  drawButton(renderer, classicGM, event);
                  drawButton(renderer, infiniteGM, event);
                  drawButton(renderer, start, event);
                  drawButton(renderer, back, event);
                  showUserInput(renderer, Settings, username, 250, 295);
                  render(renderer);
                  if (back.wasClicked || timerGM.wasClicked || classicGM.wasClicked || infiniteGM.wasClicked || start.wasClicked || !inputBox.isSelected)
                  {
                    bool playerFound = false;
                    for (int i = 0; i < players.size(); i++)
                    {
                      if (players[i].name == username)
                      {
                        playerFound = true;
                        playerIndex = i;
                        cout << "Player Found! Welcome Back " << players[i].name << endl;
                        break;
                      }
                    }
                    if (!playerFound)
                    {
                      player newPlayer = {username, 0, 0, 0};
                      players.push_back(newPlayer);
                      addNewPlayerToFile(newPlayer);
                      playerIndex = players.size() - 1;
                      cout << "New Player Added : " << players[playerIndex].name << endl;
                    }
                    inputBox.image = IMG_LoadTexture(renderer, "assets/GameMode/inputBox.png");
                    inputBox.isSelected = false;
                    break;
                  }
                }
              }
            }

            if (start.wasClicked && playerIndex != -1) // this should run the game
            {
              start.wasClicked = false;

              SDL_RenderClear(renderer);

              if (timerGM.isSelected)
              {
                while (timerGM.isSelected)
                {
                  while (SDL_PollEvent(&event) && timerGM.isSelected)
                  {
                    if (quitGame(event))
                    {
                      timerGM.isSelected = false;
                      quit(running);
                      break;
                    }
                    if (goBack(event) || back.wasClicked)
                    {
                      timerGM.isSelected = false;
                      break;
                    }
                    SDL_RenderCopy(renderer, Background, 0, 0);
                    drawButton(renderer, back, event);
                    render(renderer);
                  }
                }
                timerGM.isSelected = true;
              }
              else if (classicGM.isSelected)
              {
                while (classicGM.isSelected)
                {
                  while (SDL_PollEvent(&event) && classicGM.isSelected)
                  {
                    if (quitGame(event))
                    {
                      classicGM.isSelected = false;
                      quit(running);
                      break;
                    }
                    if (goBack(event) || back.wasClicked)
                    {
                      classicGM.isSelected = false;
                      break;
                    }
                    SDL_RenderCopy(renderer, Background, 0, 0);
                    drawButton(renderer, back, event);
                    render(renderer);
                  }
                }
                classicGM.isSelected = true;
              }
              else if (infiniteGM.isSelected)
              {
                while (infiniteGM.isSelected)
                {
                  while (SDL_PollEvent(&event) && infiniteGM.isSelected)
                  {
                    if (quitGame(event))
                    {
                      infiniteGM.isSelected = false;
                      quit(running);
                      break;
                    }
                    if (goBack(event) || back.wasClicked)
                    {
                      infiniteGM.isSelected = false;
                      break;
                    }
                    SDL_RenderCopy(renderer, Background, 0, 0);
                    drawButton(renderer, back, event);
                    render(renderer);
                  }
                }
                infiniteGM.isSelected = true;
              }

              back.wasClicked = false;
              SDL_RenderCopy(renderer, PlayBG, 0, 0);
              drawButton(renderer, inputBox, event);
              showUserInput(renderer, Settings, username, 250, 295);
              drawButton(renderer, timerGM, event);
              drawButton(renderer, classicGM, event);
              drawButton(renderer, infiniteGM, event);
              drawButton(renderer, start, event);
              drawButton(renderer, back, event);
              render(renderer);
            }
          }
        }
      }

      if (leaderboard.wasClicked)
      {
        string sort;
        players.clear();
        getLeaderboard(players);
        while (leaderboard.wasClicked)
        {
          if (classic.wasClicked)
          {
            sortPlayersBasedOnClassicScore(players);
            sort = "Classic";
          }
          while (classic.wasClicked && leaderboard.wasClicked)
          {
            while (SDL_PollEvent(&event) && leaderboard.wasClicked)
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
              drawLeaderboard(players, renderer, classic.image, Leaderboard, sort);
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
            while (SDL_PollEvent(&event) && leaderboard.wasClicked)
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
              drawLeaderboard(players, renderer, timer.image, Leaderboard, sort);
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
            while (SDL_PollEvent(&event) && leaderboard.wasClicked)
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
              drawLeaderboard(players, renderer, infinite.image, Leaderboard, sort);
              drawButton(renderer, back, event);
              render(renderer);
            }
          }
        }
      }

      if (settings.wasClicked)
      {
        SDL_Texture *SettingsBG = IMG_LoadTexture(renderer, "assets/Settings/Settings.png");

        vector<SDL_Texture *> soundBar = {IMG_LoadTexture(renderer, "assets/Settings/sound0.png"), IMG_LoadTexture(renderer, "assets/Settings/sound20.png"), IMG_LoadTexture(renderer, "assets/Settings/sound40.png"), IMG_LoadTexture(renderer, "assets/Settings/sound60.png"), IMG_LoadTexture(renderer, "assets/Settings/sound80.png"), IMG_LoadTexture(renderer, "assets/Settings/sound100.png")};

        vector<button> adjustButtons = {
            {39, 46, 218, 226, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/L-SFXHovered.png"), 0, hover, click},
            {39, 46, 530, 226, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/R-SFXHovered.png"), 0, hover, click},
            {39, 46, 218, 324, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/L-MusicHovered.png"), 0, hover, click},
            {39, 46, 530, 324, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/R-MusicHovered.png"), 0, hover, click},
            {58, 68, 90, 663, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-THEME.png"), IMG_LoadTexture(renderer, "assets/Settings/L-THEMEHovered.png"), 0, hover, click},
            {58, 68, 480, 663, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-THEME.png"), IMG_LoadTexture(renderer, "assets/Settings/R-THEMEHovered.png"), 0, hover, click},
        };

        vector<SDL_Texture *> themes = {IMG_LoadTexture(renderer, "assets/Settings/Theme1.png"), IMG_LoadTexture(renderer, "assets/Settings/Theme2.png"), IMG_LoadTexture(renderer, "assets/Settings/Theme3.png")};

        while (settings.wasClicked)
        {

          while (SDL_PollEvent(&event) && settings.wasClicked)
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
              Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
              adjustButtons[1].wasClicked = false;
            }
            else if (adjustButtons[0].wasClicked && volume > 0)
            {
              volume -= 20;
              Mix_Volume(-1, volume);
              Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
              adjustButtons[0].wasClicked = false;
            }

            SDL_Rect volumeBar = {285, 225, 222, 43};
            SDL_RenderCopy(renderer, soundBar[volume / 20], 0, &volumeBar);

            setMusic(renderer, adjustButtons[3].wasClicked, adjustButtons[2].wasClicked, musicList, musicIndex, musicSound, Settings, 285, 323);

            if (adjustButtons[3].wasClicked || adjustButtons[2].wasClicked)
            {
              Mix_PlayMusic(musicSound, -1);
              adjustButtons[3].wasClicked = false;
              adjustButtons[2].wasClicked = false;
            }

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

      if (back.wasClicked)
      {
        SDL_RenderCopy(renderer, Background, 0, 0);
        drawButton(renderer, play, event);
        drawButton(renderer, leaderboard, event);
        drawButton(renderer, settings, event);
        drawButton(renderer, close, event);
        SDL_RenderPresent(renderer);
        back.wasClicked = false;
      }
    }
  }
  return 0;
}
