// Samuel Weber

#ifndef UNIT_H
  #define UNIT_H

  #include "Attack.h"


  enum GROWTH_TYPE
  {
    GROWTH_STATIC,
    GROWTH_KING,
    GROWTH_SHOPKEEPER
  };


  //! Backend of a character’s data. Visual depiction handled by Sprite class.
  class Unit
  {
  public:
    Unit(string n, string g, long l);
    //!< Unit constructor based on growth type.
    Unit(string n, long s, long i, long a, long v, long w, long g, long x);
    //!< Unit constructor with defined stats.
    Attack attack(mt19937_64 &randNumGen);
    //!< Returns the amount of damage the attack should deal (before defenses).
    CRITICAL calcCrit(long acc, mt19937_64 &randNumGen);
    //!< Calculates whether a hit is weak, avg, or a crit.
    void fullHeal();
    //!< Restores all HP.
    bool gainXP(long x);
    //!< Gains XP, leveling as needed. Returns true if a level was gained.
    long getAgi();
    //!< returns agi.
    long long getCurrHP();
    //!< Returns currHP.
    long getGold();
    //!< returns gold.
    long getLevel();
    //!< returns level.
    long long getMaxHP();
    //!< Returns maxHP.
    string getName();
    //!< Returns name.
    long getXP();
    //!< returns XP.
    bool isDead();
    //!< Returns true if currHP is less than or equal to zero.
    void recalcStats(long long lostHP = 0, long long lostMP = 0);
    //!< Recalculates HP and MP from stats.
    Attack receiveAttack(Attack attack, mt19937_64 &randNumGen);
    //!< Given an attack, takes damage based on stats. Returns damage taken.
  protected:
    long agi;
    //!< Increase odds of getting a crit.
    //!< Increase odds of acting earlier, decrease odds of being crit.
    long long currHP;
    //!< Unit's current HP.
    long long currMP;
    //!< Unit's current MP.
    long gold;
    //!< How much loot the Unit grants when beaten. 0 for party.
    GROWTH_TYPE growth;
    //!< Rate at which Unit's stats grow.
    long intl;
    //!< Deal more magical damage.
    long level;
    //!< Unit's level. Higher level means higher stats.
    long long maxHP;
    //!< Unit's maximum HP. How much damage it can take.
    long long maxMP;
    //!< Unit's maximum MP. Spent on magic.
    string name;
    //!< Unit's name. Used for saying who attacked who.
    long str;
    //!< Deal more physical damage.
    long vit;
    //!< Increase maxHP, decrease physical damage taken.
    long wis;
    //!< Increase maxMP, decrease magical damage taken.
    long xp;
    //!< For the party, growing one level takes 100 XP * level.
    //!< For enemies, this is how much XP they given when defeated.
  };

#endif

