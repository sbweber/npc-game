// Samuel Weber

#include "Draw.h"


void drawBattle(SDL_Renderer *ren, TTF_Font* font)
{ // TODO: All battle functionality stubbed or otherwise incomplete
  SDL_RenderClear(ren);
  // draw graphics visuals (ignoring this step for now, except for...)
  // ...including text box at bottom
  // choose and print next line of text (damage report, etc)
  string str = "I am an extended text message. I need to be long enough to"
          "necessitate covering multiple lines. I should also contain some"
          "relatively long words, just in case, as well as plenty of short"
          "ones. Really, anything necessary to extend the length of this as"
          "long as possible. Ideally, this message should be too long to fit on"
          "the screen. Hopefully this is enough to do so.";
  renderTextbox(ren, font, str);
  SDL_RenderPresent(ren);
}  // void drawBattle(SDL_Renderer *ren, TTF_Font* font)


void drawMap(SDL_Renderer *ren, SDL_Texture *tiles, Party *party)
{  // portion of map to be drawn based on position of hero
  SDL_RenderClear(ren);
  int tileClip = 0;
  bool contAny = false, cont = false;
  SDL_Rect tileClips[16];  // magic number (16): number of tile types. Currently five (black/impassable, white/passable, etc)
  getClips(tileClips, 16, 4, TILE_WIDTH, TILE_HEIGHT);  // magic number (4): number of rows in the tile spritesheet

  // position of unit on which camera is focused
  /* TODO: getPos
  int x = party->getSprite()->getPos()->getX();
  int y = party->getSprite()->getPos()->getY();
  */
  int x = 0, y = 0;

  Tile* tilePtr = nullptr;
  Tile* offMap = new Tile();
  for (int i = 0; i < (NUM_TILES_WIDTH); i++)
    for (int j = 0; j < (NUM_TILES_HEIGHT); j++)
    {
      if (((x + i - 8) >= 0) && ((x + i - 8) < party->getTerr()->getWidth()) &&
              ((y + j - 6) >= 0) && ((y + j - 6) < party->getTerr()->getHeight()))
        tilePtr = party->getTerr()->getTile((x + i - 8), (j + y - 6));
      // if Tile in question exists
      else
        tilePtr = offMap;
      tileClip = tilePtr->getType();
      // tile spritesheet MUST be kept in the same order as the tileType enum
      switch (party->getSprite()->getFacing())
      {
      case EAST:
        renderTexture(tiles, ren,
                TILE_WIDTH * i + party->getSprite()->getSpline(),
                TILE_HEIGHT * j, &tileClips[tileClip], tilePtr->getAngle());
        break;
      case NORTH:
        renderTexture(tiles, ren, TILE_WIDTH * i,
               TILE_HEIGHT * j - party->getSprite()->getSpline(),
                &tileClips[tileClip], tilePtr->getAngle());
        break;
      case SOUTH:
        renderTexture(tiles, ren, TILE_WIDTH * i,
                TILE_HEIGHT * j + party->getSprite()->getSpline(),
                &tileClips[tileClip], tilePtr->getAngle());
        break;
      case WEST:
        renderTexture(tiles, ren,
                TILE_WIDTH * i - party->getSprite()->getSpline(),
                TILE_HEIGHT * j, &tileClips[tileClip], tilePtr->getAngle());
        break;
      default:  // Should be impossible to get here
        break;
      }
    }
    /* TODO: For all Sprites onscreen on the Terr, draw them! Accomplish by checking all the tiles onscreen and, if they have a paired Sprite, draw it there!
  for (int i = 0; i < (NUM_TILES_WIDTH); i++)
    for (int j = 0; j < (NUM_TILES_HEIGHT); j++)
    {
      if (((x + i - 8) >= 0) && ((x + i - 8) < party->getTerr()->getWidth()) &&
        ((y + j - 6) >= 0) && ((y + j - 6) < party->getTerr()->getHeight()))
        tilePtr = party->getTerr()->getTile((x + i - 8), (j + y - 6));
      // if Tile in question exists
      else
        tilePtr = offMap;
      if (tilePtr->isOccupied())
        cont = drawUnit(ren, tilePtr, party, i, j);
      // if the tile is occupied, draw the character
      if (!contAny && cont)
        contAny = true;
    }
    */
  if (contAny)
  {
    SDL_Event* wait = new SDL_Event();
    SDL_PushEvent(wait);
  }
  delete offMap;
  SDL_RenderPresent(ren);
}  // void drawMap()


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
        if ((e.key.keysym.sym != SDLK_LALT) &&
                (e.key.keysym.sym != SDLK_RALT) &&
                (e.key.keysym.sym != SDLK_LSHIFT) &&
                (e.key.keysym.sym != SDLK_RSHIFT) &&
                (e.key.keysym.sym != SDLK_LCTRL) &&
                (e.key.keysym.sym != SDLK_RCTRL))
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
}  // void drawRebind(SDL_Renderer *ren)


void drawScreen(gameState &state, SDL_Renderer *ren, TTF_Font* font,
        SDL_Texture *tiles, Party *party, Button *toGame)
{
  // when things change (any event happens): clear the renderer, refill it.
  // Draw the background first, then the chars on top of it.
  SDL_RenderClear(ren);
  switch (state)
  {
  case BATTLE:
    drawBattle(ren, font);
    break;
  case MAP:
    drawMap(ren, tiles, party);
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
}  // void drawScreen()


void drawTitle(SDL_Renderer *ren, Button *toGame)
{
  SDL_RenderClear(ren);
  SDL_Texture *bg = loadTexture("Title.png", ren);
  renderTexture(bg, ren, 0, 0);
  toGame->render(ren);
  SDL_RenderPresent(ren);
}  // void drawTitle(SDL_Renderer *ren)


bool drawUnit(SDL_Renderer *ren, Sprite* sprite, Party *party, int i, int j)
{
  int sc = 0, vSpline = 0, hSpline = 0;
  SDL_Rect spriteClips[4];
  getClips(spriteClips, 4, 2, TILE_WIDTH, TILE_HEIGHT);
  // magic number (4): number of unit sprite types
  switch (sprite->getSprite())
  {  // note the order -- clips are taken by column, not by row!
  case UP:
    sc = 0;
    vSpline += sprite->getSpline();
    break;
  case DOWN:
    sc = 1;
    vSpline -= sprite->getSpline();
    break;
  case LEFT:
    sc = 2;
    hSpline += sprite->getSpline();
    break;
  case RIGHT:
    sc = 3;
    hSpline -= sprite->getSpline();
    break;
  default:  // Reaching here should be impossible.
    break;
  }
  switch (party->getSprite()->getFacing())
  {
  case EAST:
    hSpline += party->getSprite()->getSpline();
    break;
  case NORTH:
    vSpline -= party->getSprite()->getSpline();
    break;
  case SOUTH:
    vSpline += party->getSprite()->getSpline();
    break;
  case WEST:
    hSpline -= party->getSprite()->getSpline();
    break;
  default:  // Should be impossible to be here.
    break;
  }
  // technically, this switch should be optional so long as the character
  // spritesheet is kept in the same order as the enum
  renderTexture(sprite->getSpriteSheet(), ren, TILE_WIDTH * i + hSpline, TILE_HEIGHT * j + vSpline, &spriteClips[sc]);

  return sprite->decSpline();
}//void drawUnit(SDL_Renderer *ren, Sprite* sprite, int i, int j)

