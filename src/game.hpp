#ifndef game_hpp
#define game_hpp

#include <iostream>
#include "SDL_Headers.h"
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

bool check_win()
{
    for (int i = 0; i < lines + stick; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (balls[i][j].isEmpty == false && balls[i][j].color != 7)
            {
                cout << i << " " << j << " " << balls[i][j].isEmpty << endl;
                return false;
            }
        }
    }
    return true;
}

bool sameColor(ball ball1, ball ball2)
{
    if (ball2.color != 7 && (ball1.color == 11 || ball2.color == 11 || ball1.color == ball2.color || (ball1.color == 1 && ball2.color % 2 != 0) || (ball1.color == 2 && (ball2.color == 3 || ball2.color == 6 || ball2.color == 10)) || (ball1.color == 4 && (ball2.color == 5 || ball2.color == 6 || ball2.color == 12)) || (ball1.color == 8 && (ball2.color == 9 || ball2.color == 10 || ball2.color == 12)) || (ball1.color == 3 && (ball2.color == 1 || ball2.color == 2)) || (ball1.color == 5 && (ball2.color == 1 || ball2.color == 4)) || (ball1.color == 9 && (ball2.color == 1 || ball2.color == 8)) || (ball1.color == 6 && (ball2.color == 2 || ball2.color == 4)) || (ball1.color == 10 && (ball2.color == 2 || ball2.color == 8)) || (ball1.color == 12 && (ball2.color == 4 || ball2.color == 8))))
        return true;
    return false;
}

bool areConnected(ball ball1, ball ball2)
{
    if (pow(ball1.x - ball2.x, 2) + pow(ball1.y - ball2.y, 2) <= pow(2 * ballRadius, 2))
        return true;
    return false;
}

void draw_aimLine(SDL_Renderer *renderer)
{
    int dxm = x_mouse - crash_balls[0].x;
    int dym = y_mouse - crash_balls[0].y;

    float angle = atan2(dym, dxm);
    float y_end = crash_balls[0].y + (WIDTH - ballRadius - crash_balls[0].x) * tan(angle);

    if (x_mouse < crash_balls[0].x)
        y_end = crash_balls[0].y - (crash_balls[0].x - ballRadius) * tan(angle);

    if (y_end > 0 && y_end < HIGHT)
    {
        if (x_mouse > crash_balls[0].x)
        {
            lineRGBA(renderer, crash_balls[0].x, crash_balls[0].y, WIDTH - ballRadius, y_end, 255, 0, 0, 255);
            lineRGBA(renderer, WIDTH - ballRadius, y_end, ballRadius, y_end + WIDTH * tan(angle), 255, 0, 0, 255);
        }
        else
        {
            lineRGBA(renderer, crash_balls[0].x, crash_balls[0].y, ballRadius, y_end, 255, 0, 0, 255);
            lineRGBA(renderer, ballRadius, y_end, WIDTH - ballRadius, y_end - WIDTH * tan(angle), 255, 0, 0, 255);
        }
    }

    if (y_end <= 0 || y_end >= HIGHT)
    {
        float x_end = crash_balls[0].x - (crash_balls[0].y) / tan(angle);

        if (x_end > 0 && x_end < WIDTH)
        {
            lineRGBA(renderer, crash_balls[0].x, crash_balls[0].y, x_end, 0, 255, 0, 0, 255);
        }
    }
}

bool areConnectedBallsVector(ball ball1, ball ball2)
{
    int i1 = ball1.i, j1 = ball1.j, i2 = ball2.i, j2 = ball2.j;

    if (i1 % 2 == 0)
    {
        if (i1 == i2)
        {
            if (abs(j1 - j2) == 1)
                return true;
        }
        else if (i1 == i2 + 1)
        {
            if (j1 == j2 || j1 == j2 - 1)
                return true;
        }
        else if (i1 == i2 - 1)
        {
            if (j1 == j2 || j1 == j2 - 1)
                return true;
        }
    }
    else
    {
        if (i1 == i2)
        {
            if (j1 == j2 + 1 || j1 == j2 - 1)
                return true;
        }
        else if (i1 == i2 + 1)
        {
            if (j1 == j2 || j1 == j2 + 1)
                return true;
        }
        else if (i1 == i2 - 1)
        {
            if (j1 == j2 || j1 == j2 + 1)
                return true;
        }
    }
    return false;
}

