// Samuel Weber

#include "Button.h"

Button::Button(SDL_Renderer *ren, const string &file, int x, int y, int w,
        int h, TTF_Font *font, const string &str)
{
  button.x = x;
  button.y = y;
  button.w = w;
  button.h = h;
  pic = loadTexture(file, ren);
  if (font)
    text = renderText(ren, font, str);
  else
    text = NULL;
}  // Button constructor

Button::~Button()
{
  SDL_DestroyTexture(pic);
  SDL_DestroyTexture(text);
}  // Button::~Button()

bool Button::buttonClick(SDL_MouseButtonEvent &click)
{
  bool cont = true;
  SDL_Event e;
  SDL_MouseButtonEvent release;
  if (click.state == SDL_RELEASED)
    return false;
  if ((click.x >= button.x) && (click.x <= (button.x + button.w)) &&
          (click.y >= button.y) && (click.y <= (button.y + button.h)))
  {
    while (cont)
    {
      if (SDL_PollEvent(&e))
      {
        if (e.type == SDL_MOUSEBUTTONUP)
        {
          release = e.button;
          cont = false;
        }  // wait for event == mousebuttonup
      }  // wait for event
    }  // loop while waiting
  }  // if mousebuttondown on button
  else
    return false; //did not click on button
  if ((release.x >= button.x) && (release.x <= (button.x + button.w)) &&
          (release.y >= button.y) && (release.y <= (button.y + button.h)))
    return true;
  return false;
}  // bool buttonClick(SDL_Rect &button, SDL_MouseButtonEvent &click)

SDL_Texture* Button::getPic()
{
  return pic;
}  // SDL_Texture* Button::getPic()

void Button::render(SDL_Renderer *ren)
{
  SDL_Rect clips[2];
  getClips(clips, 2, 2, 240, 100);
  renderTexture(pic, ren, SCREEN_WIDTH / 2 - 120, 300, &clips[0]);
  if (text)
    renderTexture(text, ren, button);
}  // void Button::render(SDL_Renderer *ren)

