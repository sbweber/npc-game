// Samuel Weber

#include "Render.h"


void getClips(SDL_Rect* clips, int numClips, int rows, int cWidth, int cHeight)
{
  for (int i = 0; i < numClips; i++)
  {
    clips[i].x = i / rows * cWidth;
    clips[i].y = i % rows * cHeight;
    clips[i].w = cWidth;
    clips[i].h = cHeight;
  }  // fill in clips from spritesheet
}  // void getClips()


SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren)
{
  SDL_Texture* tex(IMG_LoadTexture(ren, ("resources/"+file).c_str()));
  if (!tex)
    quit("LoadTexture");
  return tex;
}  // SDL_Texture* loadTexture(const string file, SDL_Renderer *ren)


void renderBackground(SDL_Texture* bg, SDL_Renderer *ren)
{
  SDL_RenderCopy(ren, bg, nullptr, nullptr);
}  // void renderBackground(SDL_Texture *tex, SDL_Renderer *ren)


SDL_Texture* renderText(SDL_Renderer *ren, TTF_Font *font,
        const string &str, SDL_Color color)
{
  SDL_Surface *surf = TTF_RenderText_Blended(font, str.c_str(), color);
  if (surf == nullptr)
    quit("TTF_RenderText");
  SDL_Texture* tex(SDL_CreateTextureFromSurface(ren, surf));
  if (!tex)
    quit("CreateTexture");
  SDL_FreeSurface(surf);
  return tex;
}  // SDL_Texture* renderText()


void renderTextbox(SDL_Renderer *ren, TTF_Font *font, const string &str,
        SDL_Color color)
{
  SDL_Texture* textbox = loadTexture("textbox.png", ren);
  int lineH = TTF_FontLineSkip(font);
  int boxH;
  int lineNum = 0;
  string line, word;
  SDL_Texture* text;
  line.clear();
  word.clear();
  SDL_QueryTexture(textbox, nullptr, nullptr, nullptr, &boxH);
  renderTexture(textbox, ren, 0, (SCREEN_HEIGHT - boxH));
  for (const char c : str)
  {
    if (c == ' ')
    {
      if ((line.length() + word.length() + 1 <= 70))
        // magic number: at current font size, 70 characters to a line
        line += ' ';  // line with word added is within length allowed
      else
      {
        text = renderText(ren, font, line, color);
        renderTexture(text, ren, 10, (SCREEN_HEIGHT - boxH + lineNum *
                lineH + 10));
        line.clear();
        lineNum++;
      }  // line is finished. print line and clear line.
      line += word;
      word.clear();
    }  // if word completed
    else
      word += c;
  }
  // problems will occur if more lines of text are needed than
  // will fit in one textbox

  if ((line.length() + word.length() + 1 <= 70))
  {  // magic number: at current font size, 70 characters to a line
    line += ' ';
    line += word;
    word.clear();
  }
  text = renderText(ren, font, line, color);
  renderTexture(text, ren, 10, (SCREEN_HEIGHT - boxH + lineNum * lineH + 10));
  line.clear();
  lineNum++;
  if (!word.empty())
  {
    text = renderText(ren, font, line, color);
    renderTexture(text, ren, 10, (SCREEN_HEIGHT - boxH + lineNum * lineH + 10));
  }

  SDL_DestroyTexture(textbox);
}  // void renderTextbox()


void renderTexture(SDL_Texture* tex, SDL_Renderer *ren, int x,
        int y, SDL_Rect *clip, const double angle, const SDL_RendererFlip flip)
{
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  if (clip != nullptr)
  {
    dst.w = clip->w;
    dst.h = clip->h;
  }
  else
    SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
  renderTexture(tex, ren, dst, clip, angle, flip);
}  // void renderTexture()


void renderTexture(SDL_Texture* tex, SDL_Renderer *ren, int x,
        int y, int w, int h, const double angle, const SDL_RendererFlip flip)
{
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopyEx(ren, tex, nullptr, &dst, angle, nullptr, flip);
}  // void renderTexture()


void renderTexture(SDL_Texture* tex, SDL_Renderer *ren,
        SDL_Rect dst, SDL_Rect *clip, const double angle,
        const SDL_RendererFlip flip)
{
  SDL_RenderCopyEx(ren, tex, clip, &dst, angle, nullptr, flip);
}  // void renderTexture()


void tileBackground(SDL_Texture* tile, SDL_Renderer *ren)
{
  for (int i = 0; i < (NUM_TILES_WIDTH * NUM_TILES_HEIGHT); i++)
  {
    int x = i % NUM_TILES_WIDTH;
    int y = i / NUM_TILES_WIDTH;
    renderTexture(tile, ren, x * TILE_WIDTH, y * TILE_HEIGHT,
            TILE_WIDTH, TILE_HEIGHT);
  }
}  // void tileBackground(SDL_Texture *tile, SDL_Renderer *ren)

