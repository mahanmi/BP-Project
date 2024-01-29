#include <iostream>
#include "SDL_Headers.h"
#include <windows.h>
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
    bool isEmpty;
};

vector<vector<ball>> balls;
struct ball crash_ball;
bool is_crash_ball_moved = false;
bool is_crash_ball_crashed = false;

vector<ball> crash_balls;
int length_crash_balls = 0;

int x_mouse;
int y_mouse;

int W = 625;
int H = 1000;

int dx = 20;
int dy = 20;
float dy_initial = 0.1;
int rad_ball = 25;
int number_of_lines = 4;
int n_colum = W / (2 * rad_ball);
int n_stick = 0;

void window_color(SDL_Renderer *Renderer, int R, int G, int B);
void rect(SDL_Renderer *Renderer, int x, int y, int w, int h, int R, int G, int B, int fill_bool);
void initial_ball();
void draw_ball(SDL_Renderer *Renderer);
void initial_crash_ball();
void crashed_ball(SDL_Renderer *Renderer);
void isConnected(int i, int j);

int main(int argv, char **args)
{
    srand(time(0));

    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
    Uint32 WND_flags = SDL_WINDOW_SHOWN;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Init(SDL_flags);
    SDL_CreateWindowAndRenderer(W, H, WND_flags, &m_window, &m_renderer);
    SDL_RaiseWindow(m_window);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    window_color(m_renderer, 0, 0, 0);

    initial_ball();
    for(int i=0; i<number_of_lines; i++)
        for(int j=0; j<n_colum; j++)
            balls[i][j].isEmpty = false;
    initial_crash_ball();
    draw_ball(m_renderer);
    SDL_RenderPresent(m_renderer);

    SDL_Event *event = new SDL_Event();

    while (1)
    {
        rect(m_renderer, 0, 0, W, H, 0, 0, 0, 1);
        crashed_ball(m_renderer);
        initial_ball();
        draw_ball(m_renderer);
        if (SDL_PollEvent(event))
        {
            if (event->type == SDL_KEYDOWN)
            {
                switch (event->key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        SDL_DestroyWindow(m_window);
                        SDL_DestroyRenderer(m_renderer);
                        IMG_Quit();
                        SDL_Quit();
                        return 0;
                    }
                }
            }
            if (event->type != SDL_KEYDOWN)
            {
                x_mouse = event->button.x;
                y_mouse = event->button.y;
                if (event->type == SDL_MOUSEBUTTONDOWN)
                {
                }
            }
        }
        SDL_RenderPresent(m_renderer);
        SDL_Delay(16);
    }
}

