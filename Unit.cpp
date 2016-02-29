// Samuel Weber

#include "Unit.h"


Unit::Unit(string n)
{
  name = n;
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
  long long damage = long long(str * 20 *
          (1 + double(rng(randNumGen, -10, 10)) / 100));
  return Attack(damage, agi, DAMAGE_PHYS);
}  // long Unit::attack()


critical Unit::calcCrit(long acc, mt19937_64 &randNumGen)
{
  long long roll = rng(randNumGen, 0, ((2 * acc) + (2 * agi)));
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


long Unit::getAgi()
{
  return agi;
}  // long Unit::getAgi()


long long Unit::getCurrHP()
{
  return currHP;
}  // long Unit::getCurrHP()


long long Unit::getMaxHP()
{
  return maxHP;
}  // long Unit::getMaxHP()


string Unit::getName()
{
  return name;
}  // string Unit::getName()


bool Unit::isDead()
{
  if (currHP <= 0)
    return true;
  return false;
}  // bool Unit::isDead()


void Unit::recalcStats()
{
  long long lostHP = maxHP - currHP, lostMP = maxMP - currMP;
  maxHP = vit * 50;
  currHP = maxHP - lostHP;
  maxMP = wis * 50;
  currMP = maxMP - lostMP;
}  // void Unit::recalcStats()


Attack Unit::receiveAttack(Attack attack, mt19937_64 &randNumGen)
{
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
  long long def = long long(defstat * 10 *
          (1 + double(rng(randNumGen, -10, 10)) / 100));
  long long damage = attack.getDamage() - def;

  critical crit = calcCrit(attack.getAcc(), randNumGen);
  if (crit == HIT_WEAK)
    damage = long long(double(damage) * 0.8);
  else if (crit == HIT_CRIT)
    damage = long long(double(damage) * 1.2);

  if (damage > currHP)
    currHP = 0;
  else
    currHP -= damage;
  return Attack(damage, crit, attack.getElement());
}  // void Unit::receiveAttack(long damage)

