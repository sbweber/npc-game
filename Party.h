// Samuel Weber

#ifndef PARTY_H
  #define PARTY_H

  #include "Globals.h"
  #include "Sprite.h"
  #include "Unit.h"

  class Party
  {
    Unit* active[4];  // pointers to the up to four active party members
    vector<Unit*> passive;  // vector of pointers to non-active party members
    Sprite* sprite;  // sprite representing the player's party
  public:
    Party();
    Party(SDL_Renderer *ren);
    ~Party();
    Sprite* getSprite();
    void setSprite(SDL_Renderer *ren, const string &str);
  };  // describes the hero's party, directly controlled by the player

#endif