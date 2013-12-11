#include "unit.h"
#include "item.h"
Weapon::Weapon(int id, std::string name, int price, int WP, int range, WeaponType t)
:WP(WP),range(range),weaponType(t) {
  this->id = id;
  this->name = name;
  this->price = price;
}
void Weapon::equipTo(Unit * u)
{
  u->equipWeapon(this);
}
Armor::Armor(int id, std::string name, int price, int BHP, int BMP)
:BHP(BHP),BMP(BMP) {
  this->id = id;
  this->name = name;
  this->price = price;
}
void Armor::equipTo(Unit *u)
{
  u->equipArmor(this);
}
