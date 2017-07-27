// Samuel Weber

#include "Draw.h"


void drawBattle(SDL_Renderer *ren, unique_ptr<Party> &party, TTF_Font* font,
        vector<unique_ptr<Button> > &buttons, int x, int y, int cursor,
        vector<shared_ptr<Unit> > &enemies)
{
  SDL_RenderClear(ren);
  SDL_Texture* c = loadTexture("ButtonCursor.png", ren);
  renderTextbox(ren, font, "");
  for (unique_ptr<Button> &button : buttons)
  {
    button->render(ren, buttonUp);
    if (button->mouseOnButton(x, y))
      button->render(ren, buttonSelected);
    else
      button->render(ren, buttonUp);
  }
  renderTexture(c, ren, buttons[cursor]->getPos());
  drawBattleUpdate(ren, party, font, enemies);
  SDL_RenderPresent(ren);
  SDL_DestroyTexture(c);
}  // void drawBattle(SDL_Renderer *ren, TTF_Font* font)


void drawBattleAttackText(SDL_Renderer *ren, TTF_Font *font, Attack attack,
        bool playerIsAttacking)
{
  string str;
  switch (attack.getAcc())
  {
  case -1:  // Weak hit
    str = "A weak hit... ";
    break;
  case 1:  // Crit
    str = "A critical hit! ";
    break;
  default:
    break;  // should be impossible to reach here
  }
  if (playerIsAttacking)
    str = str + "You attacked the enemy for " + to_string(attack.getDamage()) + " damage!";
  else
    str = str + "You were attacked for " + to_string(attack.getDamage()) + " damage!";
  renderTextbox(ren, font, str);
  SDL_RenderPresent(ren);
  pressAnyKey();
}  // void drawBattleAttackText(SDL_Renderer *ren, Attack attack)


void drawBattleUpdate(SDL_Renderer *ren, unique_ptr<Party> &party,
        TTF_Font* font, vector<shared_ptr<Unit> > &enemies)
{
  SDL_Texture* partyHP[4];
  SDL_Texture* black = loadTexture("Black.png", ren);
  renderTexture(black, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 4 / 5);
  string HPstr = to_string(party->getUnit(0)->getCurrHP()) + "/" + to_string(party->getUnit(0)->getMaxHP());
  string HPLabel = "Your HP:";
  SDL_Texture* partyHPLabel = renderText(ren, font, HPLabel);
  partyHP[0] = renderText(ren, font, HPstr);
  HPstr = to_string(enemies[0]->getCurrHP()) + "/" + to_string(enemies[0]->getMaxHP());
  HPLabel = "Enemy HP:";
  SDL_Texture* enemyHPLabel = renderText(ren, font, HPLabel);
  SDL_Texture* enemyHP = renderText(ren, font, HPstr);
  renderTexture(partyHPLabel, ren, 800, 70);
  renderTexture(partyHP[0], ren, 800, 100);
  renderTexture(enemyHPLabel, ren, 24, 70);
  renderTexture(enemyHP, ren, 24, 100);
  SDL_RenderPresent(ren);
  SDL_DestroyTexture(partyHP[0]);
  SDL_DestroyTexture(enemyHP);
  SDL_DestroyTexture(black);
  SDL_DestroyTexture(partyHPLabel);
  SDL_DestroyTexture(enemyHPLabel);
}  // void drawBattleUpdate


