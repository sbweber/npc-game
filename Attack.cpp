// Samuel Weber

#include "Attack.h"


Attack::Attack(long dam, long a, DAMAGE_TYPE e)
{
  damage = dam;
  acc = a;
  element = e;
}  // Attack::Attack(long dam, long a, damageType e)


long Attack::getAcc()
{
  return acc;
}  // long AttackgetAcc()


long Attack::getDamage()
{
  return damage;
}  // long Attack::getDamage()


DAMAGE_TYPE Attack::getElement()
{
  return element;
}  // damageType Attack::getElement()

