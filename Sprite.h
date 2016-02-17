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


  //! Sprite's AI
  enum spriteAI
  {
    S_AI_STAND,  // Stand still forever.
    S_AI_WANDER  // Randomly wander around the map.
  };


  //! Action direction and type.
  typedef tuple<dir, actType> action;


  //! Visual half of a Unit. Information on what to draw to put a sprite onscreen.
  class Sprite
  {
  public:
    Sprite(SDL_Renderer *ren, const string &spriteFile, const string &n = "",
            const string &p = "", const string &scriptFile = "");
    //!< All units MUST have a spritesheet specified. type string optional.
    ~Sprite();
    //!< Default destructor
    void changeDir(dir d);
    //!< Change what direction Sprite is facing. Usuallly part of movement.
    void clearActs();
    //!< Removes all elements from the action queue.
    bool decSpline();
    //!< If spline is non-zero, decrements it and returns true. Else false.
    bool decTicks();
    //!< Decrement ticks. Then, if it's zero, return true and reset it.
    dir getFacing();
    //!< Returns which direction the Sprite is currently facing.
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
    void setPurpose(const string &p);
    //!< Set the descriptive string describing Sprite
    void setSpline(int s);
    //!< Manually sets the number of pixels to spline.
    void setSprite(spriteType st);
    //!< Manually change sprite type (facing, etc).
  protected:
    queue<action> actionQ;
    //!< Queue of upcoming moves to make.
    dir facing;
    //!< Direction Sprite is currently facing.
    TTF_Font *font;
    //!< Font the Sprite uses to 'speak'.
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

