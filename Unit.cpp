// Samuel Weber

#include "Unit.h"


Unit::Unit(string n, string g, long l)
{
  name = n;
  if (!g.compare("King"))
    growth = GROWTH_KING;
  else if (!g.compare("Shopkeeper"))
    growth = GROWTH_SHOPKEEPER;
  else
    growth = GROWTH_STATIC;
  str = 10;
  intl = 10;
  agi = 10;
  vit = 10;
  wis = 10;
  level = l;
  gold = 0;
  xp = 0;
  maxHP = currHP = maxMP = currMP = 0;
  recalcStats();
}  // Unit::Unit(string &n, string &g, long l)


Unit::Unit(string n, long s, long i, long a, long v, long w, long g, long x)
{
  name = n;
  growth = GROWTH_STATIC;
  str = s;
  intl = i;
  agi = a;
  vit = v;
  wis = w;
  level = max(long(1), (str + intl + agi + vit + wis) / 5 - 10);
  gold = g;
  xp = x;
  maxHP = currHP = maxMP = currMP = 0;
  recalcStats();
}  // Unit::Unit(string &n, long s, long i, long a, long v, long w)


Attack Unit::attack(mt19937_64 &randNumGen)
{
  long long damage = long long(str * 20 *
          (1 + double(rng(randNumGen, -10, 10)) / 100));
  return Attack(damage, agi, DAMAGE_PHYS);
}  // long Unit::attack()


CRITICAL Unit::calcCrit(long acc, mt19937_64 &randNumGen)
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


bool Unit::gainXP(long x)
{
  bool retval = false;
  xp += x;
  while (xp >= (level * 100))
  {
    xp -= (level * 100);
    level++;
    retval = true;
  }
  recalcStats((maxHP - currHP), (maxMP - currMP));
  return retval;
}  // void Unit::gainXP(long x)


long Unit::getAgi()
{
  return agi;
}  // long Unit::getAgi()


long long Unit::getCurrHP()
{
  return currHP;
}  // long Unit::getCurrHP()


long Unit::getGold()
{
  return gold;
}  // long Unit::getGold()


long Unit::getLevel()
{
  return level;
}  // long Unit::getLevel()


long long Unit::getMaxHP()
{
  return maxHP;
}  // long Unit::getMaxHP()


string Unit::getName()
{
  return name;
}  // string Unit::getName()


long Unit::getXP()
{
  return xp;
}  // long Unit::getXP()


bool Unit::isDead()
{
  if (currHP <= 0)
    return true;
  return false;
}  // bool Unit::isDead()


void Unit::recalcStats(long long lostHP, long long lostMP)
{
  switch (growth)
  {
  case GROWTH_KING:
    str = 10 + level;
    intl = 10 + level;
    agi = 10 + level;
    vit = 10 + level;
    wis = 10 + level;
    break;
  case GROWTH_SHOPKEEPER:
    str = 10 + long(floor(1.5 * level));
    intl = 10;
    agi = 10 + level;
    vit = 10 + long(ceil(2.5 * level));
    wis = 10;
    break;
  default:
    break;
  }

  if (!lostHP)
    lostHP = maxHP - currHP;
  if (!lostMP)
    lostMP = maxMP - currMP;
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

  CRITICAL crit = calcCrit(attack.getAcc(), randNumGen);
  if (crit == HIT_WEAK)
    damage = long long(double(damage) * 0.8);
  else if (crit == HIT_CRIT)
    damage = long long(double(damage) * 1.2);

  if (damage < 1)
    damage = 1;  // minimum damage is chip damage of 1.

  if (damage > currHP)
    currHP = 0;
  else
    currHP -= damage;
  return Attack(damage, crit, attack.getElement());
}  // void Unit::receiveAttack(long damage)