int distance(ball ball1, ball ball2)
{
    return sqrt(pow(ball1.x - ball2.x, 2) + pow(ball1.y - ball2.y, 2));
}

bool shouldStick(int iBall, int jBall)
{
    if (balls[iBall][jBall].isEmpty)
        return false;

    if (lines < 50 && iBall == 0)
        return true;

    if (lines > 50 && abs(balls[iBall][jBall].y) <= ballRadius)
        return true;

    if (balls[iBall][jBall].stickCheck == -1)
        return false;

    if (iBall % 2 == 0)
    {
        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty)
        {
            if (balls[iBall - 1][jBall].stickCheck == 1) // North *
            {
                balls[iBall][jBall].stickCheck = balls[iBall - 1][jBall].stickCheck;
                return true;
            }
        }

        if (iBall > 0 && jBall > 0 && !balls[iBall - 1][jBall - 1].isEmpty)
        {
            if (balls[iBall - 1][jBall - 1].stickCheck == 1) // North West *
            {
                balls[iBall][jBall].stickCheck = balls[iBall - 1][jBall - 1].stickCheck;
                return true;
            }
        }

        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty)
        {
            if (balls[iBall][jBall + 1].stickCheck == 1) // East *
            {
                balls[iBall][jBall].stickCheck = balls[iBall][jBall + 1].stickCheck;
                return true;
            }
        }

        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty)
        {
            if (balls[iBall][jBall - 1].stickCheck == 1) // West *
            {
                balls[iBall][jBall].stickCheck = balls[iBall][jBall - 1].stickCheck;
                return true;
            }
        }

        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty)
            if (balls[iBall + 1][jBall].stickCheck == 1) // South *
            {
                balls[iBall][jBall].stickCheck = balls[iBall + 1][jBall].stickCheck;
                return true;
            }

        if (jBall > 0 && iBall < lines - 1 && !balls[iBall + 1][jBall - 1].isEmpty)
            if (balls[iBall + 1][jBall - 1].stickCheck == 1) // South West *
            {
                balls[iBall][jBall].stickCheck = balls[iBall + 1][jBall - 1].stickCheck;
                return true;
            }

        // 2nd stickCheck

        balls[iBall][jBall].stickCheck = -1;

        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty && balls[iBall - 1][jBall].stickCheck != -1)
        {
            if (shouldStick(iBall - 1, jBall)) // North *
            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }

        if (iBall > 0 && jBall > 0 && !balls[iBall - 1][jBall - 1].isEmpty && balls[iBall - 1][jBall - 1].stickCheck != -1)
        {
            if (shouldStick(iBall - 1, jBall - 1)) // North West *

            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }

        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty && balls[iBall][jBall - 1].stickCheck != -1)
        {
            if (shouldStick(iBall, jBall - 1)) // West *
            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }

        if (iBall < lines - 1 && jBall > 0 && !balls[iBall + 1][jBall - 1].isEmpty && balls[iBall + 1][jBall - 1].stickCheck != -1)
        {
            if (shouldStick(iBall + 1, jBall - 1)) // South West *

            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }

        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty && balls[iBall][jBall + 1].stickCheck != -1)
        {
            if (shouldStick(iBall, jBall + 1)) // East *
            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }
        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty && balls[iBall + 1][jBall].stickCheck != -1)
        {
            if (shouldStick(iBall + 1, jBall)) // South *

            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }

        return false;
    }
    else
    {
        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty)
        {
            if (balls[iBall - 1][jBall].stickCheck == 1) // North *

            {
                balls[iBall][jBall].stickCheck = balls[iBall - 1][jBall].stickCheck;
                return true;
            }
        }

        if (iBall > 0 && jBall < columns - 1 && !balls[iBall - 1][jBall + 1].isEmpty)
        {
            if (balls[iBall - 1][jBall + 1].stickCheck == 1) // North East *

            {
                balls[iBall][jBall].stickCheck = balls[iBall - 1][jBall - 1].stickCheck;
                return true;
            }
        }

        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty)
        {
            if (balls[iBall][jBall + 1].stickCheck == 1) // East *
            {
                balls[iBall][jBall].stickCheck = balls[iBall][jBall + 1].stickCheck;
                return true;
            }
        }

        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty)
        {
            if (balls[iBall][jBall - 1].stickCheck == 1) // West *

            {
                balls[iBall][jBall].stickCheck = balls[iBall][jBall - 1].stickCheck;
                return true;
            }
        }

        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty)
            if (balls[iBall + 1][jBall].stickCheck == 1) // South *
            {
                balls[iBall][jBall].stickCheck = balls[iBall + 1][jBall].stickCheck;
                return true;
            }

        if (jBall < columns - 1 && iBall < lines - 1 && !balls[iBall + 1][jBall + 1].isEmpty)
            if (balls[iBall + 1][jBall + 1].stickCheck == 1) // South East *
            {
                balls[iBall][jBall].stickCheck = balls[iBall + 1][jBall + 1].stickCheck;
                return true;
            }

        // 2nd stickCheck

        balls[iBall][jBall].stickCheck = -1;

        if (iBall > 0 && jBall < columns - 1 && !balls[iBall - 1][jBall + 1].isEmpty && balls[iBall - 1][jBall + 1].stickCheck != -1)
        {
            if (shouldStick(iBall - 1, jBall + 1)) // North East *
            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }

        if (iBall > 0 && !balls[iBall - 1][jBall].isEmpty && balls[iBall - 1][jBall].stickCheck != -1)
        {
            if (shouldStick(iBall - 1, jBall)) // North *
            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }

        if (jBall < columns - 1 && !balls[iBall][jBall + 1].isEmpty && balls[iBall][jBall + 1].stickCheck != -1)
        {
            if (shouldStick(iBall, jBall + 1)) // East *
            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }

        if (jBall < columns - 1 && iBall < lines - 1 && !balls[iBall + 1][jBall + 1].isEmpty && balls[iBall + 1][jBall + 1].stickCheck != -1)
        {
            if (shouldStick(iBall + 1, jBall + 1)) // South East *
            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }

        if (jBall > 0 && !balls[iBall][jBall - 1].isEmpty && balls[iBall][jBall - 1].stickCheck != -1)
        {
            if (shouldStick(iBall, jBall - 1)) // West *
            {
                balls[iBall][jBall].stickCheck = 1;
                return true;
            }
        }

        if (iBall < lines - 1 && !balls[iBall + 1][jBall].isEmpty && balls[iBall + 1][jBall].stickCheck != -1)
        {
            if (shouldStick(iBall + 1, jBall)) // South *
            {
                balls[iBall][jBall].stickCheck = 1;
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
    int y_center = -(lines * sqrt(3) * ballRadius) + HIGHT - 150;

    ball new_ball;
    int last_color = -1;

    for (int i = 0; i < lines + stick; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            new_ball.x = x_center;
            new_ball.y = y_center;
            new_ball.i = i;
            new_ball.j = j;

            int rand_col = rand() % 5;

            if (rand_col != 0 || last_color == -1)
            {
                rand_col = rand() % 22;

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
                    if (i != 0)
                    {
                        new_ball.color = 7; // black
                    }
                    else
                    {
                        rand_col = rand() % 21;
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
                        else if (rand_col % 4 == 1 && rand_col < 22)
                        {
                            new_ball.color = 2; // blue
                        }
                        else if (rand_col % 4 == 2 && rand_col < 22)
                        {
                            new_ball.color = 4; // green
                        }
                        else if (rand_col % 4 == 3 && rand_col < 22)
                        {
                            new_ball.color = 8; // red
                        }
                    }
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
            }
            else
            {
                new_ball.color = last_color;
            }
            last_color = new_ball.color;
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
}

void draw_ball(SDL_Renderer *Renderer)
{
    for (int i = 0; i < 4; i++)
        crash_ball_color[i] = false;

    for (int i = 0; i < fallingBall.size(); i++)
        if (fallingBall[i].y < HIGHT)
        {
            SDL_Rect fallingBallRect = {int(fallingBall[i].x - ballRadius), int(fallingBall[i].y - ballRadius), 2 * ballRadius, 2 * ballRadius};
            switch (fallingBall[i].color)
            {
            case 1:
                SDL_RenderCopy(Renderer, ball1, NULL, &fallingBallRect);
                break;
            case 2:
                SDL_RenderCopy(Renderer, ball2, NULL, &fallingBallRect);
                break;
            case 3:
                SDL_RenderCopy(Renderer, ball3, NULL, &fallingBallRect);
                break;
            case 4:
                SDL_RenderCopy(Renderer, ball4, NULL, &fallingBallRect);
                break;
            case 5:
                SDL_RenderCopy(Renderer, ball5, NULL, &fallingBallRect);
                break;
            case 6:
                SDL_RenderCopy(Renderer, ball6, NULL, &fallingBallRect);
                break;
            case 7:
                SDL_RenderCopy(Renderer, ball7, NULL, &fallingBallRect);
                break;
            case 8:
                SDL_RenderCopy(Renderer, ball8, NULL, &fallingBallRect);
                break;
            case 9:
                SDL_RenderCopy(Renderer, ball9, NULL, &fallingBallRect);
                break;
            case 10:
                SDL_RenderCopy(Renderer, ball10, NULL, &fallingBallRect);
                break;
            case 11:
                SDL_RenderCopy(Renderer, ball11, NULL, &fallingBallRect);
                break;
            case 12:
                SDL_RenderCopy(Renderer, ball12, NULL, &fallingBallRect);
                break;
            }
            fallingBall[i].y += dy_fallingBall;
        }

    for (int i = 0; i < crashed.size(); i++)
        if (crashed[i].r > 1)
        {
            SDL_Rect crashedBall = {int(crashed[i].x - crashed[i].r), int(crashed[i].y - crashed[i].r), 2 * crashed[i].r, 2 * crashed[i].r};
            switch (crashed[i].color)
            {
            case 1:
                SDL_RenderCopy(Renderer, ball1, NULL, &crashedBall);
                break;
            case 2:
                SDL_RenderCopy(Renderer, ball2, NULL, &crashedBall);
                break;
            case 3:
                SDL_RenderCopy(Renderer, ball3, NULL, &crashedBall);
                break;
            case 4:
                SDL_RenderCopy(Renderer, ball4, NULL, &crashedBall);
                break;
            case 5:
                SDL_RenderCopy(Renderer, ball5, NULL, &crashedBall);
                break;
            case 6:
                SDL_RenderCopy(Renderer, ball6, NULL, &crashedBall);
                break;
            case 7:
                SDL_RenderCopy(Renderer, ball7, NULL, &crashedBall);
                break;
            case 8:
                SDL_RenderCopy(Renderer, ball8, NULL, &crashedBall);
                break;
            case 9:
                SDL_RenderCopy(Renderer, ball9, NULL, &crashedBall);
                break;
            case 10:
                SDL_RenderCopy(Renderer, ball10, NULL, &crashedBall);
                break;
            case 11:
                SDL_RenderCopy(Renderer, ball11, NULL, &crashedBall);
                break;
            case 12:
                SDL_RenderCopy(Renderer, ball12, NULL, &crashedBall);
                break;
            }
            crashed[i].r /= 1.3;
        }

    for (int i = 0; i < lines + stick; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (!balls[i][j].isEmpty)
            {
                int x_center = balls[i][j].x, y_center = balls[i][j].y;
                SDL_Rect Ball = {x_center - ballRadius, y_center - ballRadius, 2 * ballRadius, 2 * ballRadius};
                switch (balls[i][j].color)
                {
                case 1:
                    SDL_RenderCopy(Renderer, ball1, NULL, &Ball);
                    crash_ball_color[0] = true;
                    break;

                case 2:
                    SDL_RenderCopy(Renderer, ball2, NULL, &Ball);
                    crash_ball_color[1] = true;
                    break;

                case 3:
                    SDL_RenderCopy(Renderer, ball3, NULL, &Ball);
                    crash_ball_color[0] = true;
                    crash_ball_color[1] = true;
                    break;

                case 4:
                    SDL_RenderCopy(Renderer, ball4, NULL, &Ball);
                    crash_ball_color[2] = true;
                    break;

                case 5:
                    SDL_RenderCopy(Renderer, ball5, NULL, &Ball);
                    crash_ball_color[0] = true;
                    crash_ball_color[2] = true;
                    break;

                case 6:
                    SDL_RenderCopy(Renderer, ball6, NULL, &Ball);
                    crash_ball_color[1] = true;
                    crash_ball_color[2] = true;
                    break;

                case 7:
                    SDL_RenderCopy(Renderer, ball7, NULL, &Ball);
                    break;

                case 8:
                    SDL_RenderCopy(Renderer, ball8, NULL, &Ball);
                    crash_ball_color[3] = true;
                    break;

                case 9:
                    SDL_RenderCopy(Renderer, ball9, NULL, &Ball);
                    crash_ball_color[0] = true;
                    crash_ball_color[3] = true;
                    break;

                case 10:
                    SDL_RenderCopy(Renderer, ball10, NULL, &Ball);
                    crash_ball_color[1] = true;
                    crash_ball_color[3] = true;
                    break;

                case 11:
                    SDL_RenderCopy(Renderer, ball11, NULL, &Ball);
                    break;

                case 12:
                    SDL_RenderCopy(Renderer, ball12, NULL, &Ball);
                    crash_ball_color[2] = true;
                    crash_ball_color[3] = true;
                    break;
                }
                if (balls[i][j].y > HIGHT - 100 - 2 * ballRadius)
                {
                    lose = true;
                }
            }
        }
    }
}

void rotateCannonWithMouse(SDL_Renderer *renderer, SDL_Texture *image)
{

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_Rect cannonRect = {int(WIDTH / 2 - 85), int(HIGHT - 170), 170, 170};

    int dx = mouseX - 315;
    int dy = mouseY - 900;
    double angle = atan2(dy, dx) * 180 / M_PI;

    SDL_RenderCopyEx(renderer, image, nullptr, &cannonRect, angle, nullptr, SDL_FLIP_NONE);
}

void initial_crash_ball(SDL_Renderer *Renderer)
{
    crash_balls[0].x = int(WIDTH / 2);
    crash_balls[0].y = int(HIGHT - 100);
    crash_balls[0].color = crash_balls[1].color;
    crash_balls[0].isEmpty = false;

    crash_balls[1].x = 317;
    crash_balls[1].y = 920;

    bool regenerate = false;

    if ((crash_balls[0].color == 1 && crash_ball_color[0] == false) || (crash_balls[0].color == 2 && crash_ball_color[1] == false) || (crash_balls[0].color == 4 && crash_ball_color[2] == false) || (crash_balls[0].color == 8 && crash_ball_color[3] == false))
        regenerate = true;

    if (regenerate)
    {
        bool isColorSelected = true;
        while (isColorSelected)
        {
            switch (rand() % 4)
            {
            case 0:
            {
                if (crash_ball_color[0])
                {
                    crash_balls[0].color = 1;
                    isColorSelected = false;
                }
                break;
            }
            case 1:
            {
                if (crash_ball_color[1])
                {
                    crash_balls[0].color = 2;
                    isColorSelected = false;
                }
                break;
            }
            case 2:
            {
                if (crash_ball_color[2])
                {
                    crash_balls[0].color = 4;
                    isColorSelected = false;
                }
                break;
            }
            case 3:
            {
                if (crash_ball_color[3])
                {
                    crash_balls[0].color = 8;
                    isColorSelected = false;
                }
                break;
            }
            }
        }
    }

    if (rand() % 17 == 0)
    {
        crash_balls[1].color = 11;
    }
    else
    {
        bool isColorSelected = true;
        while (isColorSelected)
        {
            switch (rand() % 4)
            {
            case 0:
            {
                if (crash_ball_color[0])
                {
                    crash_balls[1].color = 1;
                    isColorSelected = false;
                }
                break;
            }
            case 1:
            {
                if (crash_ball_color[1])
                {
                    crash_balls[1].color = 2;
                    isColorSelected = false;
                }
                break;
            }
            case 2:
            {
                if (crash_ball_color[2])
                {
                    crash_balls[1].color = 4;
                    isColorSelected = false;
                }
                break;
            }
            case 3:
            {
                if (crash_ball_color[3])
                {
                    crash_balls[1].color = 8;
                    isColorSelected = false;
                }
                break;
            }
            }
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
                    balls[i][j].stickCheck = 0;
                }
            }
            for (int i = 1; i < lines; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    if (!balls[i][j].isEmpty)
                    {
                        if (!shouldStick(i, j))
                        {
                            cout << i << " " << j << " ShouldN't stick" << endl;
                            crashed_score++;
                            balls[i][j].isEmpty = true;
                            fallingBall.push_back(balls[i][j]);
                            repeat = true;
                        }
                    }
                }
            }
        }
    }
    win = check_win();
    is_crash_ball_crashed = false;
    is_crash_ball_moved = false;
}