void window_color(SDL_Renderer *Renderer, int R, int G, int B)
{
    SDL_SetRenderDrawColor(Renderer, R, G, B, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderPresent(Renderer);
}

void rect(SDL_Renderer *Renderer, int x, int y, int w, int h, int R, int G, int B, int fill_bool)
{
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    SDL_SetRenderDrawColor(Renderer, R, G, B, 255);
    if (fill_bool == 1)
        SDL_RenderFillRect(Renderer, &rectangle);
    SDL_RenderDrawRect(Renderer, &rectangle);
}

void initial_ball()
{
    balls.resize(number_of_lines);

    int x_center = rad_ball;
    int y_center = rad_ball;

    struct ball new_ball;
    for (int i = 0; i < number_of_lines; i++)
    {
        for (int j = 0; j < n_colum; j++)
        {
            new_ball.x = x_center;
            new_ball.y = y_center;
            int rand_col = rand() % 22;
            if (rand_col == 20)
            {
                switch (rand() % 6)
                {
                    case 0:
                    {
                        new_ball.color = 3; //yellow & blue
                        break;
                    }
                    case 1:
                    {
                        new_ball.color = 5; //yellow & green
                        break;
                    }
                    case 2:
                    {
                        new_ball.color = 9; //yellow & red
                        break;
                    }
                    case 3:
                    {
                        new_ball.color = 6; //blue & green
                        break;
                    }
                    case 4:
                    {
                        new_ball.color = 10; //blue & red
                        break;
                    }
                    case 5:
                    {
                        new_ball.color = 12; //green & red
                        break;
                    }
                }

            }
            else if (rand_col % 4 == 0)
            {
                new_ball.color = 1; //yellow
            }
            if (rand_col == 21)
            {
                new_ball.color = 7; //black
            }
            else if (rand_col % 4 == 1)
            {
                new_ball.color = 2; //blue
            }
            if (rand_col % 4 == 2)
            {
                new_ball.color = 4; //green
            }
            if (rand_col % 4 == 3)
            {
                new_ball.color = 8; //red
            }

            balls[i].push_back(new_ball);
            balls[i][j].x += 2*rad_ball;
        }
        x_center = rad_ball;
        y_center += 2 * rad_ball;
    }
    for (int i = 0; i < number_of_lines; i++)
        for (int j = 0; j < n_colum ; j++)
            balls[i][j].y += dy_initial;
}

void initial_crash_ball()
{
    crash_ball.x = W / 2;
    crash_ball.y = H - 100;

    if (rand() % 10 == 0)
        crash_ball.color = 11;
    else
    {
        switch (rand() % 4)
        {
            case 0:
            {
                crash_ball.color = 1;
                break;
            }
            case 1:
            {
                crash_ball.color = 2;
                break;
            }
            case 2:
            {
                crash_ball.color = 4;
                break;
            }
            case 3:
            {
                crash_ball.color = 8;
                break;
            }
        }
    }
    is_crash_ball_crashed = false;
    is_crash_ball_moved = false;
}

void draw_ball(SDL_Renderer *Renderer)
{
    for (int i = 0; i < number_of_lines; i++)
    {
        for (int j = 0; j < n_colum + n_stick; j++)
        {
            if(!balls[i][j].isEmpty)
            {
                filledCircleColor(Renderer, balls[i][j].x, balls[i][j].y, rad_ball, balls[i][j].color);
            }
        }
    }
}

void crashed_ball(SDL_Renderer *Renderer)
{
    SDL_Event *event = new SDL_Event();

    filledCircleColor(Renderer, crash_ball.x, crash_ball.y, rad_ball, crash_ball.color);
    if (!is_crash_ball_crashed)
    {
        if (!is_crash_ball_moved)
        {
            aalineRGBA(Renderer, crash_ball.x, crash_ball.y, x_mouse, y_mouse, 255, 0, 0, 255);
        }
        else
        {
            if (crash_ball.x > W || crash_ball.x < 0)
                dx *= -1;
            if (crash_ball.y > H || crash_ball.y < 0)
                dy *= -1;

            crash_ball.x += dx;
            crash_ball.y += dy;

            for (int i = 0; i < number_of_lines; i++)
            {
                for (int j = 0; j < n_colum; j++)
                {
                    if(!balls[i][j].isEmpty)
                    {
                        if (pow(balls[i][j].x - crash_ball.x,2) + pow(balls[i][j].y - crash_ball.y,2) <= 2 * rad_ball)
                        {
                            is_crash_ball_crashed = true;
                            if(j>=n_colum/2)
                                crash_ball.x += int(crash_ball.x) % rad_ball;
                            else
                                crash_ball.x -= int(crash_ball.x) % rad_ball;

                            crash_ball.y += int(crash_ball.y) % rad_ball;

                            if ((crash_ball.color == 11 && balls[i][j].color != 7) || (crash_ball.color == 1 && balls[i][j].color % 2 != 0) || (crash_ball.color == 2 && (balls[i][j].color == 2 || balls[i][j].color == 3 || balls[i][j].color == 6 || balls[i][j].color == 10)) || (crash_ball.color == 4 && (balls[i][j].color == 4 || balls[i][j].color == 5 || balls[i][j].color == 6 || balls[i][j].color == 12)) || (crash_ball.color == 8 && (balls[i][j].color == 8 || balls[i][j].color == 9 || balls[i][j].color == 10 || balls[i][j].color == 12)))
                            {
                                balls[i][j].isEmpty = true;
                                isConnected(i, j);
                                length_crash_balls = crash_balls.size();
                                for (int k = 0; k < length_crash_balls; k++)
                                {
                                    balls[i][j].isEmpty = true;
                                }
                            }
                            else
                            {
                                n_stick++;
                                crash_ball.isEmpty = false;
                                initial_crash_ball();
                            }
                            break;
                        }
                    }
                }
            }
        }

        if (SDL_PollEvent(event))
        {
            if (event->type == SDL_KEYDOWN)
            {
                switch (event->key.keysym.sym)
                {
                    case SDLK_SPACE:
                    {
                        if (!is_crash_ball_crashed)
                        {
                            is_crash_ball_moved = true;
                            dx = 10 * ((x_mouse - crash_ball.x) / sqrt(pow(x_mouse - crash_ball.x, 2) + pow(y_mouse - crash_ball.y, 2)));
                            dy = 10 * ((y_mouse - crash_ball.y) / sqrt(pow(x_mouse - crash_ball.x, 2) + pow(y_mouse - crash_ball.y, 2)));
                            break;
                        }
                    }
                }
            }
            if (event->type != SDL_KEYDOWN)
            {
                x_mouse = event->button.x;
                y_mouse = event->button.y;
            }
        }
    }
}

void isConnected(int i, int j)
{
    if (i < 0 || i >= number_of_lines + n_stick|| j < 0 || j >= n_colum)
    {
        return;
    }
    crash_balls.push_back(balls[i][j]);

    for(int m=0; m<number_of_lines; m++)
        for(int n=0; n<n_colum; n++)
            if(!balls[m][n].isEmpty && balls[m][n].color != 7)
                if(pow(balls[m][n].x - balls[i][j].x,2) + pow(balls[m][n].y - balls[i][j].y,2) <= 2*rad_ball)
                    if(balls[i][j].color == balls[m][n].color || (balls[i][j].color == 1 && balls[m][n].color % 2 != 0) || (balls[i][j].color == 2 && (balls[m][n].color == 3 || balls[m][n].color == 6 || balls[m][n].color == 10)) || (balls[i][j].color == 4 && (balls[m][n].color == 5 || balls[m][n].color == 6 || balls[m][n].color == 12)) || (balls[i][j].color == 8 && (balls[m][n].color == 9 || balls[m][n].color == 10 || balls[m][n].color == 12)) || (balls[i][j].color == 3 && (balls[m][n].color == 1 || balls[m][n].color == 2)) || (balls[i][j].color == 5 && (balls[m][n].color == 1 || balls[m][n].color == 4)) || (balls[i][j].color == 9 && (balls[m][n].color == 1 || balls[m][n].color == 8)) || (balls[i][j].color == 6 && (balls[m][n].color == 2 || balls[m][n].color == 4)) || (balls[i][j].color == 10 && (balls[m][n].color == 2 || balls[m][n].color == 8)) || (balls[i][j].color == 12 && (balls[m][n].color == 4 || balls[m][n].color == 8)))
                        isConnected(m, n);
}
