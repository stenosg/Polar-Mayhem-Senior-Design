#ifndef _ITEM_H_
#define _ITEM_H_

#include "unit.h"

struct Unit;
class Item{
 public:
  int id;
  std::string name;
  int price;
  //int elvl
  virtual void equipTo(Unit * u){}
  virtual ~Item() {}
 
};
class Weapon: public Item
{
 public:
  int WP;
  //int Ev;
  int range;
  enum WeaponType{Physical,Speed,Magic} weaponType;
  Weapon(int id, std::string name, int price, int WP, int range, WeaponType t);
  void equipTo(Unit *u);
};

class Armor: public Item
{
 public:
  int BHP;
  int BMP;
  Armor(int id, std::string name, int price, int BHP, int BMP);
  void equipTo(Unit *u);
};

#endif
