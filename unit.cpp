#include "unit.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
void Unit::equipArmor(Armor* a) 
{
  if(armor != NULL){
    stats.HP -= armor->BHP;
    stats.MP -= armor->BMP;
  }
  armor = a;
  stats.HP += a->BHP;
  stats.MP += a->BMP;
  stats.reset();
}
void Unit::equipWeapon(Weapon * w)
{
  weapon = w;
  stats.atkRange = w->range;
}
std::string Unit::getCharName()
{
  return charName;
}
void Unit::update(Ogre::Real time,Player *p1,Player *p2){}
Unit::Unit(Ogre::SceneManager *s,Player *owner,std::string name, std::string mesh, Ogre::Vector3 v,Ogre::Vector3 init,Stats stats):stats(stats)
{
  this->owner = owner;
  //Maybe add a list of names and assign it the name randomly?
  if(stats.characterClass == Stats::WARRIOR)charName = name+"warrior ";
  if(stats.characterClass == Stats::ARCHER)charName = name+"archer ";
  if(stats.characterClass == Stats::MAGE)charName = name+"mage ";
  
  sn = s->getRootSceneNode()->createChildSceneNode( name+"node",v);
  armor = NULL;
  weapon = NULL;
  entity = s->createEntity(name,mesh);
  sn->attachObject(entity);
  animState=IDLE;
  currAnim = NULL;
  //sn->setDirection(0,0,-1);
  x = 0;
  y = 0;
  initial_direction = init;
  yoffset = 0;
  if(mesh == "tux.mesh")
  {
    
    yoffset = 50;
    sn->scale(Ogre::Vector3(100,100,100));
    Ogre::Vector3 newpos = sn->getPosition();
    newpos.y=yoffset;
    sn->setPosition(newpos);
    Ogre::Entity *knife = s->createEntity(name+"knife","Cylinder.mesh");
    entity->attachObjectToBone("knife_bone", knife);
    Ogre::Material *oldmat = (entity->getSubEntity(0)->getMaterial()).getPointer();
    std::string matname = name+"classcolor";
    Ogre::Material *mat = (oldmat->clone(matname)).getPointer();
    Ogre::ColourValue classColor = mat->getTechnique(0)->getPass(0)->getDiffuse();
    if(stats.characterClass == Stats::WARRIOR)classColor+=Ogre::ColourValue(1,0,0);
    if(stats.characterClass == Stats::ARCHER)classColor+=Ogre::ColourValue(0,1,0);
    if(stats.characterClass == Stats::MAGE)classColor+=Ogre::ColourValue(0,0,1);
    mat->getTechnique(0)->getPass(0)->setDiffuse(classColor);
    entity->getSubEntity(0)->setMaterialName(matname);
    

  }
  if(mesh == "fishing_hole.mesh")
  {
    yoffset = -49;
    sn->scale(Ogre::Vector3(50,50,50));
    sn->pitch(Ogre::Radian(3.14));
    Ogre::Vector3 newpos = sn->getPosition();
    newpos.y=yoffset;
    sn->setPosition(newpos);
  }
  if(mesh == "bear.mesh")
  {
    
    yoffset = 0;
    sn->scale(Ogre::Vector3(15,15,15));
    Ogre::Vector3 newpos = sn->getPosition();
    newpos.y=yoffset;
    sn->setPosition(newpos);
    //sn->pitch(Ogre::Radian(3.14/2));
    
    for(int i=0; i < entity->getNumSubEntities(); i++)
    {
      Ogre::Material *oldmat = (entity->getSubEntity(i)->getMaterial()).getPointer();
      std::string matname = name+"classcolor";
      matname+=i;
      Ogre::Material *mat = (oldmat->clone(matname)).getPointer();
      Ogre::ColourValue classColor = mat->getTechnique(0)->getPass(0)->getDiffuse();
      if(stats.characterClass == Stats::WARRIOR)classColor+=Ogre::ColourValue(1,0,0);
      if(stats.characterClass == Stats::ARCHER)classColor+=Ogre::ColourValue(0,1,0);
      if(stats.characterClass == Stats::MAGE)classColor+=Ogre::ColourValue(0,0,1);
      mat->getTechnique(0)->getPass(0)->setDiffuse(classColor);
      entity->getSubEntity(i)->setMaterialName(matname);
    }
    
    

  }

  std::string filename = mesh+".anims";
  std::ifstream in(filename.c_str(), std::ifstream::in);
  in >> animations[IDLE];
  in >> animations[WALK];
  in >> animations[ATTACK];
  in >> animations[FLINCH];
  in >> animations[DEATH];
  in.close();

  if(animations[IDLE] != "None")currAnim = entity->getAnimationState(animations[IDLE]);
}
Ogre::Entity * Unit::getEntity() {return entity;}
Ogre::SceneNode * Unit::getSceneNode(){return sn;}
void Unit::move(int x,int y) //onMap
{
  sn->setPosition(x,yoffset,y);
}
Player * Unit::getOwner()
{
  return owner;
}
void Unit::reset()
{
  stats.reset();
  animState = NONE;
  setAnimState(IDLE);
  sn->setVisible(true);
}

Ogre::Vector3 Unit::getInitialDirection()
{
  return initial_direction;
}

