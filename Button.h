// Samuel Weber

#ifndef BUTTON_H
  #define BUTTON_H

  #include "Draw.h"
  #include "Globals.h"

  class Button
  {
    SDL_Rect button;
    SDL_Texture* pic;  // visual for button
    SDL_Texture* text;  // optional text for button
  public:
    Button(SDL_Renderer *ren, const string &file, int x, int y, int w, int h,
            TTF_Font *font = NULL, const string &s = "");
            // no default constructor, must give position, dimensions, and
            // filename for pic. String and font optional.
    ~Button();
    bool buttonClick(SDL_MouseButtonEvent &click);
            // true if button was clicked
    SDL_Texture* getPic();
    void render(SDL_Renderer *ren);
  };


#endif