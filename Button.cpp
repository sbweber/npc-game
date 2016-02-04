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
  SDL_MouseButtonEvent release;
  if (click.state == SDL_RELEASED)
    return false;
  if ((click.x >= button.x) && (click.x <= (button.x + button.w)) &&
          (click.y >= button.y) && (click.y <= (button.y + button.h)))
  {
    render(ren, true);
    SDL_RenderPresent(ren);
    while (true)
    {
      if (SDL_PollEvent(&e))
      {
        if (e.type == SDL_MOUSEBUTTONUP)
        {
          render(ren, false);
          SDL_RenderPresent(ren);
          release = e.button;
          if ((release.x >= button.x) &&
                  (release.x <= (button.x + button.w)) &&
                  (release.y >= button.y) &&
                  (release.y <= (button.y + button.h)))
            return true;
          else
            return false;
        }  // wait for event == mousebuttonup
      }  // wait for event
    }  // loop while waiting
  }  // if mousebuttondown on button
  return false;  // did not click on button
}  // bool buttonClick(SDL_Rect &button, SDL_MouseButtonEvent &click)


void Button::render(SDL_Renderer *ren, bool isClicking)
{
  SDL_Rect clips[2];
  getClips(clips, 2, 2, 240, 100);
  if (isClicking)
    renderTexture(pic, ren, button.x, button.y, &clips[1]);
  else
    renderTexture(pic, ren, button.x, button.y, &clips[0]);
  if (text)
    renderTexture(text, ren, button);
}  // void Button::render(SDL_Renderer *ren)

