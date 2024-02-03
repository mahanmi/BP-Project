#include <iostream>
#include "SDL_Headers.h"
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

struct ball
{
    int i, j, r = 25;
    int color;
    float x, y;
    bool isEmpty, shouldStick;
    int stickCheck;
};

const int WIDTH = 625, HIGHT = 1000;
int d = 20;
float dx, dy;
float dy_initial = 0.25, dy_fallingBall = 10;
int ballRadius = 25;
int lines = 11;
int columns = WIDTH / (2 * ballRadius);
int stick = 0;

int score = 100;
int crashed_score = 0;
int end_time = 120 * 1000;

SDL_Window *window = SDL_CreateWindow("Bouncing Balls Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
SDL_Event event;

bool win = false, lose = false;

vector<vector<ball>> balls;

bool is_crash_ball_moved = false;
bool is_crash_ball_crashed = false;

vector<ball> crashed;

ball crash_balls[2];
bool crash_ball_color[4] = {1, 1, 1, 1};

int x_mouse, y_mouse;

SDL_Texture *GameBG = IMG_LoadTexture(renderer, "assets/Game/GameBG2.png");

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

#include "game.hpp"

int main(int argv, char **args)
{
    srand(time(0));

    SDL_RenderCopy(renderer, GameBG, NULL, NULL);

    bool pause = false;

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

    if (rand() % 10 == 0)
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
    SDL_RenderPresent(renderer);

    Uint32 start_time = SDL_GetTicks();
    Uint32 current_time = SDL_GetTicks();
    Uint32 elapsed_time = current_time - start_time;

    while (!win && !lose)
    {
        current_time = SDL_GetTicks();
        elapsed_time = current_time - start_time;

        if (elapsed_time >= end_time)
        {
            lose = true;
            break;
        }

        SDL_RenderCopy(renderer, GameBG, NULL, NULL);

        if (!pause && !is_crash_ball_moved)
            for (int i = 0; i < lines + stick; i++)
                for (int j = 0; j < columns; j++)
                    balls[i][j].y += dy_initial;
        draw_ball(renderer);
        crashed_ball(renderer);

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            {
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                IMG_Quit();
                SDL_Quit();
                return 0;
            }
            case SDLK_p:
                pause = !pause;

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
        if (win)
        {
            cout << "You win" << endl;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
        SDL_RenderClear(renderer);
    }
    SDL_RenderCopy(renderer, GameBG, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    cout << timeScore(elapsed_time);
    return 0;
}