void Unit::setInitialDirection(Ogre::Vector3 direction)
{
  this->initial_direction  = direction;	
}
void Unit::setDirection(Ogre::Vector3 direction)
{
  Ogre::Vector3 orig = sn->_getDerivedOrientation() * initial_direction;
  orig.y = 0;
  orig.normalise();
  direction.y = 0;
  direction.normalise();
  Ogre::Quaternion src = sn->_getDerivedOrientation();
  Ogre::Quaternion dest = orig.getRotationTo(direction);
  dest.normalise();
  if ((1.0f + orig.dotProduct(direction)) < 0.0001f)            // Work around 180 degree quaternion rotation quirk                         
  {
    sn->yaw(Ogre::Degree(180));
  }
  else
  {
    sn->rotate(dest);


  }
}
int Unit::getX(){return x;}
int Unit::getY(){return y;}

void Unit::setOrientation(Ogre::Quaternion q)
{
  orientation = q;
}
Ogre::Quaternion Unit::getOrientation()
{
  return orientation;
}
Ogre::Vector3 Unit::getDirection()
{
  return sn->_getDerivedOrientation() * initial_direction;
}
void Unit::update(Ogre::Real time)
{
  //std::cerr<<"Unit update begin"<<std::endl;
  if(currAnim)
    {
      switch(animState)
	{
	case IDLE:
	  currAnim = entity->getAnimationState(animations[IDLE]);
	  currAnim->setEnabled(true);
	  currAnim->setLoop(true);
	  currAnim->addTime(time);
	  break;

	case WALK:
	  currAnim = entity->getAnimationState(animations[WALK]);
	  currAnim->setEnabled(true);
	  currAnim->setLoop(true);
	  currAnim->addTime(time);
	  break;
	case NONE:
	  currAnim = NULL;
	  break;
	case ATTACK:
	  currAnim = entity->getAnimationState(animations[ATTACK]);
	  currAnim->setEnabled(true);
	  currAnim->setLoop(false);
	  currAnim->addTime(time);
	  if(currAnim->hasEnded())
	    {
	      currAnim->setTimePosition(0);
	      setAnimState(Unit::IDLE);
	    }
	  break;

		case FLINCH:
			currAnim = entity->getAnimationState(animations[FLINCH]);
			currAnim->setEnabled(true);
			currAnim->setLoop(false);
			currAnim->addTime(time);
			if(currAnim->hasEnded())
			{
				currAnim->setTimePosition(0);
				setAnimState(Unit::IDLE);
			}
			break;
		case DEATH:
			currAnim = entity->getAnimationState(animations[DEATH]);
			currAnim->setEnabled(true);
			currAnim->setLoop(false);
			currAnim->addTime(time);
			if(currAnim->hasEnded())
			{
        currAnim->setTimePosition(0);
        sn->setVisible(false);
			}
			break;
		}
	}
    
	
  //std::cerr<<"Unit update end"<<std::endl;
}
Unit::Anim_State Unit::getAnimState()
{
  return animState;
}
bool Unit::isAlive()
{
  return stats.currentHP > 0;
}
bool Unit::inRange(int x, int y)
{
  return (abs(this->x-x)+abs(this->y-y))<= stats.atkRange;
}
void Unit::setAnimState(Anim_State state)
{

	if(animState == DEATH || (animState == ATTACK && state == ATTACK))return;
	if(currAnim)
  {
    currAnim->setEnabled(false);
  }
	animState = state;

	
}

void Unit::setX(int x)
{
  this->x = x;
}
void Unit::setY(int y)
{
  this->y = y;
}
Unit::~Unit()
{
  delete armor;
  delete weapon;
}
std::string Unit::attacked(Unit *u,int h1, int h2)
{
  if(!isAlive() || u->animState == ATTACK)return "";
  //stats.Attack(u);
  std::stringstream ss;
  int damage = 0;
  int dcmod = 0;
  int dc;
  if(h1< h2)
    dcmod = 1;
  else if (h2 > h1)
    dcmod = -2;
  
  if(!u->weapon){
    damage = u->stats.PA * u->stats.PA * .7;
    std::cout<<"Attacked with fists for "<<damage<<std::endl;
  }
  else{
    switch(u->weapon->weaponType){
    case Weapon::Physical:
      damage = u->stats.PA * u->weapon->WP;
      dc = 4 + dcmod;
      if((rand() % 20) < dc) return "Attack missed";
      break;
    case Weapon::Speed:
      damage = ((u->stats.PA + u->stats.Sp)/2)*u->weapon->WP;
      dc = 3 + dcmod;
      if((rand() % 20) < dc) return "Attack missed";
      break;
    case Weapon::Magic:
      dc = 5 + dcmod;
      damage = u->stats.MA * u->weapon->WP;
      if((rand() % 20) < dc) return "Attack missed";
      break;
    }
    
  }
  stats.currentHP -= damage;
  std::cout<<"Attacked with "<<u->weapon->name<<" for "<<damage<<std::endl;
  std::cout<<"HP remaining "<<stats.currentHP<<"/"<<stats.HP<<std::endl;
  if(stats.currentHP <= 0)
    {
      setAnimState(Unit::DEATH);
    }else
    {
      setAnimState(Unit::FLINCH);
    }
  ss<<u->charName<<"attacked "<<charName<<" with "<<u->weapon->name<<" for "<<damage<<" damage ";

  return ss.str();
}
