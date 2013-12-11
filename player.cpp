#include "player.h"

Player::Player()
{
	
}
void Player::initPlayer(Ogre::SceneManager *s,std::string name,std::string mesh,Ogre::Vector3 pos, Ogre::Vector3 initDir)
{	
	map = NULL;
  Stats war(Stats::WARRIOR);
  Stats arch(Stats::ARCHER);
  Stats mage(Stats::MAGE);
  Stats stats = war;
	for(int i=0;i<10;i++)
	{
		std::string newName = name;
		newName+=i;
		Unit * u = new Unit(s,this,newName,mesh,pos,initDir,stats);
		Weapon * w;
    if(stats.characterClass == Stats::WARRIOR)w = new Weapon(1, "Longsword", 500,5,1,Weapon::Physical);
    if(stats.characterClass == Stats::ARCHER)w = new Weapon(1, "Throwing Knife", 500,5,2,Weapon::Physical);
    if(stats.characterClass == Stats::MAGE)w = new Weapon(1, "Fireball", 500,5,3,Weapon::Magic);
		Armor * a = new Armor(4, "LeatherArmor", 400,20,0);
		u->equipWeapon(w);
		u->equipArmor(a);
		units.push_back(u);
		units[i]->getSceneNode()->setVisible(false);
		if(i>=5)battleUnits.push_back(units[i-5]);

    if(stats.characterClass == war.characterClass)stats = arch;
    else if(stats.characterClass == arch.characterClass)stats = mage;
    else if(stats.characterClass == mage.characterClass)stats = war;
	}
       
}
void Player::setOpp(Player *p)
{opp = p;}
Player * Player::getOpp()
{return opp;}
void Player::unitReady()
{
	moveTaken=false;
	actionTaken=false;
}
void Player::setMap(Map *m)
{
	map = m;
}
bool Player::makeMove(Unit *u,std::string &gameInfo)
{
  //std::cerr<<moveTaken<<" "<<actionTaken<<std::endl;
	return moveTaken && actionTaken;
}
std::vector<Unit *> & Player::getBattleUnits()
{
	return battleUnits;
}
