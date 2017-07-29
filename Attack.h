// Samuel Weber

#ifndef ATTACK_H
  #define ATTACK_H

  #include "Globals.h"

  enum DAMAGE_TYPE
  {
    DAMAGE_PHYS,
    DAMAGE_MAGIC
  };

  enum CRITICAL
  {
    HIT_WEAK = -1,
    HIT_AVG = 0,
    HIT_CRIT = 1
  };


  //! Data for an attack. Damage amount, type, and accuracy.
  class Attack
  {
  public:
    Attack(long long dam, long a, DAMAGE_TYPE e);
    //!< Attack constructor.
    long getAcc();
    //!< Returns accuracy of attack.
    long long getDamage();
    //!< Returns base damage of attack.
    DAMAGE_TYPE getElement();
    //!< Returns element of attack.
  protected:
    long long damage;
    //!< Attack's base damage.
    long acc;
    //!< Attack's accuracy.
    DAMAGE_TYPE element;
    //!< Element of attack.
};

#endif

