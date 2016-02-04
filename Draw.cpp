// Samuel Weber

#include "Draw.h"


void drawBattle(SDL_Renderer *ren, TTF_Font* font)
{ // TODO: All battle functionality stubbed or otherwise incomplete
  SDL_RenderClear(ren);
  // draw graphics visuals (ignoring this step for now, except for...)
  // ...including text box at bottom
  // choose and print next line of text (damage report, etc)
  renderTextbox(ren, font, "");
  SDL_RenderPresent(ren);
}  // void drawBattle(SDL_Renderer *ren, TTF_Font* font)


bool drawMap(unique_ptr<Party> &party)
{  // portion of map to be drawn based on position of hero
  SDL_RenderClear(party->getRen());
  int tileClip = 0;
  bool partyMoved = false, cont = false;
  SDL_Rect tileClips[16];  // magic number (16): number of tile types. Currently five (black/impassable, white/passable, etc)
  getClips(tileClips, 16, 4, TILE_WIDTH, TILE_HEIGHT);  // magic number (4): number of rows in the tile spritesheet

  // position of unit on which camera is focused
  int x = party->getTerr()->getTile(party->getSprite())->getX();
  int y = party->getTerr()->getTile(party->getSprite())->getY();
  int hsw = NUM_TILES_WIDTH / 2;  //half screen width (in tiles)
  int hsh = NUM_TILES_HEIGHT / 2;  // half screen height

  shared_ptr<Tile> tilePtr = nullptr;
  for (int i = 0; i < (NUM_TILES_WIDTH); i++)
    for (int j = 0; j < (NUM_TILES_HEIGHT); j++)
    {
      if (((x + i - hsw) >= 0) && ((x + i - hsw) < party->getTerr()->getWidth()) &&
              ((y + j - hsh) >= 0) && ((y + j - hsh) < party->getTerr()->getHeight()))
        tilePtr = party->getTerr()->getTile((x + i - hsw), (j + y - hsh));
      // if Tile in question exists
      else
        tilePtr = nullptr;
      if (tilePtr)
      {
        tileClip = tilePtr->getType();
        // tile spritesheet MUST be kept in the same order as the tileType enum
        switch (party->getSprite()->getFacing())
        {
        case EAST:
          renderTexture(tilePtr->getTex(), party->getRen(),
                  TILE_WIDTH * i + party->getSprite()->getSpline(),
                  TILE_HEIGHT * j, &tileClips[tileClip], tilePtr->getAngle());
          break;
        case NORTH:
          renderTexture(tilePtr->getTex(), party->getRen(),
                  TILE_WIDTH * i,
                  TILE_HEIGHT * j - party->getSprite()->getSpline(),
                  &tileClips[tileClip], tilePtr->getAngle());
          break;
        case SOUTH:
          renderTexture(tilePtr->getTex(), party->getRen(),
                  TILE_WIDTH * i,
                  TILE_HEIGHT * j + party->getSprite()->getSpline(),
                  &tileClips[tileClip], tilePtr->getAngle());
          break;
        case WEST:
          renderTexture(tilePtr->getTex(), party->getRen(),
                  TILE_WIDTH * i - party->getSprite()->getSpline(),
                  TILE_HEIGHT * j, &tileClips[tileClip], tilePtr->getAngle());
          break;
        default:  // Should be impossible to get here
          break;
        }
      }
    }
  // For all Sprites onscreen on the Terr, draw them! Accomplish by checking
  // all the tiles onscreen and, if they have a paired Sprite, draw it there.
  // Done as a second layer, after all the tiles have been drawn so no tiles
  // are drawn on top of Sprites.
  for (int i = 0; i < (NUM_TILES_WIDTH); i++)
    for (int j = 0; j < (NUM_TILES_HEIGHT); j++)
    {
      if (((x + i - hsw) >= 0) && ((x + i - hsw) < party->getTerr()->getWidth()) &&
        ((y + j - hsh) >= 0) && ((y + j - hsh) < party->getTerr()->getHeight()))
        tilePtr = party->getTerr()->getTile((x + i - hsw), (j + y - hsh));
      // if Tile in question exists
      else
        tilePtr = nullptr;
      if (tilePtr)
      {
        if (party->getTerr()->isOccupied(tilePtr))
          cont = drawSprite(tilePtr, party, i, j);
        // if the tile is occupied, draw the character
        if (!partyMoved && cont &&
                party->getSprite() == party->getTerr()->getSprite(tilePtr))
          partyMoved = true;
      }
    }
  SDL_RenderPresent(party->getRen());
  return partyMoved;
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


bool drawSprite(shared_ptr<Tile> tile, unique_ptr<Party> &party, int i, int j)
{
  int sc = 0, vSpline = 0, hSpline = 0;
  SDL_Rect spriteClips[4];
  getClips(spriteClips, 4, 2, TILE_WIDTH, TILE_HEIGHT);
  shared_ptr<Sprite> sprite = party->getTerr()->getSprite(tile);
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
  renderTexture(sprite->getSpriteSheet(), party->getRen(),
          TILE_WIDTH * i + hSpline, TILE_HEIGHT * j + vSpline,
          &spriteClips[sc]);
  return sprite->decSpline();
}//void drawSprite(SDL_Renderer *ren, shared_ptr<Tile> tile, int i, int j)


void drawTitle(SDL_Renderer *ren, vector<unique_ptr<Button> > &buttons)
{
  SDL_RenderClear(ren);
  SDL_Texture* bg = loadTexture("Title.png", ren);
  renderBackground(bg, ren);
  for (unique_ptr<Button> &button : buttons)
    button->render(ren);
  SDL_RenderPresent(ren);
}  // void drawTitle(SDL_Renderer *ren)

