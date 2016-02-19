// Samuel Weber

#ifndef GAMESTATE_H
  #define GAMESTATE_H

  #include "Draw.h"
  #include "Globals.h"
  #include "Keybind.h"
  #include "Party.h"

  //! The game's current state. Handles the main event loop.
  class GameState
  {
  public:
    GameState(SDL_Renderer *ren);
    //!< GameState constructor.
    ~GameState();
    //!< GameState destructor.
    void advance(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party,
            vector<shared_ptr<Unit> > &enemies);
    //!< Loop to select which event loop to run. Advances state.
    void changeTerr(const string &newTerr);
    //!< Changes the active Terr.
    void decCursorPos(unsigned int max);
    //!< subtracts one from cursorPos, looping to max if needed.
    void incCursorPos(unsigned int max);
    //!< Adds one to cursorPos, looping to zero if needed.
    long rng(long min, long max);
    //!< Returns a uniformly distributed random number between min and max.
    void setState(gameState gs);
    //!< Sets gameState.
    shared_ptr<Tile> tileClick(SDL_MouseButtonEvent &click,
            shared_ptr<Sprite> sprite);
    //!< Used when clicking on the map. Returns a pointer to the Tile clicked.
  protected:
    unsigned int cursorPos;
    //!< Position of cursor in menus.
    mt19937_64 randNumGen;
    //!< Random Number Generator.
    gameState state;
    //!< State of game.
    unique_ptr<Terr> terr;
    //!< Currently loaded terr.
    SDL_TimerID timerID;
    //!< Timer for regular events

    void interactMessageHandler(unique_ptr<Party> &party, string &message);
    //!< Message handler for sprites interacting.
    void loopAnyState(SDL_Event &e, unique_ptr<Party> &party);
    //!< Loop to handle input that’s treated the same in all states.
    void loopBattle(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party,
            vector<shared_ptr<Unit> > &enemies);
    //!< Loop to process actions in battle state.
    void loopBattleFight(TTF_Font *font, unique_ptr<Party> &party,
            vector<shared_ptr<Unit> > &enemies);
    //!< Subloop for the 'fight' command in battle.
    void loopBattleRun(TTF_Font *font, vector<shared_ptr<Unit> > &enemies);
    //!< Subloop for the 'run' command in battle.
    void loopMap(SDL_Event &e, unique_ptr<Party> &party,
            vector<shared_ptr<Unit> > &enemies);
    //!< Loop to process inputs while on overworld map.
    void loopRebind(SDL_Renderer *ren, SDL_Event &e, TTF_Font *font);
    //!< Loop to process inputs and events while in the key binding menu.
    void loopTitle(SDL_Event &e, TTF_Font *font, unique_ptr<Party> &party);
    //!< Loop to process inputs on the title screen.
  };

#endif
