//Samuel Weber

#include "Draw.h"

void drawBattle(SDL_Renderer *ren, TTF_Font* font)
{
  SDL_RenderClear(ren);
  //draw graphics visuals (ignoring this step for now, except for...)
  //...including text box at bottom
  //choose and print next line of text (damage report, etc)
  string str = "I am an extended text message. I need to be long enough to necessitate covering multiple lines. I should also contain some relatively long words, just in case, as well as plenty of short ones. Really, anything necessary to extend the length of this as long as possible. Ideally, this message should be too long to fit on the screen. Hopefully this is enough to do so.";
  renderTextbox(ren, font, str);
  SDL_RenderPresent(ren);
}//void drawBattle(SDL_Renderer *ren, TTF_Font* font)

void drawMap(SDL_Renderer *ren, SDL_Texture *tiles, Terr *terr, Sprite *party)
{//portion of map to be drawn based on position of hero
  SDL_RenderClear(ren);
  int tileClip = 0;
  SDL_Rect tileClips[16]; //magic number (16): number of tile types. Currently five (black/impassable, white/passable, etc)
  getClips(tileClips, 16, 4, TILE_WIDTH, TILE_HEIGHT); //magic number (4): number of rows in the tile spritesheet

  //position of unit on which camera is focused
  int x = party->getPos()->getX();
  int y = party->getPos()->getY();

  Tile* tilePtr = NULL;
  Tile* offMap = new Tile();
  for (int i = 0; i < (NUM_TILES_WIDTH); i++)
    for (int j = 0; j < (NUM_TILES_HEIGHT); j++)
    {
      if (((x + i - 8) >= 0) && ((x + i - 8) < terr->getWidth()) && ((y + j - 6) >= 0) && ((y + j - 6) < terr->getHeight()))
        tilePtr = terr->getTile((x + i - 8), (j + y - 6));//if Tile in question exists
      else
        tilePtr = offMap;
      tileClip = tilePtr->getType(); //tile spritesheet MUST be kept in the same order as the tileType enum
      renderTexture(tiles, ren, TILE_WIDTH * i, TILE_HEIGHT * j, &tileClips[tileClip], tilePtr->getAngle());
      if (tilePtr->isOccupied())
        drawUnit(ren, tilePtr, i, j); //if the tile is occupied, draw the character
    }
  delete offMap;
  SDL_RenderPresent(ren);
}//void drawMap(SDL_Renderer *ren, SDL_Texture *tiles, Terr *terr, Sprite *party)

void drawRebind(SDL_Renderer *ren, TTF_Font* font)
{
  SDL_RenderClear(ren);
  string str = "Choose a new key for the interact button (previously ";
  str += SDL_GetKeyName(interact.sym);
  str += ")";
  renderTextbox(ren, font, str);
  SDL_RenderPresent(ren);
  SDL_Event e;
  bool cont = true;
  while (cont)
    if (SDL_PollEvent(&e))
      if (e.type == SDL_KEYDOWN)
      {
        if (e.key.keysym.sym != SDLK_LALT && e.key.keysym.sym != SDLK_RALT && e.key.keysym.sym != SDLK_LSHIFT && e.key.keysym.sym != SDLK_RSHIFT && e.key.keysym.sym != SDLK_LCTRL && e.key.keysym.sym != SDLK_RCTRL)
        {
          rebind(interact, e.key.keysym);
          cont = false;
        }
      }
  const char* newKey = SDL_GetKeyName(interact.sym);
  str.clear();
  str += "New key for interact button: ";
  str += newKey;
  SDL_RenderClear(ren);
  renderTextbox(ren, font, str);
  SDL_RenderPresent(ren);
  pressAnyKey();
  SDL_RenderPresent(ren);
}//void drawRebind(SDL_Renderer *ren)

void drawScreen(gameState &state, SDL_Renderer *ren, TTF_Font* font, SDL_Texture *tiles, Terr *terr, Sprite *party, Button *toGame)
{//when things change (any event happens): clear the renderer, refill it. Draw the background first, then the chars on top of it.
  SDL_RenderClear(ren);
  switch (state)
  {
  case BATTLE:
    drawBattle(ren, font);
    break;
  case MAP:
    drawMap(ren, tiles, terr, party);
    break;
  case REBIND:
    drawRebind(ren, font);
    break;
  case TITLE:
    drawTitle(ren, toGame);
    break;
  default:
    break;
  }
}//void drawScreen(gameState &state, SDL_Renderer *ren, TTF_Font* font, SDL_Texture *tiles, Terr *terr, Sprite *party, Button *toGame)

void drawTitle(SDL_Renderer *ren, Button *toGame)
{
  SDL_RenderClear(ren);
  SDL_Texture *bg = loadTexture("title.png", ren);
  renderTexture(bg, ren, 0, 0);
  toGame->render(ren);
  SDL_RenderPresent(ren);
}//void drawTitle(SDL_Renderer *ren)

