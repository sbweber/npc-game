// Samuel Weber

#ifndef ATTACK_H
  #define ATTACK_H

  #include "Globals.h"


  enum damageType
  {
    DAMAGE_PHYS,
    DAMAGE_MAGIC
  };


  enum critical
  {
    HIT_WEAK = -1,
    HIT_AVG = 0,
    HIT_CRIT = 1
  };


  //! Data for an attack. Damage amount, type, and accuracy.
  class Attack
  {
  public:
    Attack(long dam, long a, damageType e);
    //!< Attack constructor.
    long getAcc();
    //!< Returns accuracy of attack.
    long getDamage();
    //!< Returns base damage of attack.
    damageType getElement();
    //!< Returns element of attack.
  protected:
    long damage;
    //!< Attack's base damage.
    long acc;
    //!< Attack's accuracy.
    damageType element;
    //!< Element of attack.
  };

#endif

