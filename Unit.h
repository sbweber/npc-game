// Samuel Weber

#ifndef UNIT_H
  #define UNIT_H

  //! Backend of a character’s data. Visual depiction handled by Sprite class.
  class Unit
  {
  public:
    Unit();
    //!< Unit constructor
    long attack();
    //!< Returns the amount of damage the attack should deal (before defenses).
    void fullHeal();
    //!< Restores all HP.
    bool isDead();
    //!< Returns true if currHP is less than or equal to zero.
    long receiveAttack(long damage);
    //!< Given an attack, takes damage based on stats. Returns damage taken.
  protected:
    long maxHP;
    //!< Unit's maximum HP.
    long currHP;
    //!< Unit's current HP.
  };

#endif

