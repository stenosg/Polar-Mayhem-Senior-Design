#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_
#include "unit.h"
#include "player.h"
#include <vector>
class AIPlayer : public Player
{
  private:
   Unit *closest; 	
  public:
   AIPlayer();
   virtual bool makeMove(Unit *u,std::string &gameInfo);
};

#endif