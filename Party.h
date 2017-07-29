// Samuel Weber

#ifndef PARTY_H
  #define PARTY_H

  #include "Globals.h"
  #include "Sprite.h"
  #include "Terrain.h"
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
    unsigned long long addDebt(unsigned long long val);
    //!< Increases debt by val, then returns debt.
    unsigned long long getDebt();
    //!< Returns debt.
    unsigned long getGold();
    //!< Returns gold.
    unsigned long long getScore();
    //!< Score is equal to gold - debt.
    shared_ptr<Sprite> getSprite();
    //!< Returns the Sprite representing the party by reference.
    shared_ptr<Unit> getUnit(int i);
    //!< Returns specified active unit in party.
    vector<shared_ptr<Unit> >& getUnits();
    //!< Returns a vector of the active Units in the party.
    bool keepMoving();
    //!< Returns true if sprite's actionQ is size 1 and contains an ACT_MOVE.
    void move(DIR moveDir);
    //!< Pushes a movement action to sprite.
    void setSprite(SDL_Renderer *ren, const string &str);
    //!< Resets the party’s Sprite to the one in the file named by str.
    unsigned long transactGold(long val);
    //!< Adds (signed!) val to gold, then returns gold.
  protected:
    vector<shared_ptr<Unit> > active;
    //!< Vector of pointers to the active party members.
    unsigned long long debt;
    //!< Party's debt.
    unsigned long gold;
    //!< Party's money.
    vector<shared_ptr<Unit> > passive;
    //!< Vector of pointers to the non-active party members.
    shared_ptr<Sprite> sprite;
    //!< Sprite representing the player's party.
  };
#endif