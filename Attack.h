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
    HIT_WEAK,
    HIT_AVG,
    HIT_CRIT
  };


  //! Data for an attack. Damage amount, type, and accuracy.
  class Attack
  {
  public:
    Attack(long dam, long a, damageType e, mt19937_64 &randNumGen);
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

