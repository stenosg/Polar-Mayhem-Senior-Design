#include "fishinghole.h"
#include <cstdlib>

FishingHole::FishingHole(Ogre::SceneManager *s,Player *owner,std::string name, std::string mesh, Ogre::Vector3 v,Ogre::Vector3 init,Stats stats):Unit(s,NULL,name,mesh,v,init,stats)
{
  infRange = 3;
  controller = NULL;
}
int FishingHole::manhattan(int x1,int y1,int x2,int y2)
{
  return abs(x1-x2) + abs(y1-y2);
} 
void FishingHole::changeControl(Player *newPlayer)
{
  if(controller == newPlayer)return;
  if(controller)
  {
    for(int i=0; i < controller->getBattleUnits().size(); i++)
    {
      controller->getBattleUnits()[i]->stats.Sp/=2;
    }
  }
  controller = newPlayer;
  for(int i=0; i < controller->getBattleUnits().size(); i++)
  {
    controller->getBattleUnits()[i]->stats.Sp*=2;
  }
}
void FishingHole::update(Ogre::Real time,Player *p1,Player *p2)
{
  int numP1 = 0;
  int numP2 = 0;
  for(int i=0; i < p1->getBattleUnits().size(); i++)
  {
    if(p1->getBattleUnits()[i]->isAlive() && manhattan(x,y,p1->getBattleUnits()[i]->getX(),p1->getBattleUnits()[i]->getY()) <= infRange)
    {
      numP1++;
    }
    if(p2->getBattleUnits()[i]->isAlive() && manhattan(x,y,p2->getBattleUnits()[i]->getX(),p2->getBattleUnits()[i]->getY()) <= infRange)
    {
      numP2++;
    }
  }

  if(numP1 > numP2)
  {
    changeControl(p1);
  }
  if(numP2 > numP1)
  {
    changeControl(p2);
  }
}