void isConnected(ball theBall)
{
    if (theBall.j > 0 && theBall.j < columns - 1 && theBall.i > 0 && theBall.i < lines + stick - 1)
    {
        for (int i = theBall.i - 1; i <= theBall.i + 1; i++)
            for (int j = theBall.j - 1; j <= theBall.j + 1; j++)
            {
                if (!balls[i][j].isEmpty && (theBall.i != i || theBall.j != j))
                {
                    if (sameColor(theBall, balls[i][j]) && areConnected(theBall, balls[i][j]))
                    {
                        crashed.push_back(balls[i][j]);
                        balls[i][j].isEmpty = true;
                        crashed_score++;
                        isConnected(balls[i][j]);
                    }
                }
            }
    }
    else if (theBall.i == 0 && theBall.j > 0 && theBall.j < columns - 1)
    {
        for (int i = theBall.i; i <= theBall.i + 1; i++)
            for (int j = theBall.j - 1; j <= theBall.j + 1; j++)
            {
                if (!balls[i][j].isEmpty && (theBall.i != i || theBall.j != j))
                {
                    if (sameColor(theBall, balls[i][j]) && areConnected(theBall, balls[i][j]))
                    {
                        crashed.push_back(balls[i][j]);
                        balls[i][j].isEmpty = true;
                        crashed_score++;
                        isConnected(balls[i][j]);
                    }
                }
            }
    }
    else if (theBall.i == 0 && theBall.j == 0)
    {
        for (int i = theBall.i; i <= theBall.i + 1; i++)
            for (int j = theBall.j; j <= theBall.j + 1; j++)
            {
                if (!balls[i][j].isEmpty && (theBall.i != i || theBall.j != j))
                {
                    if (sameColor(theBall, balls[i][j]) && areConnected(theBall, balls[i][j]))
                    {
                        crashed.push_back(balls[i][j]);
                        balls[i][j].isEmpty = true;
                        crashed_score++;
                        isConnected(balls[i][j]);
                    }
                }
            }
    }
    else if (theBall.i == 0 && theBall.j == columns - 1)
    {
        for (int i = theBall.i; i <= theBall.i + 1; i++)
            for (int j = theBall.j - 1; j <= theBall.j; j++)
            {
                if (!balls[i][j].isEmpty && (theBall.i != i || theBall.j != j))
                {
                    if (sameColor(theBall, balls[i][j]) && areConnected(theBall, balls[i][j]))
                    {
                        crashed.push_back(balls[i][j]);
                        balls[i][j].isEmpty = true;
                        crashed_score++;
                        isConnected(balls[i][j]);
                    }
                }
            }
    }
    else if (theBall.i > 0 && theBall.i < lines + stick - 1 && theBall.j == 0)
    {
        for (int i = theBall.i - 1; i <= theBall.i + 1; i++)
            for (int j = theBall.j; j <= theBall.j + 1; j++)
            {
                if (!balls[i][j].isEmpty && (theBall.i != i || theBall.j != j))
                {
                    if (sameColor(theBall, balls[i][j]) && areConnected(theBall, balls[i][j]))
                    {
                        crashed.push_back(balls[i][j]);
                        balls[i][j].isEmpty = true;
                        crashed_score++;
                        isConnected(balls[i][j]);
                    }
                }
            }
    }
    else if (theBall.i > 0 && theBall.i < lines + stick - 1 && theBall.j == columns - 1)
    {
        for (int i = theBall.i - 1; i <= theBall.i + 1; i++)
            for (int j = theBall.j - 1; j <= theBall.j; j++)
            {
                if (!balls[i][j].isEmpty && (theBall.i != i || theBall.j != j))
                {
                    if (sameColor(theBall, balls[i][j]) && areConnected(theBall, balls[i][j]))
                    {
                        crashed.push_back(balls[i][j]);
                        balls[i][j].isEmpty = true;
                        crashed_score++;
                        isConnected(balls[i][j]);
                    }
                }
            }
    }
    else if (theBall.i == lines + stick - 1 && theBall.j > 0 && theBall.j < columns - 1)
    {
        for (int i = theBall.i - 1; i <= theBall.i; i++)
            for (int j = theBall.j - 1; j <= theBall.j + 1; j++)
            {
                if (!balls[i][j].isEmpty && (theBall.i != i || theBall.j != j))
                {
                    if (sameColor(theBall, balls[i][j]) && areConnected(theBall, balls[i][j]))
                    {
                        crashed.push_back(balls[i][j]);
                        balls[i][j].isEmpty = true;
                        crashed_score++;
                        isConnected(balls[i][j]);
                    }
                }
            }
    }
    else if (theBall.i == lines + stick - 1 && theBall.j == 0)
    {
        for (int i = theBall.i - 1; i <= theBall.i; i++)
            for (int j = theBall.j; j <= theBall.j + 1; j++)
            {
                if (!balls[i][j].isEmpty && (theBall.i != i || theBall.j != j))
                {
                    if (sameColor(theBall, balls[i][j]) && areConnected(theBall, balls[i][j]))
                    {
                        crashed.push_back(balls[i][j]);
                        balls[i][j].isEmpty = true;
                        crashed_score++;
                        isConnected(balls[i][j]);
                    }
                }
            }
    }
    else if (theBall.i == lines + stick - 1 && theBall.j == columns - 1)
    {
        for (int i = theBall.i - 1; i <= theBall.i; i++)
            for (int j = theBall.j - 1; j <= theBall.j; j++)
            {
                if (!balls[i][j].isEmpty && (theBall.i != i || theBall.j != j))
                {
                    if (sameColor(theBall, balls[i][j]) && areConnected(theBall, balls[i][j]))
                    {
                        crashed.push_back(balls[i][j]);
                        balls[i][j].isEmpty = true;
                        crashed_score++;
                        isConnected(balls[i][j]);
                    }
                }
            }
    }
}

