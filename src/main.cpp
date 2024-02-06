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

const int WIDTH = 625, HIGHT = 1000;
SDL_Window *window = SDL_CreateWindow("Bouncing Balls Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
SDL_Event event;
struct ball
{
  int i, j, r = 25;
  int color;
  float x, y;
  bool isEmpty, shouldStick;
  int stickCheck;
};

int d = 20;
float dx, dy;
float dy_initial = 0.25, dy_fallingBall = 10;
int ballRadius = 25;
int lines, realLines;
int columns = WIDTH / (2 * ballRadius);
int stick = 0;
int score = 100;
int crashed_score = 0;
int end_time;
bool pause = false, slowMotion = false;
bool win = false, lose = false;

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

vector<vector<ball>> balls;

bool is_crash_ball_moved = false;
bool is_crash_ball_crashed = false;

vector<ball> crashed, fallingBall, explode;

ball crash_balls[2];
bool crash_ball_color[4] = {1, 1, 1, 1};

int x_mouse, y_mouse;

SDL_Texture *Background = IMG_LoadTexture(renderer, "assets/Menu/BG.png");
SDL_Texture *StartScreen = IMG_LoadTexture(renderer, "assets/Menu/StartingScreen.png");
SDL_Texture *SettingsBG = IMG_LoadTexture(renderer, "assets/Settings/Settings.png");
SDL_Texture *on = IMG_LoadTexture(renderer, "assets/Settings/ON.png");
SDL_Texture *off = IMG_LoadTexture(renderer, "assets/Settings/OFF.png");

SDL_Texture *ball1 = IMG_LoadTexture(renderer, "assets/Game/Balls/1.png");
SDL_Texture *ball2 = IMG_LoadTexture(renderer, "assets/Game/Balls/2.png");
SDL_Texture *ball3 = IMG_LoadTexture(renderer, "assets/Game/Balls/3.png");
SDL_Texture *ball4 = IMG_LoadTexture(renderer, "assets/Game/Balls/4.png");
SDL_Texture *ball5 = IMG_LoadTexture(renderer, "assets/Game/Balls/5.png");
SDL_Texture *ball6 = IMG_LoadTexture(renderer, "assets/Game/Balls/6.png");
SDL_Texture *ball7 = IMG_LoadTexture(renderer, "assets/Game/Balls/7.png");
SDL_Texture *ball8 = IMG_LoadTexture(renderer, "assets/Game/Balls/8.png");
SDL_Texture *ball9 = IMG_LoadTexture(renderer, "assets/Game/Balls/9.png");
SDL_Texture *ball10 = IMG_LoadTexture(renderer, "assets/Game/Balls/10.png");
SDL_Texture *ball11 = IMG_LoadTexture(renderer, "assets/Game/Balls/11.png");
SDL_Texture *ball12 = IMG_LoadTexture(renderer, "assets/Game/Balls/12.png");
SDL_Texture *ball13 = IMG_LoadTexture(renderer, "assets/Game/Balls/13.png");

SDL_Texture *cannon1 = IMG_LoadTexture(renderer, "assets/Game/cannon/cannon1.png");
SDL_Texture *cannon2 = IMG_LoadTexture(renderer, "assets/Game/cannon/cannon2.png");
SDL_Texture *cannon4 = IMG_LoadTexture(renderer, "assets/Game/cannon/cannon4.png");
SDL_Texture *cannon8 = IMG_LoadTexture(renderer, "assets/Game/cannon/cannon8.png");
SDL_Texture *cannon11 = IMG_LoadTexture(renderer, "assets/Game/cannon/cannon11.png");
SDL_Texture *cannon13 = IMG_LoadTexture(renderer, "assets/Game/cannon/cannon13.png");

SDL_Texture *scoreBox = IMG_LoadTexture(renderer, "assets/Game/score.png");
SDL_Texture *timeBox = IMG_LoadTexture(renderer, "assets/Game/time.png");

SDL_Texture *pauseMenu = IMG_LoadTexture(renderer, "assets/Game/pauseMenu.png");

vector<SDL_Texture *> soundBar = {IMG_LoadTexture(renderer, "assets/Settings/sound0.png"), IMG_LoadTexture(renderer, "assets/Settings/sound20.png"), IMG_LoadTexture(renderer, "assets/Settings/sound40.png"), IMG_LoadTexture(renderer, "assets/Settings/sound60.png"), IMG_LoadTexture(renderer, "assets/Settings/sound80.png"), IMG_LoadTexture(renderer, "assets/Settings/sound100.png")};

int musicIndex = 1;
bool sfx = true;

vector<music> musicList = {{"Off", ""}, {"Piano", "assets/Sounds/pianoMusic.mp3"}, {"Bache Nane", "assets/Sounds/MohsenLorestani-BacheNane.mp3"}, {"FE!N", "assets/Sounds/fein.mp3"}, {"Tehrono LA KON", "assets/Sounds/Tehrano_LA_Kon.mp3"}};

Mix_Music *musicSound = Mix_LoadMUS(musicList[musicIndex].path.c_str());
Mix_Chunk *click = Mix_LoadWAV("assets/Sounds/click.mp3");
Mix_Chunk *hover = Mix_LoadWAV("assets/Sounds/hover.mp3");
Mix_Chunk *crash = Mix_LoadWAV("assets/Sounds/crash.wav");
Mix_Chunk *explosion = Mix_LoadWAV("assets/Sounds/explosion.mp3");
Mix_Chunk *movement_swipe = Mix_LoadWAV("assets/Sounds/movement_swipe.mp3");

TTF_Font *Leaderboard = TTF_OpenFont("assets/Fonts/Poppins-Bold.ttf", 45);
TTF_Font *Settings = TTF_OpenFont("assets/Fonts/Digitalt.ttf", 38);
TTF_Font *name = TTF_OpenFont("assets/Fonts/Digitalt.ttf", 28);
TTF_Font *scoreTexture = TTF_OpenFont("assets/Fonts/Digitalt.ttf", 25);

button setting = {40, 43, 20, 20, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Game/settingsButton.png"), IMG_LoadTexture(renderer, "assets/Game/settingsButtonHovered.png"), IMG_LoadTexture(renderer, "assets/Game/settingsButtonHoveredClicked.png"), hover, click};

#include "game.hpp"

int main(int argv, char **args)
{
  if (!SDLinit)
    return 1;

  srand(time(0));

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
  button play = {387, 87, 113, 370, 0, 0, 0, 255, 6, false, false, IMG_LoadTexture(renderer, "assets/Menu/PlayButton.png"), IMG_LoadTexture(renderer, "assets/Menu/PlayButtonHovered.png"), IMG_LoadTexture(renderer, "assets/Menu/PlayButtonClicked.png"), hover, click};
  button leaderboard = {387, 87, 113, 537, 0, 0, 0, 255, 6, false, false, IMG_LoadTexture(renderer, "assets/Menu/LeaderboardButton.png"), IMG_LoadTexture(renderer, "assets/Menu/LeaderboardButtonHovered.png"), IMG_LoadTexture(renderer, "assets/Menu/LeaderboardButtonClicked.png"), hover, click};
  button settings = {387, 87, 113, 704, 0, 0, 0, 255, 6, false, false, IMG_LoadTexture(renderer, "assets/Menu/SettingsButton.png"), IMG_LoadTexture(renderer, "assets/Menu/SettingsButtonHovered.png"), IMG_LoadTexture(renderer, "assets/Menu/SettingsButtonClicked.png"), hover, click};
  button timer = {151, 54, 63, 291, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Leaderboard/LeaderboardTimer.png"), 0, 0, 0, click};
  button classic = {151, 54, 234, 291, 0, 0, 0, 0, 0, true, false, IMG_LoadTexture(renderer, "assets/Leaderboard/LeaderboardClassic.png"), 0, 0, 0, click};
  button infinite = {151, 54, 405, 291, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Leaderboard/LeaderboardInfinite.png"), 0, 0, 0, click};
  button home = {55, 58, 95, 20, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Game/home.png"), IMG_LoadTexture(renderer, "assets/Game/homeHovered.png"), IMG_LoadTexture(renderer, "assets/Game/homeClicked.png"), hover, click};

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

              for (int i = 0; i < players.size(); i++)
              {
                if (players[i].name == username)
                {
                  playerIndex = i;
                  break;
                }
              }

              SDL_Texture *GameBG;

              if (theme == 0)
              {
                GameBG = IMG_LoadTexture(renderer, "assets/Game/GameBG1.png");
              }
              else if (theme == 1)
              {
                GameBG = IMG_LoadTexture(renderer, "assets/Game/GameBG2.png");
              }
              else if (theme == 2)
              {
                GameBG = IMG_LoadTexture(renderer, "assets/Game/GameBG3.png");
              }

              SDL_RenderClear(renderer);

              for (int i = 0; i < 4; i++)
              {
                crash_ball_color[i] = true;
              }

              if (timerGM.isSelected)
              {
                balls.clear();
                crashed.clear();
                fallingBall.clear();

                end_time = 90 * 1000;

                lines = 20;
                score = 100;
                crashed_score = 0;
                win = false, lose = false;
                pause = false, slowMotion = false;

                initial_ball();

                for (int j = 0; j < columns; j++)
                  balls[0][j].isEmpty = false, balls[0][j].shouldStick = true, balls[0][j].stickCheck = 1;

                for (int i = 1; i < lines; i++)
                  for (int j = 0; j < columns; j++)
                    balls[i][j].isEmpty = false, balls[i][j].shouldStick = false, balls[i][j].stickCheck = 0;

                for (int i = 6; i < lines; i++)
                {
                  for (int j = 0; j < columns; j++)
                    balls[i][j].isEmpty = true;
                }

                if (rand() % 17 == 0)
                {
                  crash_balls[1].color = 11;
                }
                else
                {
                  switch (rand() % 4)
                  {
                  case 0:
                  {
                    crash_balls[1].color = 1;
                    break;
                  }
                  case 1:
                  {
                    crash_balls[1].color = 2;
                    break;
                  }
                  case 2:
                  {
                    crash_balls[1].color = 4;
                    break;
                  }
                  case 3:
                  {
                    crash_balls[1].color = 8;
                    break;
                  }
                  }
                }

                initial_crash_ball(renderer);
                draw_ball(renderer);

                Uint32 start_time = SDL_GetTicks();
                Uint32 current_time = SDL_GetTicks();
                Uint32 elapsed_time = current_time - start_time;

                while (timerGM.isSelected)
                {
                  while (!win && !lose)
                  {
                    if (quitGame(event))
                    {
                      timerGM.isSelected = false;
                      quit(running);
                      break;
                    }
                    if (goBack(event) || back.wasClicked || home.wasClicked)
                    {
                      timerGM.isSelected = false;
                      home.wasClicked = false;
                      back.wasClicked = false;
                      break;
                    }
                    if (setting.wasClicked)
                    {
                      vector<button> adjustButtons2 = {
                          {39, 46, 218, 414, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-VOLUME.png"), IMG_LoadTexture(renderer, "assets/Settings/L-VOLUMEHovered.png"), 0, hover, click},
                          {39, 46, 530, 414, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-VOLUME.png"), IMG_LoadTexture(renderer, "assets/Settings/R-VOLUMEHovered.png"), 0, hover, click},
                          {39, 46, 218, 538, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/L-MusicHovered.png"), 0, hover, click},
                          {39, 46, 530, 538, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/R-MusicHovered.png"), 0, hover, click},
                          {39, 46, 218, 475, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/L-SFXHovered.png"), IMG_LoadTexture(renderer, "assets/Settings/L-SFXHovered.png"), hover, click},
                          {39, 46, 530, 475, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/R-SFXHovered.png"), IMG_LoadTexture(renderer, "assets/Settings/R-SFXHovered.png"), hover, click}};
                      while (setting.wasClicked)
                      {
                        while (SDL_PollEvent(&event) && setting.wasClicked)
                        {
                          if (quitGame(event))
                          {
                            setting.wasClicked = false;
                            quit(running);
                            break;
                          }
                          if (goBack(event) || back.wasClicked || home.wasClicked)
                          {
                            setting.wasClicked = false;
                            back.wasClicked = false;
                            break;
                          }
                          SDL_RenderCopy(renderer, GameBG, 0, 0);

                          SDL_Rect pauseRect = {33, 363, 562, 275};

                          SDL_RenderCopy(renderer, pauseMenu, 0, &pauseRect);

                          drawButton(renderer, adjustButtons2[0], event);
                          drawButton(renderer, adjustButtons2[1], event);
                          drawButton(renderer, adjustButtons2[2], event);
                          drawButton(renderer, adjustButtons2[3], event);
                          drawButton(renderer, adjustButtons2[4], event);
                          drawButton(renderer, adjustButtons2[5], event);

                          if (adjustButtons2[1].wasClicked && volume < 100)
                          {
                            volume += 20;
                            if (sfx)
                              Mix_Volume(-1, volume);

                            Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
                            adjustButtons2[1].wasClicked = false;
                          }
                          else if (adjustButtons2[0].wasClicked && volume > 0)
                          {
                            volume -= 20;
                            if (sfx)
                              Mix_Volume(-1, volume);
                            Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
                            adjustButtons2[0].wasClicked = false;
                          }

                          if (adjustButtons2[4].wasClicked || adjustButtons2[5].wasClicked)
                          {
                            sfx = !sfx;
                            adjustButtons2[4].wasClicked = false, adjustButtons2[5].wasClicked = false;
                            if (sfx)
                              Mix_Volume(-1, volume);
                            else
                              Mix_Volume(-1, 0);
                          }
                          if (sfx)
                          {
                            SDL_Rect sfxRect = {363, 475, 42, 22};
                            SDL_RenderCopy(renderer, on, 0, &sfxRect);
                          }
                          else
                          {
                            SDL_Rect sfxRect = {349, 475, 55, 22};
                            SDL_RenderCopy(renderer, off, 0, &sfxRect);
                          }

                          SDL_Rect volumeBar = {285, 405, 222, 43};
                          SDL_RenderCopy(renderer, soundBar[volume / 20], 0, &volumeBar);

                          setMusic(renderer, adjustButtons2[3].wasClicked, adjustButtons2[2].wasClicked, musicList, musicIndex, musicSound, Settings, 285, 535);

                          if (adjustButtons2[3].wasClicked || adjustButtons2[2].wasClicked)
                          {
                            Mix_PlayMusic(musicSound, -1);
                            adjustButtons2[3].wasClicked = false;
                            adjustButtons2[2].wasClicked = false;
                          }

                          drawButton(renderer, home, event);
                          drawButton(renderer, back, event);
                          render(renderer);
                        }
                      }
                    }
                    current_time = SDL_GetTicks();
                    elapsed_time = current_time - start_time;

                    if (elapsed_time >= end_time)
                    {
                      lose = true;
                      break;
                    }

                    SDL_RenderCopy(renderer, GameBG, NULL, NULL);

                    if (slowMotion)
                      dy_initial = 0.1;

                    else
                      dy_initial = 0.25;

                    if (!pause && !is_crash_ball_moved)
                      for (int i = 0; i < lines + stick; i++)
                        for (int j = 0; j < columns; j++)
                          balls[i][j].y += dy_initial;

                    crashed_ball(renderer);
                    draw_ball(renderer);

                    if (event.type == SDL_KEYDOWN)
                    {
                      switch (event.key.keysym.sym)
                      {
                      case SDLK_p:
                        pause = !pause;

                      case SDLK_s:
                        slowMotion = !slowMotion;
                        swap(crash_balls[0].color, crash_balls[1].color);

                      case SDLK_SPACE:
                        if (!is_crash_ball_moved)
                          swap(crash_balls[0].color, crash_balls[1].color);
                      }
                    }
                    if (event.button.x > 0 && event.button.x < WIDTH && event.button.y > 0 && event.button.y < HIGHT)
                    {
                      x_mouse = event.button.x;
                      y_mouse = event.button.y;
                    }

                    showScore(timeScore(elapsed_time), 145, 930, scoreTexture);

                    showTime((end_time - elapsed_time) / 1000, 528, 930, scoreTexture);

                    drawButton(renderer, setting, event);

                    render(renderer);
                  }
                  if (win)
                  {
                    if (players[playerIndex].timerScore < timeScore(elapsed_time))
                    {
                      cout << "New High Score" << endl;
                      players[playerIndex].timerScore = timeScore(elapsed_time);
                      updateLeaderboard(players);
                    }
                    button OK = {150, 68, 242, 657, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Game/OKButton.png"), IMG_LoadTexture(renderer, "assets/Game/OKButtonHover.png"), IMG_LoadTexture(renderer, "assets/Game/OKButtonClicked.png"), hover, click};
                    cout << "You win" << endl;
                    SDL_RenderCopy(renderer, GameBG, 0, 0);
                    SDL_Texture *winText = IMG_LoadTexture(renderer, "assets/Game/win.png");
                    SDL_Rect winRect = {112, 250, 401, 513};
                    SDL_RenderCopy(renderer, winText, 0, &winRect);
                    showUserScore(renderer, name, score, 250, 495);
                    showUserScore(renderer, name, players[playerIndex].classicScore, 250, 595);
                    drawButton(renderer, OK, event);
                    render(renderer);
                    while (win)
                    {
                      while (SDL_PollEvent(&event) && win)
                      {
                        if (quitGame(event))
                        {
                          timerGM.isSelected = false;
                          quit(running);
                          break;
                        }
                        if (goBack(event) || OK.wasClicked)
                        {
                          OK.wasClicked = false;
                          timerGM.isSelected = false;
                          win = false;
                          break;
                        }
                        SDL_RenderCopy(renderer, GameBG, 0, 0);
                        SDL_Texture *winText = IMG_LoadTexture(renderer, "assets/Game/win.png");
                        SDL_Rect winRect = {112, 250, 401, 513};
                        SDL_RenderCopy(renderer, winText, 0, &winRect);
                        showUserScore(renderer, name, score, 250, 495);
                        showUserScore(renderer, name, players[playerIndex].classicScore, 250, 595);
                        drawButton(renderer, OK, event);
                        render(renderer);
                      }
                    }
                  }
                  else
                  {
                    if (players[playerIndex].timerScore < score)
                    {
                      cout << "New High Score" << endl;
                      players[playerIndex].timerScore = score;
                      updateLeaderboard(players);
                    }
                    button OK = {150, 68, 242, 657, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Game/OKButton.png"), IMG_LoadTexture(renderer, "assets/Game/OKButtonHover.png"), IMG_LoadTexture(renderer, "assets/Game/OKButtonClicked.png"), hover, click};
                    cout << "You lose" << endl;
                    SDL_RenderCopy(renderer, GameBG, 0, 0);
                    SDL_Texture *loseText = IMG_LoadTexture(renderer, "assets/Game/lose.png");
                    SDL_Rect loseRect = {112, 250, 401, 513};
                    SDL_RenderCopy(renderer, loseText, 0, &loseRect);
                    showUserScore(renderer, name, score, 250, 495);
                    showUserScore(renderer, name, players[playerIndex].timerScore, 250, 595);
                    drawButton(renderer, OK, event);
                    render(renderer);
                    while (lose)
                    {
                      while (SDL_PollEvent(&event) && lose)
                      {
                        if (quitGame(event))
                        {
                          timerGM.isSelected = false;
                          quit(running);
                          break;
                        }
                        if (goBack(event) || OK.wasClicked)
                        {
                          OK.wasClicked = false;
                          timerGM.isSelected = false;
                          lose = false;
                          break;
                        }
                        SDL_RenderCopy(renderer, GameBG, 0, 0);
                        SDL_Texture *loseText = IMG_LoadTexture(renderer, "assets/Game/lose.png");
                        SDL_Rect loseRect = {112, 250, 401, 513};
                        SDL_RenderCopy(renderer, loseText, 0, &loseRect);
                        showUserScore(renderer, name, score, 250, 495);
                        showUserScore(renderer, name, players[playerIndex].timerScore, 250, 595);
                        drawButton(renderer, OK, event);
                        render(renderer);
                      }
                    }
                  }
                }
              }
              else if (classicGM.isSelected)
              {
                balls.clear();
                crashed.clear();
                fallingBall.clear();

                lines = 20;
                score = 100;
                crashed_score = 0;
                win = false, lose = false;
                pause = false, slowMotion = false;

                initial_ball();
                for (int j = 0; j < columns; j++)
                  balls[0][j].isEmpty = false, balls[0][j].shouldStick = true, balls[0][j].stickCheck = 1;

                for (int i = 1; i < lines; i++)
                  for (int j = 0; j < columns; j++)
                    balls[i][j].isEmpty = false, balls[i][j].shouldStick = false, balls[i][j].stickCheck = 0;

                for (int i = 6; i < lines; i++)
                {
                  for (int j = 0; j < columns; j++)
                    balls[i][j].isEmpty = true;
                }

                if (rand() % 17 == 0)
                {
                  crash_balls[1].color = 11;
                }
                else
                {
                  switch (rand() % 4)
                  {
                  case 0:
                  {
                    crash_balls[1].color = 1;
                    break;
                  }
                  case 1:
                  {
                    crash_balls[1].color = 2;
                    break;
                  }
                  case 2:
                  {
                    crash_balls[1].color = 4;
                    break;
                  }
                  case 3:
                  {
                    crash_balls[1].color = 8;
                    break;
                  }
                  }
                }
                initial_crash_ball(renderer);
                draw_ball(renderer);

                while (classicGM.isSelected)
                {
                  while (!win && !lose)
                  {
                    if (quitGame(event))
                    {
                      classicGM.isSelected = false;
                      quit(running);
                      break;
                    }
                    if (goBack(event) || back.wasClicked || home.wasClicked)
                    {
                      classicGM.isSelected = false;
                      home.wasClicked = false;
                      back.wasClicked = false;
                      break;
                    }
                    if (setting.wasClicked)
                    {
                      vector<button> adjustButtons2 = {
                          {39, 46, 218, 414, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-VOLUME.png"), IMG_LoadTexture(renderer, "assets/Settings/L-VOLUMEHovered.png"), 0, hover, click},
                          {39, 46, 530, 414, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-VOLUME.png"), IMG_LoadTexture(renderer, "assets/Settings/R-VOLUMEHovered.png"), 0, hover, click},
                          {39, 46, 218, 538, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/L-MusicHovered.png"), 0, hover, click},
                          {39, 46, 530, 538, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/R-MusicHovered.png"), 0, hover, click},
                          {39, 46, 218, 475, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/L-SFXHovered.png"), IMG_LoadTexture(renderer, "assets/Settings/L-SFXHovered.png"), hover, click},
                          {39, 46, 530, 475, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/R-SFXHovered.png"), IMG_LoadTexture(renderer, "assets/Settings/R-SFXHovered.png"), hover, click}};
                      while (setting.wasClicked)
                      {
                        while (SDL_PollEvent(&event) && setting.wasClicked)
                        {
                          if (quitGame(event))
                          {
                            setting.wasClicked = false;
                            quit(running);
                            break;
                          }
                          if (goBack(event) || back.wasClicked || home.wasClicked)
                          {
                            setting.wasClicked = false;
                            back.wasClicked = false;
                            break;
                          }
                          SDL_RenderCopy(renderer, GameBG, 0, 0);

                          SDL_Rect pauseRect = {33, 363, 562, 275};

                          SDL_RenderCopy(renderer, pauseMenu, 0, &pauseRect);

                          drawButton(renderer, adjustButtons2[0], event);
                          drawButton(renderer, adjustButtons2[1], event);
                          drawButton(renderer, adjustButtons2[2], event);
                          drawButton(renderer, adjustButtons2[3], event);
                          drawButton(renderer, adjustButtons2[4], event);
                          drawButton(renderer, adjustButtons2[5], event);

                          if (adjustButtons2[1].wasClicked && volume < 100)
                          {
                            volume += 20;
                            if (sfx)
                              Mix_Volume(-1, volume);

                            Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
                            adjustButtons2[1].wasClicked = false;
                          }
                          else if (adjustButtons2[0].wasClicked && volume > 0)
                          {
                            volume -= 20;
                            if (sfx)
                              Mix_Volume(-1, volume);
                            Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
                            adjustButtons2[0].wasClicked = false;
                          }

                          if (adjustButtons2[4].wasClicked || adjustButtons2[5].wasClicked)
                          {
                            sfx = !sfx;
                            adjustButtons2[4].wasClicked = false, adjustButtons2[5].wasClicked = false;
                            if (sfx)
                              Mix_Volume(-1, volume);
                            else
                              Mix_Volume(-1, 0);
                          }
                          if (sfx)
                          {
                            SDL_Rect sfxRect = {363, 475, 42, 22};
                            SDL_RenderCopy(renderer, on, 0, &sfxRect);
                          }
                          else
                          {
                            SDL_Rect sfxRect = {349, 475, 55, 22};
                            SDL_RenderCopy(renderer, off, 0, &sfxRect);
                          }

                          SDL_Rect volumeBar = {285, 405, 222, 43};
                          SDL_RenderCopy(renderer, soundBar[volume / 20], 0, &volumeBar);

                          setMusic(renderer, adjustButtons2[3].wasClicked, adjustButtons2[2].wasClicked, musicList, musicIndex, musicSound, Settings, 285, 535);

                          if (adjustButtons2[3].wasClicked || adjustButtons2[2].wasClicked)
                          {
                            Mix_PlayMusic(musicSound, -1);
                            adjustButtons2[3].wasClicked = false;
                            adjustButtons2[2].wasClicked = false;
                          }

                          drawButton(renderer, home, event);
                          drawButton(renderer, back, event);
                          render(renderer);
                        }
                      }
                    }

                    SDL_RenderCopy(renderer, GameBG, NULL, NULL);

                    if (slowMotion)
                      dy_initial = 0.1;
                    else
                      dy_initial = 0.25;

                    if (!pause && !is_crash_ball_moved)
                      for (int i = 0; i < lines + stick; i++)
                        for (int j = 0; j < columns; j++)
                          balls[i][j].y += dy_initial;

                    crashed_ball(renderer);
                    draw_ball(renderer);

                    if (event.type == SDL_KEYDOWN)
                    {
                      switch (event.key.keysym.sym)
                      {
                      case SDLK_p:
                        pause = !pause;

                      case SDLK_s:
                        slowMotion = !slowMotion;
                        swap(crash_balls[0].color, crash_balls[1].color);

                      case SDLK_SPACE:
                        if (!is_crash_ball_moved)
                          swap(crash_balls[0].color, crash_balls[1].color);
                      }
                    }
                    if (event.button.x > 0 && event.button.x < WIDTH && event.button.y > 0 && event.button.y < HIGHT)
                    {
                      x_mouse = event.button.x;
                      y_mouse = event.button.y;
                    }
                    drawButton(renderer, setting, event);
                    showScore(score, 145, 930, scoreTexture);
                    render(renderer);
                  }
                  if (win)
                  {
                    if (players[playerIndex].classicScore < score)
                    {
                      cout << "New High Score" << endl;
                      players[playerIndex].classicScore = score;
                      updateLeaderboard(players);
                    }
                    balls.clear();
                    crashed.clear();
                    fallingBall.clear();
                    button OK = {150, 68, 242, 657, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Game/OKButton.png"), IMG_LoadTexture(renderer, "assets/Game/OKButtonHover.png"), IMG_LoadTexture(renderer, "assets/Game/OKButtonClicked.png"), hover, click};
                    cout << "You win" << endl;
                    SDL_RenderCopy(renderer, GameBG, 0, 0);
                    SDL_Texture *winText = IMG_LoadTexture(renderer, "assets/Game/win.png");
                    SDL_Rect winRect = {112, 250, 401, 513};
                    SDL_RenderCopy(renderer, winText, 0, &winRect);
                    showUserScore(renderer, name, score, 250, 495);
                    showUserScore(renderer, name, players[playerIndex].classicScore, 250, 595);
                    drawButton(renderer, OK, event);
                    render(renderer);
                    while (win)
                    {
                      while (SDL_PollEvent(&event) && win)
                      {
                        if (quitGame(event))
                        {
                          classicGM.isSelected = false;
                          quit(running);
                          break;
                        }
                        if (goBack(event) || OK.wasClicked)
                        {
                          classicGM.isSelected = false;
                          win = false;
                          break;
                        }
                        SDL_RenderCopy(renderer, GameBG, 0, 0);
                        SDL_Rect winRect = {112, 250, 401, 513};
                        SDL_RenderCopy(renderer, winText, 0, &winRect);
                        showUserScore(renderer, name, score, 250, 495);
                        showUserScore(renderer, name, players[playerIndex].classicScore, 250, 595);
                        drawButton(renderer, OK, event);
                        render(renderer);
                      }
                    }
                  }
                  else
                  {
                    for (int i = 0; i < lines + stick; i++)
                      for (int j = 0; j < columns; j++)
                        if (!balls[i][j].isEmpty && balls[i][j].color != 7)
                          score--;
                    if (players[playerIndex].classicScore < score)
                    {
                      cout << "New High Score" << endl;
                      players[playerIndex].classicScore = score;
                      updateLeaderboard(players);
                    }
                    balls.clear();
                    crashed.clear();
                    fallingBall.clear();
                    button OK = {150, 68, 242, 657, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Game/OKButton.png"), IMG_LoadTexture(renderer, "assets/Game/OKButtonHover.png"), IMG_LoadTexture(renderer, "assets/Game/OKButtonClicked.png"), hover, click};
                    cout << "You lose" << endl;
                    SDL_RenderCopy(renderer, GameBG, 0, 0);
                    SDL_Texture *loseText = IMG_LoadTexture(renderer, "assets/Game/lose.png");
                    SDL_Rect loseRect = {112, 250, 401, 513};
                    SDL_RenderCopy(renderer, loseText, 0, &loseRect);
                    showUserScore(renderer, name, score, 250, 495);
                    showUserScore(renderer, name, players[playerIndex].classicScore, 250, 595);
                    drawButton(renderer, OK, event);
                    render(renderer);
                    while (lose)
                    {
                      while (SDL_PollEvent(&event) && lose)
                      {
                        if (quitGame(event))
                        {
                          classicGM.isSelected = false;
                          quit(running);
                          break;
                        }
                        if (goBack(event) || OK.wasClicked)
                        {
                          classicGM.isSelected = false;
                          lose = false;
                          break;
                        }
                        SDL_RenderCopy(renderer, GameBG, 0, 0);
                        SDL_Rect loseRect = {112, 250, 401, 513};
                        SDL_RenderCopy(renderer, loseText, 0, &loseRect);
                        showUserScore(renderer, name, score, 250, 495);
                        showUserScore(renderer, name, players[playerIndex].classicScore, 250, 595);
                        drawButton(renderer, OK, event);
                        render(renderer);
                      }
                    }
                  }
                }
                classicGM.isSelected = true;
              }
              else if (infiniteGM.isSelected)
              {
                balls.clear();
                crashed.clear();
                fallingBall.clear();

                lines = 150, realLines = 135;
                score = 100;
                crashed_score = 0;
                win = false, lose = false;
                pause = false, slowMotion = false;

                initial_ball();
                for (int j = 0; j < columns; j++)
                  balls[0][j].isEmpty = false, balls[0][j].shouldStick = true, balls[0][j].stickCheck = 1;

                for (int i = 1; i < lines; i++)
                  for (int j = 0; j < columns; j++)
                    balls[i][j].isEmpty = false, balls[i][j].shouldStick = false, balls[i][j].stickCheck = 0;

                for (int i = realLines; i < lines; i++)
                {
                  for (int j = 0; j < columns; j++)
                    balls[i][j].isEmpty = true;
                }

                if (rand() % 17 == 0)
                {
                  crash_balls[1].color = 11;
                }
                else
                {
                  switch (rand() % 4)
                  {
                  case 0:
                  {
                    crash_balls[1].color = 1;
                    break;
                  }
                  case 1:
                  {
                    crash_balls[1].color = 2;
                    break;
                  }
                  case 2:
                  {
                    crash_balls[1].color = 4;
                    break;
                  }
                  case 3:
                  {
                    crash_balls[1].color = 8;
                    break;
                  }
                  }
                }
                initial_crash_ball(renderer);
                draw_ball(renderer);

                while (infiniteGM.isSelected)
                {
                  while (!win && !lose)
                  {
                    if (quitGame(event))
                    {
                      infiniteGM.isSelected = false;
                      quit(running);
                      break;
                    }
                    if (goBack(event) || back.wasClicked || home.wasClicked)
                    {
                      infiniteGM.isSelected = false;
                      home.wasClicked = false;
                      back.wasClicked = false;
                      break;
                    }

                    if (setting.wasClicked)
                    {
                      vector<button> adjustButtons2 = {
                          {39, 46, 218, 414, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-VOLUME.png"), IMG_LoadTexture(renderer, "assets/Settings/L-VOLUMEHovered.png"), 0, hover, click},
                          {39, 46, 530, 414, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-VOLUME.png"), IMG_LoadTexture(renderer, "assets/Settings/R-VOLUMEHovered.png"), 0, hover, click},
                          {39, 46, 218, 538, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/L-MusicHovered.png"), 0, hover, click},
                          {39, 46, 530, 538, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/R-MusicHovered.png"), 0, hover, click},
                          {39, 46, 218, 475, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/L-SFXHovered.png"), IMG_LoadTexture(renderer, "assets/Settings/L-SFXHovered.png"), hover, click},
                          {39, 46, 530, 475, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/R-SFXHovered.png"), IMG_LoadTexture(renderer, "assets/Settings/R-SFXHovered.png"), hover, click}};
                      while (setting.wasClicked)
                      {
                        while (SDL_PollEvent(&event) && setting.wasClicked)
                        {
                          if (quitGame(event))
                          {
                            setting.wasClicked = false;
                            quit(running);
                            break;
                          }
                          if (goBack(event) || back.wasClicked || home.wasClicked)
                          {
                            setting.wasClicked = false;
                            back.wasClicked = false;
                            break;
                          }
                          SDL_RenderCopy(renderer, GameBG, 0, 0);

                          SDL_Rect pauseRect = {33, 363, 562, 275};

                          SDL_RenderCopy(renderer, pauseMenu, 0, &pauseRect);

                          drawButton(renderer, adjustButtons2[0], event);
                          drawButton(renderer, adjustButtons2[1], event);
                          drawButton(renderer, adjustButtons2[2], event);
                          drawButton(renderer, adjustButtons2[3], event);
                          drawButton(renderer, adjustButtons2[4], event);
                          drawButton(renderer, adjustButtons2[5], event);

                          if (adjustButtons2[1].wasClicked && volume < 100)
                          {
                            volume += 20;
                            if (sfx)
                              Mix_Volume(-1, volume);

                            Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
                            adjustButtons2[1].wasClicked = false;
                          }
                          else if (adjustButtons2[0].wasClicked && volume > 0)
                          {
                            volume -= 20;
                            if (sfx)
                              Mix_Volume(-1, volume);
                            Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
                            adjustButtons2[0].wasClicked = false;
                          }

                          if (adjustButtons2[4].wasClicked || adjustButtons2[5].wasClicked)
                          {
                            sfx = !sfx;
                            adjustButtons2[4].wasClicked = false, adjustButtons2[5].wasClicked = false;
                            if (sfx)
                              Mix_Volume(-1, volume);
                            else
                              Mix_Volume(-1, 0);
                          }
                          if (sfx)
                          {
                            SDL_Rect sfxRect = {363, 475, 42, 22};
                            SDL_RenderCopy(renderer, on, 0, &sfxRect);
                          }
                          else
                          {
                            SDL_Rect sfxRect = {349, 475, 55, 22};
                            SDL_RenderCopy(renderer, off, 0, &sfxRect);
                          }

                          SDL_Rect volumeBar = {285, 405, 222, 43};
                          SDL_RenderCopy(renderer, soundBar[volume / 20], 0, &volumeBar);

                          setMusic(renderer, adjustButtons2[3].wasClicked, adjustButtons2[2].wasClicked, musicList, musicIndex, musicSound, Settings, 285, 535);

                          if (adjustButtons2[3].wasClicked || adjustButtons2[2].wasClicked)
                          {
                            Mix_PlayMusic(musicSound, -1);
                            adjustButtons2[3].wasClicked = false;
                            adjustButtons2[2].wasClicked = false;
                          }

                          drawButton(renderer, home, event);
                          drawButton(renderer, back, event);
                          render(renderer);
                        }
                      }
                    }

                    SDL_RenderCopy(renderer, GameBG, NULL, NULL);

                    if (slowMotion)
                      dy_initial = 0.1;
                    else
                      dy_initial = 0.25;

                    if (!pause && !is_crash_ball_moved)
                      for (int i = 0; i < lines + stick; i++)
                        for (int j = 0; j < columns; j++)
                          balls[i][j].y += dy_initial;

                    crashed_ball(renderer);
                    draw_ball(renderer);

                    if (event.type == SDL_KEYDOWN)
                    {
                      switch (event.key.keysym.sym)
                      {
                      case SDLK_p:
                        pause = !pause;

                      case SDLK_s:
                        slowMotion = !slowMotion;
                        swap(crash_balls[0].color, crash_balls[1].color);

                      case SDLK_SPACE:
                        if (!is_crash_ball_moved)
                          swap(crash_balls[0].color, crash_balls[1].color);
                      }
                    }
                    if (event.button.x > 0 && event.button.x < WIDTH && event.button.y > 0 && event.button.y < HIGHT)
                    {
                      x_mouse = event.button.x;
                      y_mouse = event.button.y;
                    }

                    showScore(infinityScore(), 145, 930, scoreTexture);

                    drawButton(renderer, setting, event);
                    render(renderer);
                  }

                  score = infinityScore();
                  cout << "infinity Score =" << score << endl;

                  if (players[playerIndex].infiniteScore < score)
                  {
                    cout << "New High Score" << endl;
                    players[playerIndex].infiniteScore = score;
                    updateLeaderboard(players);
                  }
                  balls.clear();
                  crashed.clear();
                  fallingBall.clear();
                  button OK = {150, 68, 242, 657, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Game/OKButton.png"), IMG_LoadTexture(renderer, "assets/Game/OKButtonHover.png"), IMG_LoadTexture(renderer, "assets/Game/OKButtonClicked.png"), hover, click};

                  cout << "Game Over" << endl;

                  win = true;

                  SDL_RenderCopy(renderer, GameBG, 0, 0);
                  SDL_Texture *winText = IMG_LoadTexture(renderer, "assets/Game/GameOver.png");
                  SDL_Rect winRect = {112, 250, 401, 513};
                  SDL_RenderCopy(renderer, winText, 0, &winRect);
                  showUserScore(renderer, name, score, 250, 495);
                  showUserScore(renderer, name, players[playerIndex].infiniteScore, 250, 595);
                  drawButton(renderer, OK, event);
                  render(renderer);

                  while (win)
                  {
                    while (SDL_PollEvent(&event) && win)
                    {
                      if (quitGame(event))
                      {
                        infiniteGM.isSelected = false;
                        win = false;
                        quit(running);
                        break;
                      }
                      if (goBack(event) || OK.wasClicked)
                      {
                        infiniteGM.isSelected = false;
                        win = false;
                        break;
                      }
                      SDL_RenderCopy(renderer, GameBG, 0, 0);
                      SDL_Rect winRect = {112, 250, 401, 513};
                      SDL_RenderCopy(renderer, winText, 0, &winRect);
                      showUserScore(renderer, name, score, 250, 495);
                      showUserScore(renderer, name, players[playerIndex].infiniteScore, 250, 595);
                      drawButton(renderer, OK, event);
                      render(renderer);
                    }
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

        vector<button> adjustButtons = {
            {39, 46, 218, 226, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-VOLUME.png"), IMG_LoadTexture(renderer, "assets/Settings/L-VOLUMEHovered.png"), 0, hover, click},
            {39, 46, 530, 226, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-VOLUME.png"), IMG_LoadTexture(renderer, "assets/Settings/R-VOLUMEHovered.png"), 0, hover, click},
            {39, 46, 218, 350, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/L-MusicHovered.png"), 0, hover, click},
            {39, 46, 530, 350, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-Music.png"), IMG_LoadTexture(renderer, "assets/Settings/R-MusicHovered.png"), 0, hover, click},
            {58, 68, 90, 663, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-THEME.png"), IMG_LoadTexture(renderer, "assets/Settings/L-THEMEHovered.png"), 0, hover, click},
            {58, 68, 480, 663, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-THEME.png"), IMG_LoadTexture(renderer, "assets/Settings/R-THEMEHovered.png"), 0, hover, click},
            {39, 46, 218, 287, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/L-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/L-SFXHovered.png"), IMG_LoadTexture(renderer, "assets/Settings/L-SFXHovered.png"), hover, click},
            {39, 46, 530, 287, 0, 0, 0, 0, 0, false, false, IMG_LoadTexture(renderer, "assets/Settings/R-SFX.png"), IMG_LoadTexture(renderer, "assets/Settings/R-SFXHovered.png"), IMG_LoadTexture(renderer, "assets/Settings/R-SFXHovered.png"), hover, click}};

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
            drawButton(renderer, adjustButtons[6], event);
            drawButton(renderer, adjustButtons[7], event);

            if (adjustButtons[1].wasClicked && volume < 100)
            {
              volume += 20;
              if (sfx)
                Mix_Volume(-1, volume);

              Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
              adjustButtons[1].wasClicked = false;
            }
            else if (adjustButtons[0].wasClicked && volume > 0)
            {
              volume -= 20;
              if (sfx)
                Mix_Volume(-1, volume);
              Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
              adjustButtons[0].wasClicked = false;
            }

            if (adjustButtons[7].wasClicked || adjustButtons[6].wasClicked)
            {
              sfx = !sfx;
              adjustButtons[7].wasClicked = false, adjustButtons[6].wasClicked = false;
              if (sfx)
                Mix_Volume(-1, volume);
              else
                Mix_Volume(-1, 0);
            }
            if (sfx)
            {
              SDL_Rect sfxRect = {363, 296, 42, 22};
              SDL_RenderCopy(renderer, on, 0, &sfxRect);
            }
            else
            {
              SDL_Rect sfxRect = {349, 296, 55, 22};
              SDL_RenderCopy(renderer, off, 0, &sfxRect);
            }

            SDL_Rect volumeBar = {285, 225, 222, 43};
            SDL_RenderCopy(renderer, soundBar[volume / 20], 0, &volumeBar);

            setMusic(renderer, adjustButtons[3].wasClicked, adjustButtons[2].wasClicked, musicList, musicIndex, musicSound, Settings, 285, 350);

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

            SDL_Rect themeRect = {184, 482, 260, 457};
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
