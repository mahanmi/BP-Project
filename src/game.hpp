#ifndef game_hpp
#define game_hpp

#include <iostream>
#include "SDL_Headers.h"
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

bool areConnected(ball ball1, ball ball2)
{
    if (pow(ball1.x - ball2.x, 2) + pow(ball1.y - ball2.y, 2) <= pow(2 * ballRadius, 2))
        return true;
    return false;
}

bool shouldStick(int iBall, int jBall)
{
    if (balls[iBall][jBall].isEmpty)
        return false;

    if (iBall == 0)
        return true;

    if (balls[iBall][jBall].check == -1)
        return false;

    if (iBall % 2 == 0)
    {
        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty)
        {
            if (balls[iBall - 1][jBall].check == 1) // North *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall << endl;
                balls[iBall][jBall].check = balls[iBall - 1][jBall].check;
                return true;
            }
        }

        if (iBall > 0 && jBall > 0 && !balls[iBall - 1][jBall - 1].isEmpty)
        {
            if (balls[iBall - 1][jBall - 1].check == 1) // North West *
            {
                // //cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = balls[iBall - 1][jBall - 1].check;
                return true;
            }
        }

        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty)
        {
            if (balls[iBall][jBall + 1].check == 1) // East *
            {
                // //cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = balls[iBall][jBall + 1].check;
                return true;
            }
        }

        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty)
        {
            if (balls[iBall][jBall - 1].check == 1) // West *
            {
                // //cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = balls[iBall][jBall - 1].check;
                return true;
            }
        }

        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty)
            if (balls[iBall + 1][jBall].check == 1) // South *
            {
                // //cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall << endl;
                balls[iBall][jBall].check = balls[iBall + 1][jBall].check;
                return true;
            }

        if (jBall > 0 && iBall < lines - 1 && !balls[iBall + 1][jBall - 1].isEmpty)
            if (balls[iBall + 1][jBall - 1].check == 1) // South West *
            {
                // //cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = balls[iBall + 1][jBall - 1].check;
                return true;
            }

        // 2nd check

        balls[iBall][jBall].check = -1;

        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty && balls[iBall - 1][jBall].check != -1)
        {
            if (shouldStick(iBall - 1, jBall)) // North *
            {
                // //cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        if (iBall > 0 && jBall > 0 && !balls[iBall - 1][jBall - 1].isEmpty && balls[iBall - 1][jBall - 1].check != -1)
        {
            if (shouldStick(iBall - 1, jBall - 1)) // North West *

            {
                // cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty && balls[iBall][jBall - 1].check != -1)
        {
            if (shouldStick(iBall, jBall - 1)) // West *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        if (iBall < lines - 1 && jBall > 0 && !balls[iBall + 1][jBall - 1].isEmpty && balls[iBall + 1][jBall - 1].check != -1)
        {
            if (shouldStick(iBall + 1, jBall - 1)) // South West *

            {
                // cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty && balls[iBall][jBall + 1].check != -1)
        {
            if (shouldStick(iBall, jBall + 1)) // East *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty && balls[iBall + 1][jBall].check != -1)
        {
            if (shouldStick(iBall + 1, jBall)) // South *

            {
                // cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        return false;
    }
    else
    {
        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty)
        {
            if (balls[iBall - 1][jBall].check == 1) // North *

            {
                // cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall << endl;
                balls[iBall][jBall].check = balls[iBall - 1][jBall].check;
                return true;
            }
        }

        if (iBall > 0 && jBall < columns - 1 && !balls[iBall - 1][jBall + 1].isEmpty)
        {
            if (balls[iBall - 1][jBall + 1].check == 1) // North East *

            {
                // cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = balls[iBall - 1][jBall - 1].check;
                return true;
            }
        }

        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty)
        {
            if (balls[iBall][jBall + 1].check == 1) // East *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = balls[iBall][jBall + 1].check;
                return true;
            }
        }

        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty)
        {
            if (balls[iBall][jBall - 1].check == 1) // West *

            {
                // cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = balls[iBall][jBall - 1].check;
                return true;
            }
        }

        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty)
            if (balls[iBall + 1][jBall].check == 1) // South *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall << endl;
                balls[iBall][jBall].check = balls[iBall + 1][jBall].check;
                return true;
            }

        if (jBall < columns - 1 && iBall < lines - 1 && !balls[iBall + 1][jBall + 1].isEmpty)
            if (balls[iBall + 1][jBall + 1].check == 1) // South East *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = balls[iBall + 1][jBall + 1].check;
                return true;
            }

        // 2nd check

        balls[iBall][jBall].check = -1;

        if (iBall > 0 && jBall < columns - 1 && !balls[iBall - 1][jBall + 1].isEmpty && balls[iBall - 1][jBall + 1].check != -1)
        {
            if (shouldStick(iBall - 1, jBall + 1)) // North East *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty && balls[iBall - 1][jBall].check != -1)
        {
            if (shouldStick(iBall - 1, jBall)) // North *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty && balls[iBall][jBall + 1].check != -1)
        {
            if (shouldStick(iBall, jBall + 1)) // East *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        if (jBall < columns - 1 && iBall < lines - 1 && !balls[iBall + 1][jBall + 1].isEmpty && balls[iBall + 1][jBall + 1].check != -1)
        {
            if (shouldStick(iBall + 1, jBall + 1)) // South East *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty && balls[iBall][jBall - 1].check != -1)
        {
            if (shouldStick(iBall, jBall - 1)) // West *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty && balls[iBall + 1][jBall].check != -1)
        {
            if (shouldStick(iBall + 1, jBall)) // South *
            {
                // cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }

        return false;
    }
}

void window_color(SDL_Renderer *Renderer, int R, int G, int B)
{
    SDL_SetRenderDrawColor(Renderer, R, G, B, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderPresent(Renderer);
}

void rect(SDL_Renderer *Renderer, int x, int y, int WIDTH, int HIGHT, int R, int G, int B, int fill_bool)
{
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = WIDTH;
    rectangle.h = HIGHT;
    SDL_SetRenderDrawColor(Renderer, R, G, B, 255);
    if (fill_bool == 1)
        SDL_RenderFillRect(Renderer, &rectangle);
    SDL_RenderDrawRect(Renderer, &rectangle);
}

void initial_ball()
{
    balls.resize(lines + stick);

    int x_center = ballRadius;
    int y_center = 0;

    ball new_ball;
    for (int i = 0; i < lines + stick; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            new_ball.x = x_center;
            new_ball.y = y_center;
            new_ball.i = i;
            new_ball.j = j;

            int rand_col = rand() % 33;

            if (rand_col == 20)
            {
                switch (rand() % 6)
                {
                case 0:
                {
                    new_ball.color = 3; // yellow & blue
                    break;
                }
                case 1:
                {
                    new_ball.color = 5; // yellow & green
                    break;
                }
                case 2:
                {
                    new_ball.color = 9; // yellow & red
                    break;
                }
                case 3:
                {
                    new_ball.color = 6; // blue & green
                    break;
                }
                case 4:
                {
                    new_ball.color = 10; // blue & red
                    break;
                }
                case 5:
                {
                    new_ball.color = 12; // green & red
                    break;
                }
                }
            }
            else if (rand_col % 4 == 0 && rand_col < 22)
            {
                new_ball.color = 1; // yellow
            }
            if (rand_col == 21)
            {
                new_ball.color = 7; // black
            }
            else if (rand_col % 4 == 1 && rand_col < 22)
            {
                new_ball.color = 2; // blue
            }
            if (rand_col % 4 == 2 && rand_col < 22)
            {
                new_ball.color = 4; // green
            }
            if (rand_col % 4 == 3 && rand_col < 22)
            {
                new_ball.color = 8; // red
            }
            else
            {
                if(j == 0)
                    new_ball.color = balls[i-1][0].color;
                else
                    new_ball.color = balls[i][j-1].color;
            }

            balls[i].push_back(new_ball);
            x_center += 2 * ballRadius;
            balls[i][j].y += dy_initial;
        }
        if (i % 2 == 0)
            x_center = 2 * ballRadius;
        else
            x_center = ballRadius;
        y_center += sqrt(3) * ballRadius;
    }
    /*  if (stick > 0)
     {
         if (lines % 2 == 0)
             x_center = 2 * ballRadius;
         else
             x_center = ballRadius;
         y_center += sqrt(3) * ballRadius;
         for (int i = lines; i < lines + stick; i++)
         {
             for (int j = 0; j < columns; j++)
             {
                 new_ball.x = x_center;
                 new_ball.y = y_center;
                 new_ball.i = i;
                 new_ball.j = j;
                 new_ball.isEmpty = true;
                 balls[i].push_back(new_ball);
                 x_center += 2 * ballRadius;
                 balls[i][j].y += dy_initial;
             }
             if (i % 2 == 0)
                 x_center = 2 * ballRadius;
             else
                 x_center = ballRadius;
             y_center += sqrt(3) * ballRadius;
         }
     } */
}

void draw_ball(SDL_Renderer *Renderer)
{
    for (int i = 0; i < 4; i++)
        crash_ball_color[i] = false;

    for (int i = 0; i < lines + stick; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (!balls[i][j].isEmpty)
            {
                if (balls[i][j].color == 1)
                    crash_ball_color[0] = true;
                else if (balls[i][j].color == 2)
                    crash_ball_color[1] = true;
                else if (balls[i][j].color == 3)
                {
                    crash_ball_color[0] = true;
                    crash_ball_color[1] = true;
                }
                else if (balls[i][j].color == 4)
                    crash_ball_color[2] = true;
                    else if (balls[i][j].color == 5)
                {
                    crash_ball_color[0] = true;
                    crash_ball_color[2] = true;
                }
                else if (balls[i][j].color == 6)
                {
                    crash_ball_color[1] = true;
                    crash_ball_color[2] = true;
                }
                else if (balls[i][j].color == 8)
                    crash_ball_color[3] = true;
                    else if (balls[i][j].color == 9)
                {
                    crash_ball_color[0] = true;
                    crash_ball_color[3] = true;
                }
                else if (balls[i][j].color == 10)
                {
                    crash_ball_color[1] = true;
                    crash_ball_color[3] = true;
                }
                else if (balls[i][j].color == 12)
                {
                    crash_ball_color[2] = true;
                    crash_ball_color[3] = true;
                }

                int x_center = balls[i][j].x, y_center = balls[i][j].y;
                SDL_Rect Ball = {x_center - ballRadius, y_center - ballRadius, 2 * ballRadius, 2 * ballRadius};
                switch (balls[i][j].color)
                {
                case 1:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/1.png"), NULL, &Ball);
                    break;

                case 2:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/2.png"), NULL, &Ball);
                    break;

                case 3:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/3.png"), NULL, &Ball);
                    break;

                case 4:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/4.png"), NULL, &Ball);
                    break;

                case 5:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/5.png"), NULL, &Ball);
                    break;

                case 6:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/6.png"), NULL, &Ball);
                    break;

                case 7:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/7.png"), NULL, &Ball);
                    break;

                case 8:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/8.png"), NULL, &Ball);
                    break;

                case 9:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/9.png"), NULL, &Ball);
                    break;

                case 10:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/10.png"), NULL, &Ball);
                    break;

                case 11:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/11.png"), NULL, &Ball);
                    break;

                case 12:
                    SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/12.png"), NULL, &Ball);
                    break;
                }
            }
        }
    }
}

void initial_crash_ball(SDL_Renderer *Renderer)
{
    crash_balls[0].x = int(WIDTH / 2);
    crash_balls[0].y = int(HIGHT - 100);
    crash_balls[0].isEmpty = false;
    crash_balls[0].color = crash_balls[1].color;

    if (rand() % 10 == 0)
    {
        crash_balls[1].color = 11;
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/11.png"), NULL, &Ball2);
    }
    else
    {
        switch (rand() % 4)
        {
        case 0:
        {
            if (crash_ball_color[0])
            {
                crash_balls[1].color = 1;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/1.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[1])
            {
                crash_balls[1].color = 2;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/2.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[2])
            {
                crash_balls[1].color = 4;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/4.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[3])
            {
                crash_balls[1].color = 8;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/8.png"), NULL, &Ball2);
                break;
            }
            else
                win = true;
        }
        case 1:
        {
            if (crash_ball_color[1])
            {
                crash_balls[1].color = 2;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/2.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[0])
            {
                crash_balls[1].color = 1;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/1.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[2])
            {
                crash_balls[1].color = 4;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/4.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[3])
            {
                crash_balls[1].color = 8;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/8.png"), NULL, &Ball2);
                break;
            }
            else
                win = true;
        }
        case 2:
        {
            if (crash_ball_color[2])
            {
                crash_balls[1].color = 4;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/4.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[0])
            {
                crash_balls[1].color = 1;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/1.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[1])
            {
                crash_balls[1].color = 2;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/2.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[3])
            {
                crash_balls[1].color = 8;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/8.png"), NULL, &Ball2);
                break;
            }
            else
                win = true;
        }
        case 3:
        {
            if (crash_ball_color[3])
            {
                crash_balls[1].color = 8;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/8.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[0])
            {
                crash_balls[1].color = 1;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/1.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[1])
            {
                crash_balls[1].color = 2;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/2.png"), NULL, &Ball2);
                break;
            }
            else if (crash_ball_color[2])
            {
                crash_balls[1].color = 4;
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/4.png"), NULL, &Ball2);
                break;
            }
            else
                win = true;
        }
        }
    }

    switch (crash_balls[0].color)
    {
    case 1:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/1.png"), NULL, &Ball);
        break;
    }
    case 2:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/2.png"), NULL, &Ball);
        break;
    }
    case 4:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/4.png"), NULL, &Ball);
        break;
    }
    case 8:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/8.png"), NULL, &Ball);
        break;
    }
    case 11:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/11.png"), NULL, &Ball);
        break;
    }
    }

    if (is_crash_ball_crashed)
    {
        bool repeat = true;
        while (repeat)
        {
            repeat = false;
            for (int i = 1; i < lines + stick; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    balls[i][j].check = 0;
                }
            }
            for (int i = 1; i < lines + stick; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    if (!balls[i][j].isEmpty)
                    {
                        if (!shouldStick(i, j))
                        {
                            cout << i << " " << j << " ShouldN't stick" << endl;
                            balls[i][j].isEmpty = true;
                            repeat = true;
                        }
                    }
                }
            }
        }
    }
    is_crash_ball_crashed = false;
    is_crash_ball_moved = false;
}

