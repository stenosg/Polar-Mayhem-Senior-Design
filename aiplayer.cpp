#include "aiplayer.h"


AIPlayer::AIPlayer():Player()
{

}


bool AIPlayer::makeMove(Unit *u,std::string &gameInfo)
{
	if(!moveTaken)
	{

		closest = NULL;
		int dist = 0;
		for(int i=0; i < opp->getBattleUnits().size(); i++)
		{
			Unit *cand = opp->getBattleUnits()[i];
			int newdist = map->manhattan(u->getX(), u->getY(),cand->getX(),cand->getY());
			if(!closest)
			{
				if(cand->isAlive())
				{
					closest = cand;
					dist = newdist;
				}
			}else if(newdist < dist && cand->isAlive())
			{
				closest = cand;
				dist = newdist;
			}
		}
		if(!closest)
		{
			moveTaken = true;

		}else
		{
			std::vector<Tiles *> visitable = map->unitMoveBFS(u);
			dist = 0;
			Tiles *closeT = NULL;
			for(int i=0; i < visitable.size(); i++)
			{
				Tiles *candT = visitable[i];
				int tmpdist = map->manhattan(closest->getX(), closest->getY(),candT->getX(),candT->getY());
				if(!closeT)
				{
					if(!candT->hasUnit())
					{
						dist = tmpdist;
						closeT = candT;
					}
				}else if(tmpdist < dist)
				{
					if(!candT->hasUnit())
					{
						dist = tmpdist;
						closeT = candT;
					}
				}
			}
			map->selectTile(closeT->getX(), closeT->getY());
			map->moveTile();
			moveTaken=true;
		}
	}else if(u->getAnimState() == Unit::IDLE && !actionTaken)
	{
		if(closest)
		{
			gameInfo =	map->attackUnit(closest->getX(),closest->getY());
		}
		actionTaken=true;
	}


	return moveTaken && actionTaken;
}