void crashed_ball(SDL_Renderer *Renderer)
{
    SDL_Rect Ball = {int(crash_balls[0].x - ballRadius), int(crash_balls[0].y - ballRadius), 2 * ballRadius, 2 * ballRadius};

    if (!is_crash_ball_crashed)
    {
        if (!is_crash_ball_moved)
        {
            draw_aimLine(Renderer);
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
                SDL_RenderCopy(Renderer, ball1, NULL, &Ball);
                break;
            }
            case 2:
            {
                SDL_RenderCopy(Renderer, ball2, NULL, &Ball);
                break;
            }
            case 4:
            {
                SDL_RenderCopy(Renderer, ball4, NULL, &Ball);
                break;
            }
            case 8:
            {
                SDL_RenderCopy(Renderer, ball8, NULL, &Ball);
                break;
            }
            case 11:
            {
                SDL_RenderCopy(Renderer, ball11, NULL, &Ball);
                break;
            }
            }

            int jStick, iStick;
            for (int i = lines + stick - 1; i >= 0 && !is_crash_ball_crashed; i--)
            {
                for (int j = 0; j < columns && !is_crash_ball_crashed; j++)
                {
                    if (!balls[i][j].isEmpty)
                    {
                        if (sqrt((crash_balls[0].x - balls[i][j].x) * (crash_balls[0].x - balls[i][j].x) + (crash_balls[0].y - balls[i][j].y) * (crash_balls[0].y - balls[i][j].y)) < 1.5 * ballRadius || crash_balls[0].y < balls[0][0].y)
                        {

                            iStick = ceil((crash_balls[0].y - balls[0][0].y) / (sqrt(3) * ballRadius));

                            if (iStick % 2 == 0)
                            {
                                int ceilJstick = ceil((crash_balls[0].x - balls[0][0].x) / (2 * ballRadius)), floorJstick = floor((crash_balls[0].x - balls[0][0].x) / (2 * ballRadius));
                                if (abs(crash_balls[0].x - balls[iStick][ceilJstick].x) < abs(crash_balls[0].x - balls[iStick][floorJstick].x))
                                    jStick = ceilJstick;
                                else
                                    jStick = floorJstick;
                            }
                            else
                            {
                                int ceilJstick = ceil((crash_balls[0].x - balls[1][0].x) / (2 * ballRadius)), floorJstick = floor((crash_balls[0].x - balls[1][0].x) / (2 * ballRadius));
                                if (abs(crash_balls[0].x - balls[iStick][ceilJstick].x) < abs(crash_balls[0].x - balls[iStick][floorJstick].x))
                                    jStick = ceilJstick;
                                else
                                    jStick = floorJstick;
                            }

                            is_crash_ball_crashed = true;
                            cout << "ball x = " << balls[i][j].x << " ball y = " << balls[i][j].y << " i=" << i << " j=" << j << endl
                                 << "crash x = " << crash_balls[0].x << " crash y = " << crash_balls[0].y << endl;
                        }
                    }
                }
            }
            if (is_crash_ball_crashed)
            {
                Mix_PlayChannel(-1, crash, 0);

                if (jStick < 0)
                    jStick = 0;
                if (jStick > columns - 1)
                    jStick = columns - 1;

                balls[iStick][jStick].color = crash_balls[0].color;
                balls[iStick][jStick].isEmpty = false;

                crashed.clear();

                isConnected(balls[iStick][jStick]);

                if (crashed.size() < 3)
                {
                    for (int i = 0; i < crashed.size(); i++)
                    {
                        balls[crashed[i].i][crashed[i].j].isEmpty = false;
                        crashed_score--;
                    }
                }
                initial_crash_ball(Renderer);
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
                if (!is_crash_ball_crashed && !is_crash_ball_moved)
                {
                    Mix_PlayChannel(-1, movement_swipe, 0);
                    is_crash_ball_moved = true;
                    score--;
                    dx = d * ((x_mouse - crash_balls[0].x) / sqrt(pow(x_mouse - crash_balls[0].x, 2) + pow(y_mouse - crash_balls[0].y, 2)));
                    dy = d * ((y_mouse - crash_balls[0].y) / sqrt(pow(x_mouse - crash_balls[0].x, 2) + pow(y_mouse - crash_balls[0].y, 2)));
                }
            }
        }
    }
    if (!is_crash_ball_moved)
    {
        switch (crash_balls[0].color)
        {
        case 1:
        {
            SDL_Texture *cannon1 = IMG_LoadTexture(Renderer, "assets/Game/cannon/cannon1.png");
            rotateCannonWithMouse(Renderer, cannon1);
            break;
        }
        case 2:
        {
            SDL_Texture *cannon2 = IMG_LoadTexture(Renderer, "assets/Game/cannon/cannon2.png");
            rotateCannonWithMouse(Renderer, cannon2);
            break;
        }
        case 4:
        {
            SDL_Texture *cannon4 = IMG_LoadTexture(Renderer, "assets/Game/cannon/cannon4.png");
            rotateCannonWithMouse(Renderer, cannon4);
            break;
        }
        case 8:
        {
            SDL_Texture *cannon8 = IMG_LoadTexture(Renderer, "assets/Game/cannon/cannon8.png");
            rotateCannonWithMouse(Renderer, cannon8);
            break;
        }
        case 11:
        {
            SDL_Texture *cannon11 = IMG_LoadTexture(Renderer, "assets/Game/cannon/cannon11.png");
            rotateCannonWithMouse(Renderer, cannon11);
            break;
        }
        }
    }
    else
    {
        SDL_Texture *cannonEmpty = IMG_LoadTexture(Renderer, "assets/Game/cannon/cannonEmpty.png");
        rotateCannonWithMouse(Renderer, cannonEmpty);
    }

    SDL_Rect Ball2 = {302, 910, 20, 20};
    switch (crash_balls[1].color)
    {
    case 1:
    {
        SDL_RenderCopy(Renderer, ball1, NULL, &Ball2);
        break;
    }
    case 2:
    {
        SDL_RenderCopy(Renderer, ball2, NULL, &Ball2);
        break;
    }
    case 4:
    {
        SDL_RenderCopy(Renderer, ball4, NULL, &Ball2);
        break;
    }
    case 8:
    {
        SDL_RenderCopy(Renderer, ball8, NULL, &Ball2);
        break;
    }
    case 11:
    {
        SDL_RenderCopy(Renderer, ball11, NULL, &Ball2);
        break;
    }
    }
}

int gameScore()
{
    if (lose)
    {
        for (int i = 0; i < lines + stick; i++)
            for (int j = 0; j < columns; j++)
                if (!balls[i][j].isEmpty && balls[i][j].color != 7)
                    score--;
    }
    return score;
}

int timeScore(Uint32 elapsed_time)
{
    if (win)
    {
        return score + (end_time - elapsed_time) / 1000;
    }
    else
    {
        for (int i = 0; i < lines + stick; i++)
            for (int j = 0; j < columns; j++)
                if (!balls[i][j].isEmpty && balls[i][j].color != 7)
                    score -= 2;
        return score;
    }
}

int infinityScore()
{
    return (score + crashed_score);
}

void showUserScore(SDL_Renderer *renderer, TTF_Font *font, int score, int x, int y)
{
    SDL_Color color = {34, 138, 237, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, to_string(score).c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
#endif // !game_hpp