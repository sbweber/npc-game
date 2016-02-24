// Samuel Weber

#ifndef SPRITE_H
  #define SPRITE_H

  #include "Globals.h"
  #include "Keybind.h"
  #include "Render.h"


  //! Spritesheets for characters always arranged in the same manner,
  //! although some may end sooner than others.
  enum spriteType
  {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };


  //! Action type. Movement, interaction, etc.
  enum actType
  {
    MOVE,
    INTERACT,
    BAD_ACTION
  };


  //! Action direction and type.
  typedef tuple<dir, actType> action;


  //! Visual half of a Unit. Information on what to draw to put a sprite onscreen.
  class Sprite
  {
  public:
    Sprite(SDL_Renderer *ren, int min, int max, const string &spriteFile,
            int initTicks, const string &n = "", const string &p = "",
            const string &scriptFile = "");
    //!< All units MUST have a spritesheet specified. type string optional.
    ~Sprite();
    //!< Default destructor
    void changeDir(dir d);
    //!< Change what direction Sprite is facing. Usuallly part of movement.
    void clearActs();
    //!< Removes all elements from the action queue.
    bool decSpline();
    //!< If spline is non-zero, decrements it and returns true. Else false.
    void decTicks(mt19937_64& rng);
    //!< Decrement ticks. Then, if it's zero, return true and reset it.
    dir getFacing();
    //!< Returns which direction the Sprite is currently facing.
    int getQSize();
    //!< Returns size of actionQ.
    int getSpline();
    //!< Returns the number of pixels left to spline.
    const string getPurpose();
    //!< Returns the purpose string for this NPC.
    spriteType getSprite();
    //!< Returns the type of sprite (facing, etc) currently being used.
    SDL_Texture* getSpriteSheet();
    //!< Returns the spritesheet for this Sprite.
    const action popAct();
    //!< Returns the next action in the queue and pops it off the queue.
    void pushAct(action act);
    //!< Pushes an action into the action queue.
    void renderSpeech(SDL_Renderer *ren, const string &str,
            SDL_Color color = { 255, 255, 255, 255 });
    //!< Draws a series of textboxes with text until str has been displayed.
    void say(SDL_Renderer *ren);
    //!< Make the Sprite output text to the screen in its font.
    void setMoveFreq(int min, int max);
    //!< Sets new movement frequency.
    void setPurpose(const string &p);
    //!< Set the descriptive string describing Sprite
    void setSpline(int s);
    //!< Manually sets the number of pixels to spline.
    void setSprite(spriteType st);
    //!< Manually change sprite type (facing, etc).
    const action topAct();
    //!< Returns the next action in the queue.
  protected:
    queue<action> actionQ;
    //!< Queue of upcoming moves to make.
    dir facing;
    //!< Direction Sprite is currently facing.
    TTF_Font *font;
    //!< Font the Sprite uses to 'speak'.
    int moveFreqMax;
    //!< Max number of seconds between moving. Ignored if min is negative.
    int moveFreqMin;
    //!< Min number of seconds between moving. Negative means stationary.
    string name;
    //!< Name of the Sprite. Used for speech.
    string purpose;
    //!< String describing the purpose of Sprite.
    queue<string> speech;
    //!< Circular queue of everything this Sprite says.
    int spline;
    //!< Number of pixels left to be splined towards destination.
    spriteType sprite;
    //!< Which sprite on the spritesheet is currently being used.
    SDL_Texture *spriteSheet;
    //!< The spritesheet used to draw this Sprite.
    unsigned int ticks;
    //!< Timer cycles until next action
  };

#endif

