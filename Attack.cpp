// Samuel Weber

#include "Attack.h"


Attack::Attack(long dam, long a, damageType e, mt19937_64 &randNumGen)
{
  uniform_int_distribution<long> dist(-10, 10);
  damage = long(dam * (1 + double(dist(randNumGen)) / 100));
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


damageType Attack::getElement()
{
  return element;
}  // damageType Attack::getElement()

