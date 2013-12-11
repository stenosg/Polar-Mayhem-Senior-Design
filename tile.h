#ifndef _TILE_H_
#define _TILE_H_

#include <Ogre.h>
#include "unit.h"
#include <string>
struct Unit;
class Tiles
{
 private:
  
  Ogre::SceneNode *sn;
  Ogre::Entity *e;
  int x; //index
  int y;
  int h;
  enum terrain {water, tundra, ice} t;
  //SOME TERRAIN
 public:
  Tiles();
  void initTile(Ogre::SceneManager *s,std::string name,std::string mesh,std::string mat, Ogre::Vector3 v,int x,int y,int h,int tileD);
  bool hasUnit();
  Ogre::SceneNode *getSceneNode();
  Ogre::Entity *getEntity();
  Unit * u;
  int getX();
  int getY();
  int getH();
};

#endif
