#ifndef leaderboard_hpp
#define leaderboard_hpp

#include <iostream>
#include <vector>
#include <fstream>
#include "SDL_Headers.h"

using namespace std;

struct player
{
  string name;
  int classicScore, infiniteScore, timerScore;
};

void getLeaderboard(vector<player> &players)
{
  ifstream file;
  file.open("assets/leaderboard/leaderboard.txt");
  while (!file.eof())
  {
    player temp;
    file >> temp.name >> temp.classicScore >> temp.infiniteScore >> temp.timerScore;
    players.push_back(temp);
  }
  file.close();
}

void printPlayers(vector<player> &players)
{
  for (int i = 0; i < players.size(); i++)
  {
    cout << players[i].name << " " << players[i].classicScore << " " << players[i].infiniteScore << " " << players[i].timerScore << endl;
  }
}

void sortPlayersBasedOnClassicScore(vector<player> &players)
{
  for (int i = 0; i < players.size(); i++)
  {
    for (int j = i + 1; j < players.size(); j++)
    {
      if (players[i].classicScore < players[j].classicScore)
      {
        player temp = players[i];
        players[i] = players[j];
        players[j] = temp;
      }
    }
  }
}

void sortPlayersBasedOnInfiniteScore(vector<player> &players)
{
  for (int i = 0; i < players.size(); i++)
  {
    for (int j = i + 1; j < players.size(); j++)
    {
      if (players[i].infiniteScore < players[j].infiniteScore)
      {
        player temp = players[i];
        players[i] = players[j];
        players[j] = temp;
      }
    }
  }
}

void sortPlayersBasedOnTimerScore(vector<player> &players)
{
  for (int i = 0; i < players.size(); i++)
  {
    for (int j = i + 1; j < players.size(); j++)
    {
      if (players[i].timerScore < players[j].timerScore)
      {
        player temp = players[i];
        players[i] = players[j];
        players[j] = temp;
      }
    }
  }
}

void drawLeaderboard(vector<player> &players, SDL_Renderer *renderer, SDL_Texture *leaderboardBackground, TTF_Font *font, string sort)
{
  SDL_RenderCopy(renderer, leaderboardBackground, NULL, NULL);

  SDL_Color color = {0, 0, 0};

  if (font != nullptr)
  {
    for (int i = 0; i < 7 && i < players.size(); i++)
    {
      // player name
      SDL_Surface *surface = TTF_RenderText_Solid(font, players[i].name.c_str(), color);
      SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_Rect rect = {128, 390 + (i * 80), surface->w, surface->h};
      SDL_RenderCopy(renderer, texture, NULL, &rect);
      SDL_FreeSurface(surface);
      SDL_DestroyTexture(texture);

      // player score
      if (sort == "Classic")
      {
        surface = TTF_RenderText_Solid(font, to_string(players[i].classicScore).c_str(), color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect = {460, 390 + (i * 80), surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
      }
      else if (sort == "Infinite")
      {
        surface = TTF_RenderText_Solid(font, to_string(players[i].infiniteScore).c_str(), color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect = {460, 390 + (i * 80), surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
      }
      else if (sort == "Timer")
      {
        surface = TTF_RenderText_Solid(font, to_string(players[i].timerScore).c_str(), color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect = {460, 390 + (i * 80), surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
      }
    }
  }
  else
  {
    cout << "Font not loaded" << endl;
  }
}

void updateLeaderboard(vector<player> &players)
{
  ofstream file;
  file.open("assets/leaderboard/leaderboard.txt", ios::trunc);
  for (int i = 0; i < players.size(); i++)
  {
    if (players[i].name != "0" && players[i].name != "")
    {
      file << players[i].name << ' ' << players[i].classicScore << ' ' << players[i].infiniteScore << ' ' << players[i].timerScore << endl;
    }
  }
  file << "0 0 0 0" << endl;
  file.close();
}

#endif // !leaderboard_hpp