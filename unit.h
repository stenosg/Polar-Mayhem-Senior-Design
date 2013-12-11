#ifndef _UNIT_H_
#define _UNIT_H_

#include <Ogre.h>
#include <string>
#include <map>
#include "stats.h"
#include "item.h"
#include "player.h"
class Stats;
class Player;
class Armor;
class Weapon;
class Unit {
 public:
  enum Anim_State{NONE,IDLE,WALK,ATTACK,DEATH,FLINCH};
 protected:
  Ogre::Entity * entity;
  Ogre::SceneNode * sn;
  int x;
  int y;
  Anim_State animState; //animation to use in update;
  Ogre::AnimationState *currAnim;
  Ogre::Vector3 initial_direction;
  Ogre::Quaternion orientation;
  std::string charName;
  std::map<Anim_State,std::string> animations;
  Player *owner;
  Armor * armor;
  Weapon * weapon;

 public:
  //STATS
  Stats stats;
  int yoffset;
  Player * getOwner();
  std::string getCharName();
  void setAnimState(Anim_State state);
  ~Unit();
  virtual void update(Ogre::Real time,Player *p1,Player *p2);
  Unit(Ogre::SceneManager *s,Player *owner,std::string name, std::string mesh, Ogre::Vector3 v,Ogre::Vector3 init,Stats stats);
  void equipWeapon(Weapon * w);
  void equipArmor(Armor * a);
  Ogre::Entity * getEntity();
  Ogre::SceneNode * getSceneNode();
  void move(int x,int y);
  int getX();
  int getY();
  void setX(int x);
  void setY(int y);
  bool inRange(int x, int y);
  virtual void update(Ogre::Real time);
  Unit::Anim_State getAnimState();
  Ogre::Vector3 getDirection();
  Ogre::Vector3 getInitialDirection();
  void setDirection(Ogre::Vector3 direction);
  void setInitialDirection(Ogre::Vector3 direction);
  void setOrientation(Ogre::Quaternion q);
  Ogre::Quaternion getOrientation();
  std::string attacked(Unit *u,int h1, int h2); //return dmg dealt
  bool isAlive();
  void reset();

};

#endif
