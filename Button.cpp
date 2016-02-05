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
    text = nullptr;
}  // Button constructor


Button::~Button()
{
  SDL_DestroyTexture(pic);
  SDL_DestroyTexture(text);
}  // Button::~Button()


bool Button::buttonClick(SDL_Renderer *ren, SDL_MouseButtonEvent &click)
{
  SDL_Event e;
  if (click.state == SDL_RELEASED)
    return false;
  if (mouseOnButton(click))
  {
    render(ren, buttonClicked);
    SDL_RenderPresent(ren);
    while (true)
    {
      if (SDL_PollEvent(&e))
      {
        if (e.type == SDL_MOUSEBUTTONUP)
        {
          render(ren, buttonUp);
          SDL_RenderPresent(ren);
          if (mouseOnButton(e.button))
            return true;
          else
            return false;
        }  // wait for event == mousebuttonup
        else if (e.type == SDL_MOUSEMOTION)
        {
          if (mouseOnButton(e.motion))
          {
            render(ren, buttonClicked);
            SDL_RenderPresent(ren);
          }
          else
          {
            render(ren, buttonUp);
            SDL_RenderPresent(ren);
          }
        }  // If mouse moves off button, lift it.
      }  // wait for event
    }  // loop while waiting
  }  // if mousebuttondown on button
  return false;  // did not click on button
}  // bool buttonClick(SDL_Rect &button, SDL_MouseButtonEvent &click)


bool Button::mouseOnButton(int x, int y)
{
  if ((x >= button.x) && (x <= (button.x + button.w)) &&
          (y >= button.y) && (y <= (button.y + button.h)))
    return true;
  return false;
}  // bool Button::mouseOnButton(int x, int y)


bool Button::mouseOnButton(SDL_MouseButtonEvent &mouse)
{
  return mouseOnButton(mouse.x, mouse.y);
}  // bool Button::mouseOnButton(SDL_MouseButtonEvent &mouse)


bool Button::mouseOnButton(SDL_MouseMotionEvent &mouse)
{
  return mouseOnButton(mouse.x, mouse.y);
}  // bool Button::mouseOnButton(SDL_MouseMotionEvent &mouse)


void Button::render(SDL_Renderer *ren, buttonState state)
{
  SDL_Rect clips[3];
  getClips(clips, 3, 3, 240, 100);
  switch (state)
  {
  case buttonUp:
    renderTexture(pic, ren, button.x, button.y, &clips[0]);
    break;
  case buttonClicked:
    renderTexture(pic, ren, button.x, button.y, &clips[1]);
    break;
  case buttonSelected:
    renderTexture(pic, ren, button.x, button.y, &clips[2]);
    break;
  }
  if (text)
    renderTexture(text, ren, button);
}  // void Button::render(SDL_Renderer *ren)

