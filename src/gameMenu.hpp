#ifndef gameMenu_hpp
#include "SDL_Headers.h"

struct button
{
  int x, y, w, h;
  int red, green, blue, alpha;
  bool isClicked;
  bool wasHovered;
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

bool isClicked(SDL_Event event, int x, int y, int w, int h)
{
  if (event.type == SDL_MOUSEBUTTONDOWN)
  {
    if (event.button.button == SDL_BUTTON_LEFT)
    {
      if (event.button.x >= x && event.button.x <= x + w && event.button.y >= y && event.button.y <= y + h)
      {
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
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, b.alpha);
    if (isClicked(event, b.x, b.y, b.w, b.h))
    {
      b.isClicked = true;
    }
  }
  else
  {
    SDL_SetRenderDrawColor(renderer, b.red, b.green, b.blue, b.alpha);
  }
  SDL_Rect rect = {b.x, b.y, b.w, b.h};
  SDL_RenderFillRect(renderer, &rect);
}

#endif // !gameMenu_hpp