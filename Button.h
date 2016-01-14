// Samuel Weber

#ifndef BUTTON_H
  #define BUTTON_H

  #include "Render.h"
  #include "Globals.h"

  //! Describes a clickable button.
  class Button
  {
  public:
    Button(SDL_Renderer *ren, const string &file, int x, int y, int w, int h,
            TTF_Font *font = nullptr, const string &s = "");
    /*!< Button does not have a default constructor. A Button must have
     *   a position and dimensions, and the constructor requires a
     *   filename for the background pic. Text and font are optional.
     */
    ~Button();
    bool buttonClick(SDL_MouseButtonEvent &click);
    //!< Returns true if Button was clicked
    SDL_Texture* getPic();
    //!< Returns Button's background picture
    void render(SDL_Renderer *ren);
    //!< Draws Button on the renderer
  protected:
    SDL_Rect button;
    //!< Rectangle describing Button's position and dimensions
    SDL_Texture* pic;
    //!< Button's background picture
    SDL_Texture* text;
    //!< Button's optional foreground text, as a picture
  };


#endif