#include "gamemanager.h"


GameManager::GameManager(Ogre::SceneManager *s):leftKey(OIS::KC_S),rightKey(OIS::KC_W),upKey(OIS::KC_A),downKey(OIS::KC_D),playerTurnSpeed(100)
{
	//gameState = WORLD; //game will begin in world state
	gameState = BATTLE;
	map = NULL;
	currUnit = NULL;
	playerOne = new Player();
	playerTwo = new AIPlayer();
	playerOne->initPlayer(s,"Bear","bear.mesh",Ogre::Vector3(0,0,0),Ogre::Vector3::NEGATIVE_UNIT_Z);
	//playerTwo->initPlayer(s,"Robot","robot.mesh",Ogre::Vector3(0,0,0),Ogre::Vector3::UNIT_X,Stats(5,1,2,5,2));
	playerTwo->initPlayer(s,"Tux","tux.mesh",Ogre::Vector3(0,0,0),Ogre::Vector3::UNIT_Z);
	textRenderer = new TextRenderer();
}
void GameManager::generateMap(Ogre::SceneManager *s,std::string mesh,std::string mat,int maxX,int maxY,int tileW,int tileH, int tileD)
{
        map = new Map(s,mesh,mat,maxX,maxY,tileW,tileH,tileD);
	currUnit = NULL;
	playerOne->setMap(map);
	playerTwo->setMap(map);
	playerTwo->setOpp(playerOne);
	playerOne->setOpp(playerTwo);
	std::cout<<"Player One opp = player two "<<(playerTwo==playerOne->getOpp())<<"Player two opp = player one "<<(playerOne==playerTwo->getOpp())<<std::endl;
	for(int i=0; i < playerOne->getBattleUnits().size(); i++)
	{
		Unit *u = playerOne->getBattleUnits()[i];
		u->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);
		u->getSceneNode()->setVisible(true);
		std::cerr<<"addunit "<<i<<" "<<0<<std::endl;
		map->addUnit(u, i, 0);
		activeUnits.push_back(u);
	}
	for(int i=0; i < playerTwo->getBattleUnits().size(); i++)
	{
		Unit *u = playerTwo->getBattleUnits()[i];
		u->setDirection(Ogre::Vector3::UNIT_Z);
		u->getSceneNode()->setVisible(true);
		std::cerr<<"addunit "<<i<<" "<<(map->getMaxY()-1)<<std::endl;
		map->addUnit(u, i, map->getMaxY()-1);
		activeUnits.push_back(u);
	}
	map->selectUnit(activeUnits[0]);
	map->selectTile(0, 0);

	fishingHole = new FishingHole(s,NULL,"FishHole","fishing_hole.mesh",Ogre::Vector3(0,0,0),Ogre::Vector3::UNIT_Z,Stats(Stats::WARRIOR));
	fishingHole->getSceneNode()->setVisible(true);
	map->addUnit(fishingHole, maxX-1, maxY/2);	


	gameInfo= "Welcome to Polar Mayhem";
	TextRenderer::getSingleton().addTextBox("gameInfo", gameInfo, 100, 20, 100, 20, Ogre::ColourValue(0,1,0,1));
	soundManager.initialize("media/music/Polar_Mayhem.wav");
	soundManager.setLooping(true);
	soundManager.play();
}	
std::string GameManager::checkVictory()
{
	std::string ret = gameInfo;
	for(int i=0; i < playerTwo->getBattleUnits().size();i++)
	{
		if(playerTwo->getBattleUnits()[i]->isAlive())
		{
			for(int j=0; j < playerOne->getBattleUnits().size(); j++)
			{
				if(playerOne->getBattleUnits()[j]->isAlive())
				{
					return ret;
				}
			}
			ret = "You lose";
			return ret;
		}
	}
	ret = "You win";
	return ret;
}
void GameManager::cameraChanged(int index)
{
	switch(index)
	{
		case 0:
			leftKey = OIS::KC_S;
			rightKey = OIS::KC_W;
			upKey = OIS::KC_A;
			downKey = OIS::KC_D;
		break;
		case 1:
			leftKey = OIS::KC_A;
			rightKey = OIS::KC_D;
			upKey = OIS::KC_W;
			downKey = OIS::KC_S;

		break;
		case 2:
			leftKey = OIS::KC_D;
			rightKey = OIS::KC_A;
			upKey = OIS::KC_S;
			downKey = OIS::KC_W;
			

		break;
		case 3:
			leftKey = OIS::KC_S;
			rightKey = OIS::KC_W;
			upKey = OIS::KC_A;
			downKey = OIS::KC_D;

		break;
	}
}
void GameManager::update(Ogre::SceneManager *sm,Ogre::Real time)
{
	switch(gameState)
	{
		case BATTLE:
			if(map)
			{
				gameInfo = checkVictory();
				TextRenderer::getSingleton().setText("gameInfo", gameInfo);
				if(!currUnit)
				{
					if(readyUnits.empty())
					{
						makeUnitReady();
					}
					currUnit = readyUnits.front();
					readyUnits.pop();
					while(!currUnit->isAlive())
					{
						if(readyUnits.empty())
						{
							makeUnitReady();
						}
						currUnit = readyUnits.front();
						readyUnits.pop();
					}
					currUnit->getOwner()->unitReady();
					map->selectUnit(currUnit);
					
				}

				//now have a ready unit whose turn must be made
				if(currUnit->getOwner()->makeMove(currUnit,gameInfo) && currUnit->getAnimState() == Unit::IDLE)
				{
					//if player makes move clear curr unit so next ready unit can move
					std::cerr<<"Finished move"<<std::endl;
					map->deColor();
					currUnit = NULL;
				}
				map->update(sm,time);
				fishingHole->update(time,playerOne,playerTwo);
			}
			break;
		case WORLD:
		  
			break;
	}
}

