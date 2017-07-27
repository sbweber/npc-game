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
    GameState(SDL_Renderer *ren, TTF_Font *f, Uint32 et);
    //!< GameState constructor.
    ~GameState();
    //!< GameState destructor.
    void advance(SDL_Event &e);
    //!< Loop to select which event loop to run. Advances state.
    void changeTerr(const string &newTerr);
    //!< Changes the active Terr.
    void decCursorPos(unsigned int max);
    //!< subtracts one from cursorPos, looping to max if needed.
    void incCursorPos(unsigned int max);
    //!< Adds one to cursorPos, looping to zero if needed.
    long long rng(long long min, long long max);
    //!< Returns a uniformly distributed random long long from min to max.
    size_t rng(size_t max);
    //!< Returns a uniformly distributed random integer from 0 to (max-1).
    void setState(gameState gs);
    //!< Sets gameState.
    shared_ptr<Tile> tileClick(SDL_MouseButtonEvent &click);
    //!< Used when clicking on the map. Returns a pointer to the Tile clicked.
  protected:
    unsigned int cursorPos;
    //!< Position of cursor in menus.
    vector<shared_ptr<Unit> > enemies;
    //!< Vector of enemy units for battle.
    const Uint64 eventTick;
    //!< ID of user event triggered by ticking.
    TTF_Font *font;
    //!< Primary font for regular usage.
    unsigned int lineNum;
    //!< Line of text in textbox. For adding text to existing textboxes.
    mt19937_64 randNumGen;
    //!< Random Number Generator.
    unique_ptr<Party> party;
    //!< The player's party.
    gameState state;
    //!< State of game.
    unique_ptr<Terrain> terr;
    //!< Currently loaded terr.
    SDL_TimerID timerID;
    //!< Timer for regular events

    void actionMessageHandler(string &message);
    //!< Message handler for sprites doing things.
    void loopAnyState(SDL_Event &e);
    //!< Loop to handle input that’s treated the same in all states.
    void loopBattle(SDL_Event &e);
    //!< Loop to process actions in battle state.
    void loopBattleFight();
    //!< Subloop for the 'fight' command in battle.
    void loopBattleFlee();
    //!< Subloop for the 'run' command in battle.
    void loopBattleResolve(vector<shared_ptr<Unit> > &liveParty,
            vector<shared_ptr<Unit> > &liveEnemies);
    //!< Subloop for determining who won the battle and what to do next.
    void loopBattleTurn(shared_ptr<Unit> attacker,
            vector<shared_ptr<Unit> > &liveParty,
            vector<shared_ptr<Unit> > &liveEnemies);
    //!< Subloop for an individual unit attacking during a round.
    void loopMap(SDL_Event &e);
    //!< Loop to process inputs while on overworld map.
    void loopRebind(SDL_Event &e);
    //!< Loop to process inputs and events while in the key binding menu.
    void loopTitle(SDL_Event &e);
    //!< Loop to process inputs on the title screen.
    void turnOrder(queue<shared_ptr<Unit> > &order);
    //!< Takes party's Unit vector and enemies to fill order in turn order.
  };

#endif
