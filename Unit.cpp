// Samuel Weber

#include "Unit.h"


Unit::Unit()
{
  currHP = maxHP = 250;
}  // Unit::Unit()


long Unit::attack()
{
  return 100;
}  // long Unit::attack()


void Unit::fullHeal()
{
  currHP = maxHP;
}  // void Unit::fullHeal()


void Unit::receiveAttack(long damage)
{
  if (damage > currHP)
    currHP = 0;
  else
    currHP -= damage;
}  // void Unit::receiveAttack(long damage)

