#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "unit.h"
#include "map.h"
#include <vector>
struct Unit;
class Map;
class Player
{
  protected:
   Map *map;
   std::vector<Unit *> units;
   std::vector<Unit *> battleUnits;
   Player *opp;
   
  public:
   bool moveTaken;
   bool actionTaken;
   Player();
   void setOpp(Player *);
   Player * getOpp();
   virtual bool makeMove(Unit *u,std::string &gameInfo);
   void unitReady();
   void initPlayer(Ogre::SceneManager *s,std::string name,std::string mesh,Ogre::Vector3 pos, Ogre::Vector3 initDir);
   void setMap(Map *m);
   std::vector<Unit *> & getBattleUnits();
};

#endif
