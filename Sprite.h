//Samuel Weber

#ifndef SPRITE_H
  #define SPRITE_H

  #include "Draw.h"
  #include "Tile.h"
  #include "Globals.h"

  class Tile;

  enum spriteType
  {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };//spritesheets for characters always arranged in the same manner, although some may end sooner than others

  class Sprite
  {
    SDL_Texture *spriteSheet;
    spriteType sprite;
    Tile* pos; //pointer to the tile currently containing this Unit
    string type;
    dir facing; //direction the unit/sprite is currently facing
  public:
    Sprite(SDL_Renderer *ren, const string &spriteFile); //all units MUST have a spritesheet specified
    ~Sprite();
    Tile* getPos();
    spriteType getSprite();
    SDL_Texture* getSpriteSheet();
    void interact();
    void move(dir d);
    void setSprite(spriteType st);
    void setTile(Tile* tile);
    void setType(const string &str);
  };

#endif

