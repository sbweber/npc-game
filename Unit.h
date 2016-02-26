// Samuel Weber

#ifndef UNIT_H
  #define UNIT_H

  #include "Attack.h"


  //! Backend of a character’s data. Visual depiction handled by Sprite class.
  class Unit
  {
  public:
    Unit(string n);
    //!< Unit constructor
    Attack attack(mt19937_64 &randNumGen);
    //!< Returns the amount of damage the attack should deal (before defenses).
    critical calcCrit(long acc, mt19937_64 &randNumGen);
    //!< Calculates whether a hit is weak, avg, or a crit.
    void fullHeal();
    //!< Restores all HP.
    long getAgi();
    //!< returns agi.
    long long getCurrHP();
    //!< Returns currHP.
    long long getMaxHP();
    //!< Returns maxHP.
    string getName();
    //!< Returns name.
    bool isDead();
    //!< Returns true if currHP is less than or equal to zero.
    void recalcStats();
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
    long intl;
    //!< Deal more magical damage.
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
  };

#endif