void GameManager::makeUnitReady()
{
	bool rdy = false;
	while(!rdy)
	{
		for(int i=0; i < activeUnits.size(); i++)
		{
			Unit *u = activeUnits[i];
			u->stats.speedCnt+=u->stats.Sp;
			if(u->stats.speedCnt >= playerTurnSpeed)
			{
				rdy = true;
				u->stats.speedCnt-=playerTurnSpeed;
				readyUnits.push(u);
			}
		}
	}
}

bool GameManager::keyPressed(const OIS::KeyEvent& evt)
{

	if((&(*currUnit->getOwner())== &(*playerOne) ) )
	{
		if(upKey == evt.key)
		{
			  map->setCursorY(map->getCursorY()+1);
			  if(map->getCursorY() >= map->getMaxY())
			  {
				map->setCursorY(map->getCursorY()-1);
			  }
			  std::string tmp = map->selectTile(map->getCursorX(),map->getCursorY());
			  if(tmp != "")
			  {
			  	gameInfo = tmp;
			  }
		}
		if(downKey == evt.key)
		{
			  map->setCursorY(map->getCursorY()-1);
			  if(map->getCursorY() < 0)
			  {
			  	 map->setCursorY(0);
			  }
			  std::string tmp = map->selectTile(map->getCursorX(),map->getCursorY());
			  if(tmp != "")
			  {
			  	gameInfo = tmp;
			  }
		}
		if(leftKey == evt.key)
		{
			   map->setCursorX(map->getCursorX()-1);
			  if(map->getCursorX() < 0)
			  {
			  	map->setCursorX(0);
			  }
			  std::string tmp = map->selectTile(map->getCursorX(),map->getCursorY());
			  if(tmp != "")
			  {
			  	gameInfo = tmp;
			  }
		}
		if(rightKey == evt.key)
		{
			  map->setCursorX(map->getCursorX()+1);
			  if(map->getCursorX() >= map->getMaxX())
			  {
			  	map->setCursorX(map->getCursorX()-1);
			  }
			  std::string tmp = map->selectTile(map->getCursorX(),map->getCursorY());
			  if(tmp != "")
			  {
			  	gameInfo = tmp;
			  }
		}
		if(OIS::KC_RETURN == evt.key)
		{
			  std::cout<<"Move "<<map->getCursorX()<<" "<<map->getCursorY()<<std::endl;
			  if(!playerOne->moveTaken)
			  {
			  	playerOne->moveTaken = map->moveTile();
			  }
		}
		if(OIS::KC_SPACE  == evt.key)
		{
			  //map->selectUnit(map->getCursorX(),map->getCursorY());
			playerOne->moveTaken=true;
			playerOne->actionTaken=true;
		}
		if(OIS::KC_E == evt.key)
		{
			if(!playerOne->actionTaken)
			{ 
				std::string tmp = map->attackUnit(map->getCursorX(),map->getCursorY());
			  if(tmp != "")
			  {
			  	playerOne->actionTaken = true;
			  	gameInfo = tmp;
			  }
			}
		}
		if(OIS::KC_1  == evt.key)
		{
			  map->getSelectedUnit()->setDirection(Ogre::Vector3(0,0,1));
		}
		if(OIS::KC_2 == evt.key)
		{
			  map->getSelectedUnit()->setDirection(Ogre::Vector3(1,0,0));
		}
		if(OIS::KC_3 == evt.key)
		{
			  map->getSelectedUnit()->setDirection(Ogre::Vector3(0,0,-1));

		}
		if(OIS::KC_4 == evt.key)
		{
			  map->getSelectedUnit()->setDirection(Ogre::Vector3(-1,0,0));
		}
		if(OIS::KC_P == evt.key)
		{
			soundManager.play();
		}
		if(OIS::KC_L == evt.key)
		{
			soundManager.stop();
		}
	}

	return true;
}
bool GameManager::keyReleased( const OIS::KeyEvent& evt )
{
	switch(evt.key)
	{
		
	}
	return true;
}
