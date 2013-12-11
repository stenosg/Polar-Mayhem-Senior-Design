#include "tile.h"
#include <iostream>
Tiles::Tiles(){}
void Tiles::initTile(Ogre::SceneManager *s,std::string name,std::string mesh,std::string mat, Ogre::Vector3 v,int x,int y,int h,int tileD)
{
  float d = tileD/100.0;
  std::cout<<name<<" ("<<v.x<<","<<v.y<<","<<v.z<<")"<<std::endl;
  sn = s->getRootSceneNode()->createChildSceneNode( name+"node",v);
  e = s->createEntity(name,Ogre::SceneManager::PT_CUBE);
  sn->attachObject(e);
  sn->scale(Ogre::Vector3(1,d*(h+1),1));
  
  e->setMaterialName(mat);
  e->setCastShadows(false);

  Ogre::AxisAlignedBox box = e->getBoundingBox();
  box.scale(Ogre::Vector3(1,d*(h+1),1));
  float dy = box.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM ).y + tileD;
  sn->translate(0,-dy,0);
  u = NULL;
  this->x=x;
  this->y=y;
  this->h=h;

}

bool Tiles::hasUnit()
{
  return u!=NULL;
}

Ogre::SceneNode * Tiles::getSceneNode()
{
	return sn;
}
Ogre::Entity * Tiles::getEntity()
{
  return e;
}
int Tiles::getX()
{
	return x;
}
int Tiles::getY()
{
	return y;
}
int Tiles::getH()
{
  return h;
  
}
