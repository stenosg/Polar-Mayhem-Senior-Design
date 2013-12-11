#ifndef _FISHING_HOLE_H_
#define _FISHING_HOLE_H_

#include "unit.h"
#include "stats.h"
#include "player.h"
class FishingHole: public Unit
{
  protected:
    Player *controller;
    int infRange; //range units have to be in to influence 
    int manhattan(int x1,int y1,int x2,int y2);

  public:
    FishingHole(Ogre::SceneManager *s,Player *owner,std::string name, std::string mesh, Ogre::Vector3 v,Ogre::Vector3 init,Stats stats);
    void update(Ogre::Real time,Player *p1,Player *p2);
    void changeControl(Player *newPlayer);
};

#endif