void isConnected(int i, int j)
{
    /* if (i < 0 || i > lines + stick || j < 0 || j > columns)
        return; */

    crashed.push_back(balls[i][j]);
    balls[i][j].isEmpty = true;

    for (int m = 0; m < lines + stick; m++)
        for (int n = 0; n < columns; n++)
            if (!balls[m][n].isEmpty && balls[m][n].color != 7)
                if (pow(balls[m][n].x - balls[i][j].x, 2) + pow(balls[m][n].y - balls[i][j].y, 2) <= pow(2 * ballRadius, 2))
                    if (balls[i][j].color == balls[m][n].color || (balls[i][j].color == 1 && balls[m][n].color % 2 != 0) || (balls[i][j].color == 2 && (balls[m][n].color == 3 || balls[m][n].color == 6 || balls[m][n].color == 10)) || (balls[i][j].color == 4 && (balls[m][n].color == 5 || balls[m][n].color == 6 || balls[m][n].color == 12)) || (balls[i][j].color == 8 && (balls[m][n].color == 9 || balls[m][n].color == 10 || balls[m][n].color == 12)) || (balls[i][j].color == 3 && (balls[m][n].color == 1 || balls[m][n].color == 2)) || (balls[i][j].color == 5 && (balls[m][n].color == 1 || balls[m][n].color == 4)) || (balls[i][j].color == 9 && (balls[m][n].color == 1 || balls[m][n].color == 8)) || (balls[i][j].color == 6 && (balls[m][n].color == 2 || balls[m][n].color == 4)) || (balls[i][j].color == 10 && (balls[m][n].color == 2 || balls[m][n].color == 8)) || (balls[i][j].color == 12 && (balls[m][n].color == 4 || balls[m][n].color == 8)))
                        isConnected(m, n);

    return;
}

