// Samuel Weber

#ifndef BUTTON_H
  #define BUTTON_H

  #include "Render.h"
  #include "Globals.h"

  enum buttonState
  {
    buttonUp,
    buttonClicked,
    buttonSelected,
  };


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
    bool buttonClick(SDL_Renderer *ren, SDL_MouseButtonEvent &click);
    //!< Returns true if Button was clicked
    bool mouseOnButton(int x, int y);
    //!< Returns true if mouse is over the button (x/y coords).
    bool mouseOnButton(SDL_MouseButtonEvent &mouse);
    //!< Returns true if mouse is over the button (click).
    bool mouseOnButton(SDL_MouseMotionEvent &mouse);
    //!< Returns true if mouse is over the button (mouseover).
    void render(SDL_Renderer *ren, buttonState state);
    //!< Draws Button on the renderer
  protected:
    SDL_Rect button;
    //!< Rectangle describing Button's position and dimensions
    SDL_Texture* pic;
    //!< Button's background picture
    SDL_Texture* text;
    //!< Button's optional foreground text, as a texture
  };


#endif