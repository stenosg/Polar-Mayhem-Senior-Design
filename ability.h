#ifndef _ABILITY_H_
#define _ABILITY_H_
#include <iostream>
#include <assert.h>
class Ability{
public:
  int id;
  std::string name;
  int range;
  int area;
  int MPCost;
  int HPMod;
  int MPMod;
  int SpMod;
  int PAMod;
  int MAMod; 
  int maxTicks;
  int currentTicks;
 public:

  Ability(int id, std::string name, int range, int MPCost, int HPMod,int MPMod)
    :id(id),name(name),range(range),MPCost(MPCost),HPMod(HPMod),
    MPMod(MPMod),SpMod(0),PAMod(0),MAMod(0),maxTicks(0),currentTicks(0)
    {
    }
 Ability(int id, std::string name, int range,
	 int maxTicks, int MPCost, int HPMod = 0,
	 int MPMod = 0,int SpMod = 0,int PAMod = 0, int MAMod = 0)
    :id(id),name(name),range(range),MPCost(MPCost),HPMod(HPMod),
    MPMod(MPMod),SpMod(SpMod),PAMod(PAMod),MAMod(MAMod),
    maxTicks(maxTicks),currentTicks(0)
    {
      assert(maxTicks != 0);
    }
  bool isInstant(){return maxTicks > 0;};
  void update(int ticks){currentTicks += ticks;};
  bool isFinished(){return currentTicks > maxTicks;};

};

#endif