void drawUnit(SDL_Renderer *ren, Tile* tilePtr, int i, int j)
{
  int sc = 0;
  spriteType spriteClip = tilePtr->getSprite()->getSprite();
  SDL_Rect spriteClips[4];
  getClips(spriteClips, 4, 2, TILE_WIDTH, TILE_HEIGHT); //magic number (4): number of unit sprite types
  switch (spriteClip)
  {//note the order -- clips are taken by column, not by row!
  case UP:
    sc = 0;
    break;
  case DOWN:
    sc = 1;
    break;
  case LEFT:
    sc = 2;
    break;
  case RIGHT:
    sc = 3;
    break;
  default:
    break;
  }//technically, this switch should be optional so long as the character spritesheet is kept in the same order as the enum
  renderTexture(tilePtr->getSprite()->getSpriteSheet(), ren, TILE_WIDTH * i, TILE_HEIGHT * j, &spriteClips[sc]);
}//void drawUnit(SDL_Renderer *ren, Tile* tilePtr, int i, int j)

void getClips(SDL_Rect* clips, int numClips, int rows, int cWidth, int cHeight)
{
  for (int i = 0; i < numClips; i++)
  {
    clips[i].x = i / rows * cWidth;
    clips[i].y = i % rows * cHeight;
    clips[i].w = cWidth;
    clips[i].h = cHeight;
  }//fill in clips from spritesheet
}//void getClips(SDL_Rect* clips, int numClips, int rows, int cWidth, int cHeight)

SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren)
{
  SDL_Texture *tex = IMG_LoadTexture(ren, file.c_str());
  if (tex == nullptr)
    quit("LoadTexture");
  return tex;
}//SDL_Texture* loadTexture(const string file, SDL_Renderer *ren)

SDL_Texture* renderText(SDL_Renderer *ren, TTF_Font *font, const string &str, SDL_Color color)
{
  SDL_Surface *surf = TTF_RenderText_Blended(font, str.c_str(), color);
  if (surf == nullptr)
    quit("TTF_RenderText");
  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
  if (tex == nullptr)
    quit("CreateTexture");
  SDL_FreeSurface(surf);
  return tex;
}//SDL_Texture* renderText(TTF_Font *font, const string &str, SDL_Color color, SDL_Renderer *ren)

void renderTextbox(SDL_Renderer *ren, TTF_Font *font, const string &str, SDL_Color color)
{
  SDL_Texture* textbox = loadTexture("textbox.png", ren);
  int lineH = TTF_FontLineSkip(font);
  int boxH;
  int lineNum = 0;
  string line, word;
  SDL_Texture* text;
  line.clear();
  word.clear();
  SDL_QueryTexture(textbox, NULL, NULL, NULL, &boxH);
  renderTexture(textbox, ren, 0, (SCREEN_HEIGHT - boxH));
  for (string::const_iterator itr = str.cbegin(); itr != str.cend(); itr++)
  {
    if (*itr == ' ')
    {
      if ((line.length() + word.length() + 1 <= 70)) //magic number: at current font size, 70 characters to a line
        line += ' ';//line with word added is within length allowed
      else
      {
        text = renderText(ren, font, line, color);
        renderTexture(text, ren, 10, (SCREEN_HEIGHT - boxH + lineNum * lineH + 10));
        line.clear();
        lineNum++;
      }//line is finished. print line and clear line.
      line += word;
      word.clear();
    }//if word completed
    else
      word += *itr;
  }//problems will occur if more lines of text are needed than will fit in one textbox

  if ((line.length() + word.length() + 1 <= 70)) //magic number: at current font size, 70 characters to a line
  {
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
}//void renderTextbox(SDL_Renderer *ren, TTF_Font *font, const string &str, SDL_Color color)

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip, const double angle, const SDL_RendererFlip flip)
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
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
  renderTexture(tex, ren, dst, clip, angle, flip);
}//void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr, const double angle = 0, const SDL_RendererFlip flip = SDL_FLIP_NONE)

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip, const double angle, const SDL_RendererFlip flip)
{
  SDL_RenderCopyEx(ren, tex, clip, &dst, angle, NULL, flip);
}//void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip, const double angle = 0, const SDL_RendererFlip flip = SDL_FLIP_NONE)

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, const double angle, const SDL_RendererFlip flip)
{
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopyEx(ren, tex, NULL, &dst, angle, NULL, flip);
}//void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, const double angle = 0, const SDL_RendererFlip flip = SDL_FLIP_NONE)

void tileBackground(SDL_Texture *tile, SDL_Renderer *ren)
{
  int xTiles = SCREEN_WIDTH / TILE_SIZE;
  int yTiles = SCREEN_HEIGHT / TILE_SIZE;
  for (int i = 0; i < xTiles * yTiles; i++)
  {
    int x = i % xTiles;
    int y = i / xTiles;
    renderTexture(tile, ren, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
  }
}//void tileBackground(SDL_Texture *tile, SDL_Renderer *ren)


