// Samuel Weber

#ifndef PARTY_H
  #define PARTY_H

  #include "Globals.h"
  #include "Sprite.h"
  #include "Terr.h"
  #include "Timer.h"
  #include "Unit.h"


  //! Defines the playable party
  //! Describes the hero's party, directly controlled by the player
  class Party
  {
  public:
    Party(SDL_Renderer *ren = nullptr);
    //!< Constructs the class, setting the sprite as the ‘Hero’ sprite.
    ~Party();
    //!< Default destructor.
    shared_ptr<Sprite> getSprite();
    //!< Returns the Sprite representing the party by reference.
    shared_ptr<Unit> getUnit(int i);
    //!< Returns specified active unit in party.
    bool keepMoving();
    //!< Returns true if sprite's actionQ is size 1 and contains a MOVE.
    void move(dir moveDir);
    //!< Pushes a movement action to sprite.
    void setSprite(SDL_Renderer *ren, const string &str);
    //!< Resets the party’s Sprite to the one in the file named by str.
  protected:
    shared_ptr<Unit> active[4];
    //!< Pointers to the up to four active party members.
    vector<shared_ptr<Unit> > passive;
    //!< Vector of pointers to non-active party members.
    shared_ptr<Sprite> sprite;
    //!< Sprite representing the player's party.
  };
#endif