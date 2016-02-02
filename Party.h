// Samuel Weber

#ifndef PARTY_H
  #define PARTY_H

  #include "Globals.h"
  #include "Sprite.h"
  #include "Terr.h"
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
    void changeTerr(const string &newTerr);
    //!< Changes the active Terr.
    SDL_Renderer* getRen();
    //!< Calls Terr's getRen().
    shared_ptr<Sprite> getSprite();
    //!< Returns the Sprite representing the party by reference.
    gameState getState();
    //!< Gets gameState.
    unique_ptr<Terr>& getTerr();
    //!< Returns the current Terr.
    void move(dir d, bool interrupt = true);
    //!< Move player's Sprite. Load new Terr if entering a Warp
    void setLocation(int x, int y);
    //!< Sets the Party's location on the terr to (x, y).
    void setLocation(shared_ptr<Tile> tile);
    //!< Sets the Party's location on the terr to the specified Tile.
    void setSprite(SDL_Renderer *ren, const string &str);
    //!< Resets the party’s Sprite to the one in the file named by str.
    void setState(gameState gs);
    //!< Sets gameState.
    shared_ptr<Tile> tileClick(SDL_MouseButtonEvent &click);
    //!< Used when clicking on the map. Returns a pointer to the Tile clicked.
  protected:
    unique_ptr<Unit> active[4];
    //!< Pointers to the up to four active party members.
    vector<unique_ptr<Unit> > passive;
    //!< Vector of pointers to non-active party members.
    shared_ptr<Sprite> sprite;
    //!< Sprite representing the player's party.
    gameState state;
    //!< State of game.
    unique_ptr<Terr> terr;
    //!< Terr where the player's party currently is.
  };
#endif