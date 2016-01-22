// Samuel Weber

#ifndef SPRITE_H
  #define SPRITE_H

  #include "Render.h"
  #include "Globals.h"


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
    Sprite(SDL_Renderer *ren, const string &spriteFile, const string &t = "");
    //!< All units MUST have a spritesheet specified. type string optional.
    ~Sprite();
    //!< Default destructor
    void changeDir(dir d);
    //!< Change what direction Sprite is facing. Usuallly part of movement.
    bool decSpline();
    //!< If spline is non-zero, decrements it and returns true. Else false.
    dir getFacing();
    //!< Returns which direction the Sprite is currently facing.
    int getSpline();
    //!< Returns the number of pixels left to spline.
    const string getType();
    //!< Returns the type descriptor string for this NPC.
    spriteType getSprite();
    //!< Returns the type of sprite (facing, etc) currently being used.
    SDL_Texture* getSpriteSheet();
    //!< Returns the spritesheet for this Sprite.
    void setSpline(int s);
    //!< Manually sets the number of pixels to spline.
    void setSprite(spriteType st);
    //!< Manually change sprite type (facing, etc).
    void setType(const string &str);
    //!< Set the descriptive string describing Sprite
  protected:
    SDL_Texture *spriteSheet;
    //!< The spritesheet used to draw this Sprite.
    spriteType sprite;
    //!< Which sprite on the spritesheet is currently being used.
    string type;
    //!< String describing the purpose of Sprite.
    dir facing;
    //!< Direction Sprite is currently facing.
    int spline;
    //!< Number of pixels left to be splined towards destination.
  };

#endif

