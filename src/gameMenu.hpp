#ifndef gameMenu_hpp
#define gameMenu_hpp
#include "SDL_Headers.h"
struct button
{
  int w, h, x, y;
  int red, green, blue, alpha;
  int stroke;
  bool wasClicked;
  bool wasHovered;
  SDL_Texture *image, *hoveredImage, *clickedImage;
};

bool isHovered(SDL_Event event, int x, int y, int w, int h, bool &wasHovered)
{
  if (event.motion.x != 0 && event.motion.y != 0 && !(event.motion.x >= x && event.motion.x <= x + w && event.motion.y >= y && event.motion.y <= y + h))
  {
    wasHovered = false;
    return false;
  }
  else if (wasHovered)
  {
    return true;
  }
  else if (event.motion.x >= x && event.motion.x <= x + w && event.motion.y >= y && event.motion.y <= y + h)
  {
    wasHovered = true;
    return true;
  }
  return false;
}

bool isClicked(SDL_Event event, int x, int y, int w, int h, bool &wasClicked)
{
  if (wasClicked)
  {
    return true;
  }
  if (event.type == SDL_MOUSEBUTTONDOWN)
  {
    if (event.button.button == SDL_BUTTON_LEFT)
    {
      if (event.button.x >= x && event.button.x <= x + w && event.button.y >= y && event.button.y <= y + h)
      {
        wasClicked = true;
        return true;
      }
    }
  }
  return false;
}

void drawButton(SDL_Renderer *renderer, button &b, SDL_Event event)
{
  if (isHovered(event, b.x, b.y, b.w, b.h, b.wasHovered))
  {
    SDL_Rect rect = {b.x - b.stroke, b.y - b.stroke, b.w + 2 * b.stroke, b.h + 2 * b.stroke};
    SDL_RenderCopy(renderer, b.hoveredImage, NULL, &rect);
    if (isClicked(event, b.x, b.y, b.w, b.h, b.wasClicked))
    {
      SDL_RenderCopy(renderer, b.clickedImage, NULL, &rect);
    }
  }
  else
  {
    SDL_Rect rect = {b.x, b.y, b.w, b.h};
    b.wasClicked = false;
    SDL_RenderCopy(renderer, b.image, NULL, &rect);
  }
}

void showStartScreen(SDL_Renderer *renderer, SDL_Texture *StartScreen, SDL_Event event)
{
  if (SDL_PollEvent(&event))
  {
    SDL_RenderCopy(renderer, StartScreen, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(4000);
    SDL_RenderClear(renderer);
  }
}

#endif // !gameMenu_hpp