void crashed_ball(SDL_Renderer *Renderer)
{
    SDL_Rect Ball = {int(crash_balls[0].x - ballRadius), int(crash_balls[0].y - ballRadius), 2 * ballRadius, 2 * ballRadius};
    switch (crash_balls[0].color)
    {
    case 1:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/1.png"), NULL, &Ball);
        break;
    }
    case 2:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/2.png"), NULL, &Ball);
        break;
    }
    case 4:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/4.png"), NULL, &Ball);
        break;
    }
    case 8:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/8.png"), NULL, &Ball);
        break;
    }
    case 11:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/11.png"), NULL, &Ball);
        break;
    }
    }
    SDL_Rect Ball2 = {int(crash_balls[1].x - ballRadius), int(crash_balls[1].y - ballRadius), 40, 40};
    switch (crash_balls[1].color)
    {
    case 1:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/1.png"), NULL, &Ball2);
        break;
    }
    case 2:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/2.png"), NULL, &Ball2);
        break;
    }
    case 4:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/4.png"), NULL, &Ball2);
        break;
    }
    case 8:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/8.png"), NULL, &Ball2);
        break;
    }
    case 11:
    {
        SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/11.png"), NULL, &Ball2);
        break;
    }
    }

    if (!is_crash_ball_crashed)
    {
        if (!is_crash_ball_moved)
        {
            aalineRGBA(Renderer, crash_balls[0].x, crash_balls[0].y, x_mouse, y_mouse, 255, 0, 0, 255);
        }
        else
        {
            if (crash_balls[0].x + ballRadius > WIDTH || crash_balls[0].x - ballRadius < 0)
                dx *= -1;
            if (crash_balls[0].y + ballRadius > HIGHT || crash_balls[0].y - ballRadius < balls[0][0].y - ballRadius)
                dy *= -1;

            crash_balls[0].x += dx;
            crash_balls[0].y += dy;

            switch (crash_balls[0].color)
            {
            case 1:
            {
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/1.png"), NULL, &Ball);
                break;
            }
            case 2:
            {
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/2.png"), NULL, &Ball);
                break;
            }
            case 4:
            {
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/4.png"), NULL, &Ball);
                break;
            }
            case 8:
            {
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/8.png"), NULL, &Ball);
                break;
            }
            case 11:
            {
                SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/11.png"), NULL, &Ball);
                break;
            }
            }
            bool shouldRemain = true;
            for (int i = 0; i < lines + stick && !is_crash_ball_crashed; i++)
            {
                for (int j = 0; j < columns && !is_crash_ball_crashed; j++)
                {
                    if (!balls[i][j].isEmpty)
                    {
                        if (pow(crash_balls[0].x - balls[i][j].x, 2) + pow(crash_balls[0].y - balls[i][j].y, 2) < 0.75 * pow(2 * ballRadius, 2))
                        {
                            is_crash_ball_crashed = true;

                            if ((crash_balls[0].color == 11 && balls[i][j].color != 7) || (crash_balls[0].color == 1 && (balls[i][j].color % 2 != 0 && balls[i][j].color != 7)) || (crash_balls[0].color == 2 && (balls[i][j].color == 2 || balls[i][j].color == 3 || balls[i][j].color == 6 || balls[i][j].color == 10)) || (crash_balls[0].color == 4 && (balls[i][j].color == 4 || balls[i][j].color == 5 || balls[i][j].color == 6 || balls[i][j].color == 12)) || (crash_balls[0].color == 8 && (balls[i][j].color == 8 || balls[i][j].color == 9 || balls[i][j].color == 10 || balls[i][j].color == 12)))
                            {
                                isConnected(i, j);
                                if (crashed.size() > 1) // set this to number of the balls that should be sticked to crash - 1
                                    shouldRemain = false;
                                for (int k = crashed.size(); k > 0; k--)
                                {
                                    if (shouldRemain)
                                        balls[crashed[k - 1].i][crashed[k - 1].j].isEmpty = false;
                                    crashed.pop_back();
                                }
                            }
                            else if (shouldRemain)
                            {
                                int jStick = ceil((crash_balls[0].x - balls[0][0].x) / (2 * ballRadius)), iStick = ceil((crash_balls[0].y - balls[0][0].y) / (sqrt(3) * ballRadius));

                                if (iStick - (lines + stick) > 0)
                                    iStick = lines + stick;

                                cout << "New Ball" << iStick << " " << jStick << endl;

                                if (iStick % 2 == 1)
                                    jStick--;
                                if (jStick < 0)
                                    jStick = 0;
                                if (jStick > columns - 1)
                                    jStick = columns - 1;

                                /* if (iStick > lines - 1)
                                {
                                    stick++;
                                    int x_center, y_center;
                                    if (stick + lines % 2)
                                        x_center = 2 * ballRadius;
                                    else
                                        x_center = ballRadius;
                                    y_center = balls[lines - 1][0].y + sqrt(3) * ballRadius;
                                    for (int i = lines + stick - 1; i > lines; i--)
                                    {
                                        for (int j = 0; j < columns; j++)
                                        {
                                            balls[i][j].x = x_center;
                                            balls[i][j].y = y_center;
                                            balls[i][j].i = i;
                                            balls[i][j].j = j;
                                            balls[i][j].isEmpty = true;
                                            x_center += 2 * ballRadius;
                                            balls[i][j].y += dy_initial;
                                        }
                                        if (i % 2 == 0)
                                            x_center = 2 * ballRadius;
                                        else
                                            x_center = ballRadius;
                                        y_center += sqrt(3) * ballRadius;
                                    }
                                } */
                                if (balls[iStick][jStick].isEmpty)
                                {
                                    balls[iStick][jStick].color = crash_balls[0].color;
                                    balls[iStick][jStick].isEmpty = false;
                                    isConnected(iStick, jStick);
                                    if (crashed.size() > 1) // set this to number of the balls that should be sticked to crash - 1
                                        shouldRemain = false;
                                    for (int k = crashed.size(); k > 0; k--)
                                    {
                                        if (shouldRemain)
                                            balls[crashed[k - 1].i][crashed[k - 1].j].isEmpty = false;
                                        crashed.pop_back();
                                    }
                                }
                            }
                            crash_balls[0].isEmpty = true;
                            initial_crash_ball(Renderer);
                        }
                    }
                }
            }
        }

        if (SDL_PollEvent(&event))
        {
            if (event.button.x > 0 && event.button.x < WIDTH && event.button.y > 0 && event.button.y < HIGHT)
            {
                x_mouse = event.button.x;
                y_mouse = event.button.y;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (!is_crash_ball_crashed)
                {
                    is_crash_ball_moved = true;
                    dx = d * ((x_mouse - crash_balls[0].x) / sqrt(pow(x_mouse - crash_balls[0].x, 2) + pow(y_mouse - crash_balls[0].y, 2)));
                    dy = d * ((y_mouse - crash_balls[0].y) / sqrt(pow(x_mouse - crash_balls[0].x, 2) + pow(y_mouse - crash_balls[0].y, 2)));
                }
            }
        }
    }
}

#endif // !game_hpp
