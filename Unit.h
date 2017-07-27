// Samuel Weber

#ifndef UNIT_H
  #define UNIT_H

  #include "Attack.h"


//! Backend of a character’s data. Visual depiction handled by Sprite class.
class Unit
{
public:
  Unit();
  //!< Unit constructor
  Attack attack(mt19937_64 &randNumGen);
  //!< Returns the amount of damage the attack should deal (before defenses).
  CRITICAL calcCrit(long acc, mt19937_64 &randNumGen);
  //!< Calculates whether a hit is weak, avg, or a crit.
  void fullHeal();
  //!< Restores all HP.
  long getCurrHP();
  //!< Returns currHP.
  long getMaxHP();
  //!< Returns maxHP.
  bool isDead();
  //!< Returns true if currHP is less than or equal to zero.
  void recalcStats();
  //!< Recalculates HP and MP from stats.
  Attack receiveAttack(Attack attack, mt19937_64 &randNumGen);
  //!< Given an attack, takes damage based on stats. Returns damage taken.
protected:
  long maxHP;
  //!< Unit's maximum HP. How much damage it can take.
  long currHP;
  //!< Unit's current HP.
  long maxMP;
  //!< Unit's maximum MP. Spent on magic.
  long currMP;
  //!< Unit's current MP.
  long str;
  //!< Deal more physical damage.
  long intl;
  //!< Deal more magical damage.
  long agi;
  //!< Increase odds of getting a crit.
  //!< Increase odds of acting earlier, decrease odds of being crit.
  long vit;
  //!< Increase maxHP, decrease physical damage taken.
  long wis;
  //!< Increase maxMP, decrease magical damage taken.
};

#endif

