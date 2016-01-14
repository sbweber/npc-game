// Samuel Weber

#ifndef SPRITE_H
  #define SPRITE_H

  #include "Render.h"
  #include "Tile.h"
  #include "Globals.h"

  class Tile;

  //! Spritesheets for characters always arranged in the same manner,
  //! although some may end sooner than others.
  enum spriteType
  {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

  //! Visual half of a Unit. Information on what to draw to put a sprite onscreen.
  class Sprite
  {
  public:
    Sprite(SDL_Renderer *ren, const string &spriteFile);
    //!< All units MUST have a spritesheet specified
    ~Sprite();
    //!< Default destructor
    Tile* getPos();
    //!< Returns a pointer to the Tile containing this Sprite. nullptr if not on a Tile.
    spriteType getSprite();
    //!< Returns the type of sprite (facing, etc) currently being used.
    SDL_Texture* getSpriteSheet();
    //!< Returns the spritesheet for this Sprite.
    void interact();
    //!< Intelligently interacts with Sprite being faced, if any.
    void move(dir d);
    //!< Move Sprite to a different Tile, and change facing accordingly.
    void setSprite(spriteType st);
    //!< Manually change sprite type (facing, etc).
    void setTile(Tile* tile);
    //!< Set the Tile on which Sprite is located.
    void setType(const string &str);
    //!< Set the descriptive string describing Sprite
  protected:
    SDL_Texture *spriteSheet;
    //!< The spritesheet used to draw this Sprite.
    spriteType sprite;
    //!< Which sprite on the spritesheet is currently being used.
    Tile* pos;
    //!< Pointer to the tile currently containing Sprite.
    string type;
    //!< String describing the purpose of Sprite.
    dir facing;
    //!< Direction Sprite is currently facing.
  };

#endif

