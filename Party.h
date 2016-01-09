// Samuel Weber

#ifndef PARTY_H
  #define PARTY_H

  #include "Globals.h"
  #include "Sprite.h"
  #include "Terr.h"
  #include "Unit.h"

  class Sprite;
  class Terr;

  //! Defines the playable party
  //! Describes the hero's party, directly controlled by the player
  class Party
  {
  public:
    Party(SDL_Renderer *ren = NULL);
    //!< Constructs the class, setting the sprite as the ‘Hero’ sprite.
    ~Party();
    //!< Default destructor.
    void changeTerr(const string &newTerr);
    //!< Changes the active Terr.
    Sprite* getSprite();
    //!< Returns the Sprite representing the party by reference.
    Terr* getTerr();
    //!< Returns the current Terr.
    void setLocation(uint8_t x, uint8_t y);
    //!< Sets the Party's location on the terr to (x, y).
    void setSprite(SDL_Renderer *ren, const string &str);
    //!< Resets the party’s Sprite to the one in the file named by str.
  protected:
    Unit* active[4];
    //!< Pointers to the up to four active party members.
    vector<Unit*> passive;
    //!< Vector of pointers to non-active party members.
    Sprite* sprite;
    //!< Sprite representing the player's party.
    Terr* terr;
    //!< Terr where the player's party currently is.
  };
#endif