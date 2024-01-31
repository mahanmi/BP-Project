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
    if (iBall == lines - 1)
        return true;

    if (balls[iBall][jBall].check == -1)
        return false;

    if (iBall % 2 == 0)
    {
        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty)
            if (balls[iBall + 1][jBall].check == 1) // North
            {
                cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall << endl;
                balls[iBall][jBall].check = balls[iBall + 1][jBall].check;
                return true;
            }

        if (jBall > 0 && iBall < lines - 1 && !balls[iBall + 1][jBall - 1].isEmpty)
            if (balls[iBall + 1][jBall - 1].check == 1) // North West
            {
                cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = balls[iBall + 1][jBall - 1].check;
                return true;
            }

        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty)
        {
            if (balls[iBall][jBall - 1].check == 1) // West
            {
                cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = balls[iBall][jBall - 1].check;
                return true;
            }
        }

        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty)
        {
            if (balls[iBall][jBall + 1].check == 1) // East
            {
                cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = balls[iBall][jBall + 1].check;
                return true;
            }
        }

        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty)
        {
            if (balls[iBall - 1][jBall].check == 1) // South
            {
                cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall << endl;
                balls[iBall][jBall].check = balls[iBall - 1][jBall].check;
                return true;
            }
        }

        if (iBall > 0 && jBall > 0 && !balls[iBall - 1][jBall - 1].isEmpty)
        {
            if (balls[iBall - 1][jBall - 1].check == 1) // South West
            {
                cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = balls[iBall - 1][jBall - 1].check;
                return true;
            }
        }

        // 2nd check

        balls[iBall][jBall].check = -1;

        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty)
        {
            if (shouldStick(iBall + 1, jBall)) // North
            {
                cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (iBall < lines - 1 && jBall > 0 && !balls[iBall + 1][jBall - 1].isEmpty)
        {
            if (shouldStick(iBall + 1, jBall - 1)) // North West
            {
                cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty)
        {
            if (shouldStick(iBall, jBall - 1)) // West
            {
                cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty)
        {
            if (shouldStick(iBall - 1, jBall)) // South
            {
                cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty)
        {
            if (shouldStick(iBall, jBall + 1)) // East
            {
                cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (iBall > 0 && jBall > 0 && !balls[iBall - 1][jBall - 1].isEmpty)
        {
            if (shouldStick(iBall - 1, jBall - 1)) // South West
            {
                cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        return false;
    }
    else
    {
        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty)
            if (balls[iBall + 1][jBall].check == 1) // North
            {
                cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall << endl;
                balls[iBall][jBall].check = balls[iBall + 1][jBall].check;
                return true;
            }
        if (jBall < columns - 1 && iBall < lines - 1 && !balls[iBall + 1][jBall + 1].isEmpty)
            if (balls[iBall + 1][jBall + 1].check == 1) // North East
            {
                cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = balls[iBall + 1][jBall + 1].check;
                return true;
            }
        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty)
        {
            if (balls[iBall][jBall + 1].check == 1) // East
            {
                cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = balls[iBall][jBall + 1].check;
                return true;
            }
        }
        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty)
        {
            if (balls[iBall][jBall - 1].check == 1) // West
            {
                cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = balls[iBall][jBall - 1].check;
                return true;
            }
        }
        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty)
        {
            if (balls[iBall - 1][jBall].check == 1) // South
            {
                cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall << endl;
                balls[iBall][jBall].check = balls[iBall - 1][jBall].check;
                return true;
            }
        }
        if (iBall > 0 && jBall > 0 && !balls[iBall - 1][jBall - 1].isEmpty)
        {
            if (balls[iBall - 1][jBall - 1].check == 1) // South West
            {
                cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = balls[iBall - 1][jBall - 1].check;
                return true;
            }
        }

        // 2nd check

        balls[iBall][jBall].check = -1;

        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty)
        {
            if (shouldStick(iBall + 1, jBall)) // North
            {
                cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (jBall < columns - 1 && iBall < lines - 1 && !balls[iBall + 1][jBall + 1].isEmpty)
        {
            if (shouldStick(iBall + 1, jBall + 1)) // North East
            {
                cout << iBall << " " << jBall << "is connected to " << iBall + 1 << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty)
        {
            if (shouldStick(iBall, jBall + 1)) // East
            {
                cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall + 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty)
        {
            if (shouldStick(iBall - 1, jBall)) // South
            {
                cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty)
        {
            if (shouldStick(iBall, jBall - 1)) // West
            {
                cout << iBall << " " << jBall << "is connected to " << iBall << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (iBall > 0 && jBall > 0 && !balls[iBall - 1][jBall - 1].isEmpty)
        {
            if (shouldStick(iBall - 1, jBall - 1)) // South West
            {
                cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall - 1 << endl;
                balls[iBall][jBall].check = 1;
                return true;
            }
        }
        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty)
        {
            if (shouldStick(iBall - 1, jBall)) // South
            {
                cout << iBall << " " << jBall << "is connected to " << iBall - 1 << " " << jBall << endl;
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
    balls.resize(lines);

    int x_center = 12 + ballRadius;
    int y_center = 7 * ballRadius;

    ball new_ball;
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns + stick; j++)
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
            else if (rand_col % 4 == 0)
            {
                new_ball.color = 1; // yellow
            }
            if (rand_col == 21)
            {
                new_ball.color = 7; // black
            }
            else if (rand_col % 4 == 1)
            {
                new_ball.color = 2; // blue
            }
            if (rand_col % 4 == 2)
            {
                new_ball.color = 4; // green
            }
            if (rand_col % 4 == 3)
            {
                new_ball.color = 8; // red
            }

            balls[i].push_back(new_ball);
            x_center += 2 * ballRadius;
        }

        x_center = ballRadius + 12;
        y_center -= 2 * ballRadius;
    }
}

void draw_ball(SDL_Renderer *Renderer)
{
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns + stick; j++)
        {
            if (!balls[i][j].isEmpty)
            {
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
            crash_balls[1].color = 1;
            SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/1.png"), NULL, &Ball2);
            break;
        }
        case 1:
        {
            crash_balls[1].color = 2;
            SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/2.png"), NULL, &Ball2);
            break;
        }
        case 2:
        {
            crash_balls[1].color = 4;
            SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/4.png"), NULL, &Ball2);
            break;
        }
        case 3:
        {
            crash_balls[1].color = 8;
            SDL_RenderCopy(Renderer, IMG_LoadTexture(Renderer, "assets/Game/Balls/8.png"), NULL, &Ball2);
            break;
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
            for (int i = 0; i < lines - 1; i++)
            {
                for (int j = 0; j < columns + stick; j++)
                {
                    balls[i][j].check = 0;
                }
            }
            for (int i = lines - 1; i >= 0; i--)
            {
                for (int j = 0; j < columns + stick; j++)
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
    if (i < 0 || i > lines || j < 0 || j > columns)
        return;

    crashed.push_back(balls[i][j]);
    balls[i][j].isEmpty = true;

    for (int m = 0; m < lines; m++)
        for (int n = 0; n < columns; n++)
            if (!balls[m][n].isEmpty && balls[m][n].color != 7)
                if (areConnected(balls[i][j], balls[m][n]))
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
            if (crash_balls[0].x > WIDTH || crash_balls[0].x < 0)
                dx *= -1;
            if (crash_balls[0].y > HIGHT || crash_balls[0].y < 0)
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

            for (int i = 0; i < lines && !is_crash_ball_crashed; i++)
            {
                for (int j = 0; j < columns + stick && !is_crash_ball_crashed; j++)
                {
                    if (!balls[i][j].isEmpty)
                    {
                        if (areConnected(crash_balls[0], balls[i][j]))
                        {

                            is_crash_ball_crashed = true;
                            crash_balls[0].isEmpty = false;

                            if ((crash_balls[0].color == 11 && balls[i][j].color != 7) || (crash_balls[0].color == 1 && (balls[i][j].color % 2 != 0 && balls[i][j].color != 7)) || (crash_balls[0].color == 2 && (balls[i][j].color == 2 || balls[i][j].color == 3 || balls[i][j].color == 6 || balls[i][j].color == 10)) || (crash_balls[0].color == 4 && (balls[i][j].color == 4 || balls[i][j].color == 5 || balls[i][j].color == 6 || balls[i][j].color == 12)) || (crash_balls[0].color == 8 && (balls[i][j].color == 8 || balls[i][j].color == 9 || balls[i][j].color == 10 || balls[i][j].color == 12)))
                            {
                                isConnected(i, j);
                                crashed.push_back(crash_balls[0]);
                                crash_balls[0].isEmpty = true;
                                initial_crash_ball(Renderer);
                            }
                            else
                            {
                                // stick++
                                initial_crash_ball(Renderer);
                            }
                            for (int n = 0; n < lines; n++)
                                for (int m = 0; m < columns; m++)
                                    balls[n][m].y += 2 * ballRadius;
                        }
                    }
                }
            }
        }

        if (SDL_PollEvent(&event))
        {
            if (event.button.x != 0 && event.button.y != 0)
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
