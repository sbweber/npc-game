// Samuel Weber

#include "Unit.h"


Unit::Unit()
{
  str = 10;
  intl = 10;
  agi = 10;
  vit = 10;
  wis = 10;
  maxHP = currHP = maxMP = currMP = 100;
  recalcStats();
}  // Unit::Unit()


Attack Unit::attack(mt19937_64 &randNumGen)
{
  return Attack(str * 20, agi, DAMAGE_PHYS, randNumGen);
}  // long Unit::attack()


critical Unit::calcCrit(long acc, mt19937_64 &randNumGen)
{
  long max = (2 * acc) + (2 * agi);
  uniform_int_distribution<long> dist(0, max);
  long roll = dist(randNumGen);
  if (roll < acc)
    return HIT_CRIT;
  else if (roll > (2 * acc + agi))
    return HIT_WEAK;
  return HIT_AVG;
}  // critical Unit::calcCrit(long acc)


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


void Unit::recalcStats()
{
  long lostHP = maxHP - currHP, lostMP = maxMP - currMP;
  maxHP = vit * 50;
  currHP = maxHP - lostHP;
  maxMP = wis * 50;
  currMP = maxMP - lostMP;
}  // void Unit::recalcStats()


long Unit::receiveAttack(Attack attack, mt19937_64 &randNumGen)
{
  uniform_int_distribution<long> dist(-10, 10);
  long defstat;
  switch (attack.getElement())
  {
  case DAMAGE_PHYS:
    defstat = vit;
    break;
  case DAMAGE_MAGIC:
    defstat = wis;
    break;
  default:
    defstat = 0;
    break;  // should be impossible to reach here
  }
  long def = long(defstat * 10 * (1 + double(dist(randNumGen)) / 100));
  long damage = attack.getDamage() - def;

  critical crit = calcCrit(attack.getAcc(), randNumGen);
  if (crit == HIT_WEAK)
    damage = long(double(damage) * 0.8);
  else if (crit == HIT_CRIT)
    damage = long(double(damage) * 1.2);

  if (damage > currHP)
    currHP = 0;
  else
    currHP -= damage;
  return damage;
}  // void Unit::receiveAttack(long damage)