void drawMap(unique_ptr<Terrain> &terr, unique_ptr<Party> &party)
{  // portion of map to be drawn based on position of hero
  SDL_RenderClear(terr->getRen());
  int tileClip = 0;
  SDL_Rect tileClips[16];  // magic number (16): number of tile types. Currently five (black/impassable, white/passable, etc)
  getClips(tileClips, 16, 4, TILE_WIDTH, TILE_HEIGHT);  // magic number (4): number of rows in the tile spritesheet

  // position of unit on which camera is focused
  int x = terr->getTile(party->getSprite())->getX();
  int y = terr->getTile(party->getSprite())->getY();
  int hsw = NUM_TILES_WIDTH / 2;  //half screen width (in tiles)
  int hsh = NUM_TILES_HEIGHT / 2;  // half screen height

  shared_ptr<Tile> tilePtr = nullptr;
  for (int i = 0; i < (NUM_TILES_WIDTH); i++)
    for (int j = 0; j < (NUM_TILES_HEIGHT); j++)
    {
      if (((x + i - hsw) >= 0) && ((x + i - hsw) < terr->getWidth()) &&
              ((y + j - hsh) >= 0) && ((y + j - hsh) < terr->getHeight()))
        tilePtr = terr->getTile((x + i - hsw), (j + y - hsh));
      // if Tile in question exists
      else
        tilePtr = nullptr;
      if (tilePtr)
      {
        tileClip = tilePtr->getType();
        // tile spritesheet MUST be kept in the same order as the tileType enum
        switch (party->getSprite()->getFacing())
        {
        case DIR_EAST:
          renderTexture(tilePtr->getTex(), terr->getRen(),
                  TILE_WIDTH * i + party->getSprite()->getSpline(),
                  TILE_HEIGHT * j, &tileClips[tileClip], tilePtr->getAngle());
          break;
        case DIR_NORTH:
          renderTexture(tilePtr->getTex(), terr->getRen(),
                  TILE_WIDTH * i,
                  TILE_HEIGHT * j - party->getSprite()->getSpline(),
                  &tileClips[tileClip], tilePtr->getAngle());
          break;
        case DIR_SOUTH:
          renderTexture(tilePtr->getTex(), terr->getRen(),
                  TILE_WIDTH * i,
                  TILE_HEIGHT * j + party->getSprite()->getSpline(),
                  &tileClips[tileClip], tilePtr->getAngle());
          break;
        case DIR_WEST:
          renderTexture(tilePtr->getTex(), terr->getRen(),
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
    {  // if Tile is on screen
      if (((x + i - hsw) >= 0) && ((x + i - hsw) < terr->getWidth()) &&
              ((y + j - hsh) >= 0) && ((y + j - hsh) < terr->getHeight()))
        tilePtr = terr->getTile((x + i - hsw), (j + y - hsh));
      else
        tilePtr = nullptr;
      if (tilePtr && terr->isOccupied(tilePtr))  // Draw Sprite, if any
        drawSprite(terr->getRen(), terr->getSprite(tilePtr),
                party->getSprite(), i, j);
    }
  SDL_RenderPresent(terr->getRen());
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


void drawSprite(SDL_Renderer *ren, shared_ptr<Sprite> sprite,
        shared_ptr<Sprite> partySprite, int i, int j)
{
  int vSpline = 0, hSpline = 0;
  SDL_Rect spriteClips[8];
  getClips(spriteClips, 8, 4, TILE_WIDTH, TILE_HEIGHT);
  // magic number (8): number of unit sprite types
  // magic number (4): number of rows on a unit's spritesheet
  switch (sprite->getSprite())
  {  // note the order -- clips are taken by column, not by row!
  case SPRITE_UP:
  case SPRITE_WALK_UP:
    vSpline += sprite->getSpline();
    break;
  case SPRITE_DOWN:
  case SPRITE_WALK_DOWN:
    vSpline -= sprite->getSpline();
    break;
  case SPRITE_LEFT:
  case SPRITE_WALK_LEFT:
    hSpline += sprite->getSpline();
    break;
  case SPRITE_RIGHT:
  case SPRITE_WALK_RIGHT:
    hSpline -= sprite->getSpline();
    break;
  default:  // Reaching here should be impossible.
    break;
  }
  switch (partySprite->getFacing())
  {
  case DIR_EAST:
    hSpline += partySprite->getSpline();
    break;
  case DIR_NORTH:
    vSpline -= partySprite->getSpline();
    break;
  case DIR_SOUTH:
    vSpline += partySprite->getSpline();
    break;
  case DIR_WEST:
    hSpline -= partySprite->getSpline();
    break;
  default:  // Should be impossible to be here.
    break;
  }
  // technically, this switch should be optional so long as the character
  // spritesheet is kept in the same order as the enum
  renderTexture(sprite->getSpriteSheet(), ren,
          TILE_WIDTH * i + hSpline, TILE_HEIGHT * j + vSpline,
          &spriteClips[sprite->getSprite()]);
  sprite->walk();
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

