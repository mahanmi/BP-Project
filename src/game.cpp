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
    int color;
    float x, y;
    bool isEmpty, shouldStick;
};

const int WIDTH = 625, HIGHT = 1000;
SDL_Window *window = SDL_CreateWindow("Bouncing Balls Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
SDL_Event event;

vector<vector<ball>> balls;
ball crash_ball;

bool is_crash_ball_moved = false;
bool is_crash_ball_crashed = false;

vector<ball> crash_balls;

int x_mouse;
int y_mouse;

int d = 20;
int dx, dy;
float dy_initial = 0.25;
int ballRadius = 25;
int lines = 6;
int columns = WIDTH / (2 * ballRadius);
int stick = 0;

#include "game.hpp"

int main(int argv, char **args)
{
    srand(time(0));

    window_color(renderer, 0, 0, 0);

    initial_ball();
    for (int i = 0; i < lines; i++)
        for (int j = 0; j < columns; j++)
            balls[i][j].isEmpty = false;
    initial_crash_ball(renderer);
    draw_ball(renderer);
    SDL_RenderPresent(renderer);

    while (1)
    {
        rect(renderer, 0, 0, WIDTH, HIGHT, 0, 0, 0, 1);
        crashed_ball(renderer);
        initial_ball();
        draw_ball(renderer);
        if (SDL_PollEvent(&event))
        {
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
                }
            }
            if (event.type != SDL_KEYDOWN)
            {
                x_mouse = event.button.x;
                y_mouse = event.button.y;
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
        SDL_RenderClear(renderer);
    }
}