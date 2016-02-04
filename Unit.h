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
    void receiveAttack(long damage);
    //!< Given an attack, takes damage based on stats.
  protected:
    long maxHP;
    //!< Unit's maximum HP.
    long currHP;
    //!< Unit's current HP.
  };

#endif

