// Samuel Weber

#ifndef RENDER_H
  #define RENDER_H

  #include "Globals.h"

  // Render.cpp
  void getClips(SDL_Rect* clips, int numClips, int rows, int cWidth,
          int cHeight);
  //!< Fills in sprite clips from spritesheet.
  SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren);
  //!< Loads a texture
  SDL_Texture* renderText(SDL_Renderer *ren, TTF_Font *font, const string &str,
          SDL_Color color = {255, 255, 255, 255});
  //!< Renders text into a picture. Text is white by default.
  void renderTextbox(SDL_Renderer *ren, TTF_Font *font, const string &str,
          SDL_Color color = {255, 255, 255, 255});
  //!< Draws a textbox with text on the screen. Text is white by default.
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
          SDL_Rect *clip = nullptr, const double angle = 0,
          const SDL_RendererFlip flip = SDL_FLIP_NONE);
  //!< Renders a texture to the screen.
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
          SDL_Rect *clip = nullptr, const double angle = 0,
          const SDL_RendererFlip flip = SDL_FLIP_NONE);
  //!< Renders a texture to the screen.
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w,
          int h, const double angle = 0,
          const SDL_RendererFlip flip = SDL_FLIP_NONE);
  //!< Renders a texture to the screen.
  void tileBackground(SDL_Texture *tile, SDL_Renderer *ren);
  //!< Given a texture, regularly tiles it (as a square tile) over the screen.

#endif /* RENDER_H */
