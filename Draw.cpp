// Samuel Weber

#include "Draw.h"


void drawBattle(unique_ptr<Party> &party, TTF_Font* font,
        vector<unique_ptr<Button> > &buttons, int x, int y, int cursor,
        vector<shared_ptr<Unit> > &enemies)
{
  SDL_RenderClear(party->getRen());
  SDL_Texture* c = loadTexture("ButtonCursor.png", party->getRen());
  renderTextbox(party->getRen(), font, "");
  for (unique_ptr<Button> &button : buttons)
  {
    button->render(party->getRen(), buttonUp);
    if (button->mouseOnButton(x, y))
      button->render(party->getRen(), buttonSelected);
    else
      button->render(party->getRen(), buttonUp);
  }
  renderTexture(c, party->getRen(), buttons[cursor]->getPos());
  drawBattleUpdate(party, font, enemies);
  SDL_RenderPresent(party->getRen());
  SDL_DestroyTexture(c);
}  // void drawBattle(SDL_Renderer *ren, TTF_Font* font)


void drawBattleUpdate(unique_ptr<Party> &party, TTF_Font* font,
        vector<shared_ptr<Unit> > &enemies)
{
  SDL_Texture* partyHP[4];
  SDL_Texture* black = loadTexture("Black.png", party->getRen());
  renderTexture(black, party->getRen(), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 4 / 5);
  string HPstr = to_string(party->getUnit(0)->getCurrHP()) + "/" + to_string(party->getUnit(0)->getMaxHP());
  string HPLabel = "Your HP:";
  SDL_Texture* partyHPLabel = renderText(party->getRen(), font, HPLabel);
  partyHP[0] = renderText(party->getRen(), font, HPstr);
  HPstr = to_string(enemies[0]->getCurrHP()) + "/" + to_string(enemies[0]->getMaxHP());
  HPLabel = "Enemy HP:";
  SDL_Texture* enemyHPLabel = renderText(party->getRen(), font, HPLabel);
  SDL_Texture* enemyHP = renderText(party->getRen(), font, HPstr);
  renderTexture(partyHPLabel, party->getRen(), 800, 70);
  renderTexture(partyHP[0], party->getRen(), 800, 100);
  renderTexture(enemyHPLabel, party->getRen(), 24, 70);
  renderTexture(enemyHP, party->getRen(), 24, 100);
  SDL_RenderPresent(party->getRen());
  SDL_DestroyTexture(partyHP[0]);
  SDL_DestroyTexture(enemyHP);
  SDL_DestroyTexture(black);
}  // void drawBattleUpdate


bool drawMap(unique_ptr<Party> &party)
{  // portion of map to be drawn based on position of hero
  SDL_RenderClear(party->getRen());
  int tileClip = 0;
  bool spriteMoved = false, cont = false;
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
        if (!spriteMoved && cont)
          spriteMoved = true;
      }
    }
  SDL_RenderPresent(party->getRen());
  return spriteMoved;
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
  SDL_WaitEvent(&e);
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


void drawTitle(SDL_Renderer *ren, vector<unique_ptr<Button> > &buttons,
        int x, int y, int cursor)
{
  SDL_RenderClear(ren);
  SDL_Texture* bg = loadTexture("Title.png", ren);
  SDL_Texture* c = loadTexture("ButtonCursor.png", ren);
  renderBackground(bg, ren);
  for (unique_ptr<Button> &button : buttons)
  {
    button->render(ren, buttonUp);
    if (button->mouseOnButton(x, y))
      button->render(ren, buttonSelected);
    else
      button->render(ren, buttonUp);
  }
  renderTexture(c, ren, buttons[cursor]->getPos());
  SDL_RenderPresent(ren);
  SDL_DestroyTexture(bg);
  SDL_DestroyTexture(c);
}  // void drawTitle(SDL_Renderer *ren)

