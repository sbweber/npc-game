// Samuel Weber

#ifndef PARTY_H
  #define PARTY_H

  #include "Globals.h"
  #include "Sprite.h"
  #include "Unit.h"

  //! Defines the playable party
  //! Describes the hero's party, directly controlled by the player
  class Party
  {
  public:
    Party();
    //!< Default constructor.
    Party(SDL_Renderer *ren);
    //!< Constructs the class, setting the sprite as the ‘Hero’ sprite.    ~Party();
    //!< Default destructor.
    Sprite* getSprite();
    //!< Returns the Sprite representing the party by reference.
    void setSprite(SDL_Renderer *ren, const string &str);
    //!< Resets the party’s Sprite to the one in the file named by str.
  protected:
    Unit* active[4];
    //!< Pointers to the up to four active party members.
    vector<Unit*> passive;
    //!< Vector of pointers to non-active party members.
    Sprite* sprite;
    //!< Sprite representing the player's party.
  };
#endif