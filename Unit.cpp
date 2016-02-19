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


long Unit::getCurrHP()
{
  return currHP;
}  // long Unit::getCurrHP()


long Unit::getMaxHP()
{
  return maxHP;
}  // long Unit::getMaxHP()


bool Unit::isDead()
{
  if (currHP <= 0)
    return true;
  return false;
}  // bool Unit::isDead()


long Unit::receiveAttack(long damage)
{
  if (damage > currHP)
    currHP = 0;
  else
    currHP -= damage;
  return damage;
}  // void Unit::receiveAttack(long damage)

