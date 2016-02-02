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
  SDL_Texture* tex(IMG_LoadTexture(ren, ("resources/tex/"+file).c_str()));
  if (!tex)
    quit("LoadTexture");
  return tex;
}  // SDL_Texture* loadTexture(const string file, SDL_Renderer *ren)


void renderBackground(SDL_Texture* bg, SDL_Renderer *ren)
{
  SDL_RenderCopy(ren, bg, nullptr, nullptr);
}  // void renderBackground(SDL_Texture *tex, SDL_Renderer *ren)


void renderSpeech(SDL_Renderer *ren, TTF_Font *font, const string &name,
        const string &str, SDL_Color color)
{
  string temp = str;
  while (!temp.empty())
  {
    renderTextbox(ren, font, "");
    if (!name.empty())
      temp = renderTextbox(ren, font, name + ": " + temp, color);
    else
      temp = renderTextbox(ren, font, temp, color);
    SDL_RenderPresent(ren);
    pressAnyKey();
  }
}  // void renderSpeech()


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


string renderTextbox(SDL_Renderer *ren, TTF_Font *font, const string &str,
        SDL_Color color)
{
  SDL_Texture* textbox = loadTexture("textbox.png", ren);
  int lineH = TTF_FontLineSkip(font);
  int boxH = SCREEN_HEIGHT / 5;  // magic number: textbox 1/5 of screen tall.
  unsigned int charsInLine = (SCREEN_WIDTH - 20) / 9;
    // left and right margins of 10, 9 pixels per character.
  int lineNum = 0;
  string line, word, excess;
  SDL_Texture* text;
  line.clear();
  word.clear();
  renderTexture(textbox, ren, 0, (SCREEN_HEIGHT - boxH), SCREEN_WIDTH, boxH);
  for (const char c : str)
  {
    if (!excess.empty())
      excess += c;
    else if (c == ' ')
    {
      if ((line.length() + word.length()) < charsInLine)
        line += ' ';  // line with word added is within length allowed
      else
      {
        while (!line.empty() && line[0] == ' ')
          line.erase(line.begin());
        text = renderText(ren, font, line, color);
        renderTexture(text, ren, 10, (SCREEN_HEIGHT - boxH + lineNum *
                lineH + 10));  // magic number 10 for top margin of text box.
        line.clear();
        lineNum++;
        if (boxH < (lineNum * lineH + 20))  // margin of 10 at top and bottom.
          excess += word += ' ';
      }  // line is finished. print line and clear line.
      line += word;
      word.clear();
    }  // if word completed
    else
      word += c;
  }
  if (excess.empty())
  {
    if ((line.length() + word.length()) < charsInLine)
    {
      line += ' ';  // line with word added is within length allowed
      line += word;
      text = renderText(ren, font, line, color);
      renderTexture(text, ren, 10, (SCREEN_HEIGHT - boxH + lineNum *
              lineH + 10));  // magic number 10 for top margin of text box.
    }
    else
    {
      text = renderText(ren, font, line, color);
      renderTexture(text, ren, 10, (SCREEN_HEIGHT - boxH + lineNum *
        lineH + 10));  // magic number 10 for top margin of text box.
      line.clear();
      lineNum++;
      if (boxH < (lineNum * lineH + 20))  // margin of 10 at top and bottom.
        excess += word;
      else
      {
        text = renderText(ren, font, word, color);
        renderTexture(text, ren, 10, (SCREEN_HEIGHT - boxH + lineNum *
                lineH + 10));  // magic number 10 for top margin of text box.
      }
    }
  }

  SDL_DestroyTexture(textbox);
  return excess;
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

