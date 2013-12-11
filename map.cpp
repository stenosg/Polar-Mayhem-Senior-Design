#include "map.h"
#include "player.h"
#include <cmath>
#include <map>
#include <sstream>
#include <vector>
#include <list>

Map::Map(Ogre::SceneManager *s,std::string mesh,std::string mat,int maxX,int maxY,int tileW,int tileH, int tileD)
  :maxX(maxX),maxY(maxY),tileW(tileW),tileH(tileH),tileD(tileD)
{
  cursor = Ogre::Vector2(0,0);
  // Ogre::MeshManager::getSingleton().createPlane(mesh,
  //       Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
  //       tileW*maxX,tileH*maxY,maxX,maxY,true,1,5,5,Ogre::Vector3::UNIT_Z);
  currPath = NULL;
  currAnim = NULL;
  currUnit = NULL;
  currTile = NULL;
  animTime = 0;
  tiles = new Tiles*[maxX];
  for(int i=0; i < maxX; i++)
  {
    tiles[i] = new Tiles[maxY];
    normal.push_back(std::vector<Ogre::Material *>(maxY));
    highlight.push_back(std::vector<Ogre::Material *>(maxY));
  }
  for(int x=0; x < maxX; x++)
  {
    for(int y=0; y < maxY; y++)
    {
     std::string str = "map";
     str+=x;
     str+=y;
     tiles[x][y].initTile(s,str,mesh,mat, Ogre::Vector3(x*tileW,0,-y*tileH),x,y,rand() % 2,tileD);
     normal[x][y] = (tiles[x][y].getEntity()->getSubEntity(0)->getMaterial()).getPointer();
     highlight[x][y] = (normal[x][y]->clone(tiles[x][y].getEntity()->getName())).getPointer();
   }
 }
}
Ogre::Vector3 Map::mapToWorld(int x, int y)
{//+(0.5f*tileW)+(0.5f*tileH)

  return Ogre::Vector3((x*tileW),tiles[x][y].getH() * tileD,-((y*tileH)));
}
bool Map::addUnit(Unit *u,int x,int y)
{
  if(!tiles[x][y].hasUnit())
  {
    Ogre::Vector3 newpos = mapToWorld(x,y);
    newpos.y = newpos.y + u->yoffset;
    u->getSceneNode()->setPosition(newpos);
    u->setX(x);
    u->setY(y);
    tiles[x][y].u = u;
    return true;
  }
  else return false;
}
Map::~Map()
{
  for (int i = 0; i < maxX; ++i)delete [] tiles[i];
    delete [] tiles;
}
std::string Map::selectTile(int x,int y)
{
  if(x >= 0 && x < maxX && y>= 0 && y < maxY)
  {
      //std::cerr<<"Chose "<<x<<" "<<y<<std::endl;
    if(currTile)currTile->getSceneNode()->showBoundingBox(false);
    currTile = &tiles[x][y];
    currTile->getSceneNode()->showBoundingBox(true);
    std::stringstream ss;
    if(currTile->hasUnit() && currTile->u->isAlive())ss<<currTile->u->getCharName()<<" "<<currTile->u->stats.currentHP<<"/"<<currTile->u->stats.HP<<" atk range "<<currTile->u->stats.atkRange<<" move "<<currTile->u->stats.move;
    return ss.str();
  }
  return "";
}
bool Map::moveTile()
{
  return moveUnit(currUnit,currTile->getX(),currTile->getY());
}
int Map::manhattan(int srcx,int srcy,int dstx, int dsty)
{
  return abs(srcx-dstx)+abs(srcy-dsty);
}
bool Map::BFSIsIn(std::vector<Tiles*> &v,Tiles *t)
{
  for(int i=0; i < v.size(); i++)
  {
   if(v[i]->getX() == t->getX() && v[i]->getY() == t->getY())
   {
     //std::cerr<<t->getX()<<" "<<t->getY()<<"is in"<<std::endl;
    return true;
  }
}
  //std::cerr<<t->getX()<<" "<<t->getY()<<"is not in"<<std::endl;
return false;
}
std::vector<Tiles*> Map::unitMoveBFS(Unit *u)
{
  std::vector<Tiles*> colored;
  std::queue<Tiles*> q;
  q.push(&tiles[u->getX()][u->getY()]);
  int srcX = tiles[u->getX()][u->getY()].getX();
  int srcY = tiles[u->getX()][u->getY()].getY();
  while(!q.empty())
  {
    Tiles *t = q.front();
    q.pop();
    colored.push_back(t);
    int currX = t->getX();
    int currY = t->getY();
    if(manhattan(srcX, srcY, currX+1, currY) <= u->stats.move )
    {
      if((currX+1) >= 0 && (currX+1) < maxX && currY >= 0 && currY < maxY && !BFSIsIn(colored,&tiles[currX+1][currY]) && !(tiles[currX+1][currY].hasUnit() && manhattan(srcX, srcY, currX+1, currY) == u->stats.move))
      {
        if(!(tiles[currX+1][currY].hasUnit() && tiles[currX+1][currY].u->getOwner() != currUnit->getOwner()))q.push(&tiles[currX+1][currY]);
      }
    }
    if(manhattan(srcX, srcY, currX, currY+1) <= u->stats.move )
    {
      if(currX >= 0 && currX < maxX && (currY+1) >= 0 && (currY+1) < maxY && !BFSIsIn(colored,&tiles[currX][currY+1]) && !(tiles[currX][currY+1].hasUnit() && manhattan(srcX, srcY, currX, currY+1) == u->stats.move))
      {
        if(!(tiles[currX][currY+1].hasUnit() && tiles[currX][currY+1].u->getOwner() != currUnit->getOwner()))q.push(&tiles[currX][currY+1]);
      }
    }
    if(manhattan(srcX, srcY, currX-1, currY) <= u->stats.move )
    {
      if((currX-1) >= 0 && (currX-1) < maxX && currY >= 0 && currY < maxY && !BFSIsIn(colored,&tiles[currX-1][currY]) && !(tiles[currX-1][currY].hasUnit() && manhattan(srcX, srcY, currX-1, currY) == u->stats.move))
      {
        if(!(tiles[currX-1][currY].hasUnit() && tiles[currX-1][currY].u->getOwner() != currUnit->getOwner()))q.push(&tiles[currX-1][currY]);
      }
    }
    if(manhattan(srcX, srcY, currX, currY-1) <= u->stats.move )
    {
      if(currX >= 0 && currX < maxX && (currY-1) >= 0 && (currY-1) < maxY && !BFSIsIn(colored,&tiles[currX][currY-1]) && !(tiles[currX][currY-1].hasUnit() && manhattan(srcX, srcY, currX, currY-1) == u->stats.move))
      {
        if(!(tiles[currX][currY-1].hasUnit() && tiles[currX][currY-1].u->getOwner() != currUnit->getOwner()))q.push(&tiles[currX][currY-1]);
      }
    }
  }
  return colored;
}
bool Map::selectUnit(Unit *u)
{
  // std::cerr<<"Unit selected"<<std::endl;
  if(currUnit)
  {
    currUnit->getSceneNode()->showBoundingBox(false);
    //Decolor
    visitable = unitMoveBFS(currUnit);
    //std::cerr<<"got old  visitable list"<<std::endl;
    for(int i=0; i < visitable.size(); i++)
    {
      visitable[i]->getEntity()->setMaterialName(normal[visitable[i]->getX()][visitable[i]->getY()]->getName());
    }
    
  }
  currUnit = u;
  currUnit->getSceneNode()->showBoundingBox(true);
  //recolor
  visitable = unitMoveBFS(currUnit);
  //std::cerr<<"got new  visitable list"<<std::endl;
  for(int i=0; i < visitable.size(); i++)
  {
    //std::cerr<<"x "<<visitable[i]->getX()<<" y "<<visitable[i]->getY()<<std::endl;
    visitable[i]->getEntity()->setMaterialName(highlight[visitable[i]->getX()][visitable[i]->getY()]->getName());
    Ogre::ColourValue highColor(0,0,1);
    highlight[visitable[i]->getX()][visitable[i]->getY()]->getTechnique(0)->getPass(0)->setAmbient(highColor);
  }
  for(int i=0; i < currUnit->getOwner()->getOpp()->getBattleUnits().size();i++)
  {
    if(!currUnit->getOwner()->getOpp()->getBattleUnits()[i]->isAlive())continue;
    Tiles *tmpTile = &tiles[currUnit->getOwner()->getOpp()->getBattleUnits()[i]->getX()][currUnit->getOwner()->getOpp()->getBattleUnits()[i]->getY()];
      std::cout<<"Dist "<<manhattan(currUnit->getX(), currUnit->getY(), currUnit->getOwner()->getOpp()->getBattleUnits()[i]->getX(), currUnit->getOwner()->getOpp()->getBattleUnits()[i]->getY())<<" range "<<currUnit->stats.atkRange<<" Enemy Tile coords "<<tmpTile->getX()<<" "<<tmpTile->getY()<<" unit coords "<<currUnit->getX()<<" "<<currUnit->getY()<<std::endl;

    if(manhattan(currUnit->getX(), currUnit->getY(), currUnit->getOwner()->getOpp()->getBattleUnits()[i]->getX(), currUnit->getOwner()->getOpp()->getBattleUnits()[i]->getY()) <= currUnit->stats.atkRange)
    {
      
      visitable.push_back(tmpTile);
      tmpTile->getEntity()->setMaterialName(highlight[tmpTile->getX()][tmpTile->getY()]->getName());
      Ogre::ColourValue atkColor = Ogre::ColourValue(1,0,0)+highlight[tmpTile->getX()][tmpTile->getY()]->getTechnique(0)->getPass(0)->getAmbient();
      highlight[tmpTile->getX()][tmpTile->getY()]->getTechnique(0)->getPass(0)->setAmbient(atkColor);
    }
  }

  

  return true;

}

bool Map::selectUnit(int x, int y)
{
  if(currAnim || (currUnit && currUnit->getAnimState()==Unit::WALK))return false;
  if(x >= 0 && x < maxX && y>= 0 && y < maxY)
  {
    if(tiles[x][y].hasUnit() && tiles[x][y].u->isAlive())
    {
     selectUnit(tiles[x][y].u);
     return true;
   }
 }
 return false;
}

int Map::getMaxX()
{
  return maxX;
}
int Map::getMaxY()
{
  return maxY;
}
bool Map::moveUnit(Unit *u,int x,int y)
{
  std::cerr<<"src "<<u->getX()<<" "<<u->getY()<<" dst "<<x<<" "<<y<<std::endl;
  if(currAnim || (u->getX() == x && u->getY() == y) || (tiles[x][y].hasUnit() && tiles[x][y].u->isAlive()) || u->getAnimState() == Unit::WALK || (manhattan(u->getX(),u->getY(), x, y) > u->stats.move))return false;
  
  currPath = findPath(u->getX(),u->getY(),x,y);
  if(!currPath)
  {
    std::cerr<<"Couldn't find path"<<std::endl;
    return false;
  }
  currPath->pop();
  currUnit->setAnimState(Unit::WALK);
  tiles[currUnit->getX()][currUnit->getY()].u = NULL;
  currUnit->getSceneNode()->showBoundingBox(false);
  deColor();
  return true;

}


class SearchNode{
public:
  int x;
  int y;
  int goalX;
  int goalY;
  int cost;
  SearchNode *prev;
  Map *m;
  SearchNode():x(0),y(0),goalX(0),goalY(0),cost(0),prev(NULL),m(NULL){}
  SearchNode(int x,int y,int goalX,int goalY,int cost, SearchNode *prev,Map *m):x(x),y(y),goalX(goalX),goalY(goalY),cost(cost),prev(prev),m(m)
  {}
  int heuristic() const
  {
    return abs(x-goalX+y-goalY);
  }
  bool isGoal()
  {
    return x==goalX && y==goalY;
  }
  std::vector<SearchNode*> successors()
  {
    //add has unit logic
    std::cout<<"finding successors"<<std::endl;
    std::vector<SearchNode *> v;
    std::cout<<"potential successors "<<std::endl<<(x-1)<<" "<<y<<" "<<(x+1)<<" "<<y<<" "<<(x)<<" "<<(y-1)<<" "<<(x)<<" "<<(y+1)<<std::endl;
    if((x-1)>=0)
    {
      if(!m->tiles[x-1][y].hasUnit() || (m->tiles[x-1][y].hasUnit() && !m->tiles[x-1][y].u->isAlive()) || (m->tiles[x-1][y].hasUnit() && m->tiles[x-1][y].u->isAlive() && m->tiles[x-1][y].u->getOwner() == m->getSelectedUnit()->getOwner()))
      {
        SearchNode *n = new SearchNode(x-1,y,goalX,goalY,cost+1,this,m);
        v.push_back(n);
      }else
      {
       std::cout<<(x-1)<<" "<<y<<" has a unit"<<std::endl;
     }

   }
   if((x+1)<m->maxX)
   {
    if(!m->tiles[x+1][y].hasUnit() || (m->tiles[x+1][y].hasUnit() && !m->tiles[x+1][y].u->isAlive()) || (m->tiles[x+1][y].hasUnit() && m->tiles[x+1][y].u->isAlive() && m->tiles[x+1][y].u->getOwner() == m->getSelectedUnit()->getOwner()))
    {
      SearchNode *n = new SearchNode(x+1,y,goalX,goalY,cost+1,this,m);
      v.push_back(n);
    }else

    {

     std::cout<<(x+1)<<" "<<y<<" has a unit"<<std::endl;
   }

 }
 if((y-1)>=0)

 {
  if(!m->tiles[x][y-1].hasUnit() || (m->tiles[x][y-1].hasUnit() && !m->tiles[x][y-1].u->isAlive()) || (m->tiles[x][y-1].hasUnit() && m->tiles[x][y-1].u->isAlive() && m->tiles[x][y-1].u->getOwner() == m->getSelectedUnit()->getOwner()))

  {

   SearchNode *n = new SearchNode(x,y-1,goalX,goalY,cost+1,this,m);
   v.push_back(n);
 }else
 {
   std::cout<<(x)<<" "<<(y-1)<<" has a unit"<<std::endl;
 }

}
if((y+1)<m->maxY)

{
  if(!m->tiles[x][y+1].hasUnit() || (m->tiles[x][y+1].hasUnit() && !m->tiles[x][y+1].u->isAlive()) || (m->tiles[x][y+1].hasUnit() && m->tiles[x][y+1].u->isAlive() && m->tiles[x][y+1].u->getOwner() == m->getSelectedUnit()->getOwner()))
  {
    SearchNode *n = new SearchNode(x,y+1,goalX,goalY,cost+1,this,m);
    v.push_back(n); 
  }else

  {

   std::cout<<(x)<<" "<<(y+1)<<" has a unit"<<std::endl;
 }

}
std::cout<<"returning finding successors"<<std::endl;
return v;
}

bool isIn(std::vector<SearchNode *> &v)
{
  for(int i=0; i < v.size(); i++)
  {
   if(v[i]->x == x && v[i]->y == y)return true;
 }
 return false;
}

};

struct classcomp : public std::binary_function<SearchNode*,SearchNode*,bool> {
  bool operator() (const SearchNode * lhs,const SearchNode * rhs) const
  {return (lhs->cost+lhs->heuristic())>(rhs->cost+rhs->heuristic());}
};
/*
  struct mapcomp {
  bool operator() (const SearchNode& lhs, const SearchNode& rhs) const
  {return (lhs.cost+lhs.heuristic())<(rhs.cost+rhs.heuristic());}
  };
*/

  std::queue<Ogre::Vector2> * Map::findPath(int srcx,int srcy,int dstx, int dsty)
  {
    std::cout<<"starting findpath"<<std::endl;
    std::priority_queue<SearchNode*,std::vector<SearchNode*>,classcomp> frontier;
    std::vector<SearchNode *> explored;
    SearchNode *start = new SearchNode(srcx,srcy,dstx,dsty,-1,NULL,this);
    frontier.push(start);
    SearchNode *curr = NULL;
    while(!frontier.empty())
    {

      curr = frontier.top();
      frontier.pop();
      if(!(curr->cost <= currUnit->stats.move))
      {
        SearchNode *ptr = curr->prev;
        delete curr;
        while(ptr != NULL)
        {
          curr = ptr;
          ptr = curr->prev;
          delete curr;
          curr = NULL;
        }
        break;
      }
      std::cout<<"popped"<<curr->x<<" "<<curr->y<<std::endl;
      if(curr->isGoal())
      {
       std::cout<<"found goal ("<<curr->x<<","<<curr->y<<")"<<std::endl;
       break;
     }
     explored.push_back(curr);
     std::vector<SearchNode*> next = curr->successors();
     for(int i=0; i < next.size(); i++)
     {
       std::cout<<"is "<<next[i]->x<<" "<<next[i]->y<<" explored"<<std::endl;
       if(!next[i]->isIn(explored))
       {
         std::cout<<"it is not!"<<std::endl;
         frontier.push(next[i]);
       }
     }
     curr = NULL;

   }
   std::cout<<"cleaning"<<std::endl;
  //Cleanup
   while(!frontier.empty())
   {
      //std::cout<<"cleaning frontier"<<std::endl;
      //stuff still on frontier needs to be deleted
    SearchNode *tmp = frontier.top();
    frontier.pop();
    delete tmp;
  }
  std::queue<Ogre::Vector2> *ret = NULL;
  if(curr != NULL)
  {
    ret = new std::queue<Ogre::Vector2>();
    std::list<SearchNode *> l;
    l.push_front(curr);
    SearchNode *ptr = curr->prev;
    while(ptr != NULL)
    {
     l.push_front(ptr);
     ptr = ptr->prev;
   }
   while(l.size()>0)
   {
     ptr = l.front();
     l.pop_front();
	  //std::cout<<"pushing "<<ptr->x<<" "<<ptr->y<<std::endl;
     ret->push(Ogre::Vector2(ptr->x,ptr->y));

     delete ptr;
   }
 }

 return ret;

}

Ogre::Vector2 Map::worldToMap(const Ogre::Vector3 &v)
{
  int x = (v.x/(Ogre::Real)tileW);
  int y = -(v.z/(Ogre::Real)tileH);
  return Ogre::Vector2(x,y);
}
Ogre::Vector3 lerp (Ogre::Vector3 &srcLocation, Ogre::Vector3 &destLocation, Ogre::Real Time)
{
  //std::cerr<<srcLocation<<" "<<destLocation<<" "<<Time<<std::endl;
  Ogre::Vector3 l = srcLocation + (destLocation - srcLocation) * Time;
  l.y = 4*l.y;
  if(l.y > destLocation.y)
    l.y = destLocation.y;
  return l;
}

void Map::update(Ogre::SceneManager *sm,Ogre::Real time)
{
 //std::cerr<<"Map update begin"<<std::endl;

 if(currPath){
   //std::cerr<<"Path Exists"<<std::endl;
    // currAnim = sm->getAnimationState(Unit::WALK);
   
   Ogre::Vector3 current( mapToWorld(currUnit->getX(),currUnit->getY()));
   current.y = current.y + currUnit->yoffset;
   Ogre::Vector3 next;
   Ogre::Vector3 orig = currUnit->getSceneNode()->_getDerivedOrientation() * currUnit->getInitialDirection();
   orig.y = 0;
   orig.normalise();
   //std::cout<<orig<<std::endl;
   if(currPath->empty()){
    //std::cerr<<"Path Empty"<<std::endl;
    delete currPath;
    //std::cerr<<"Deleted current path"<<std::endl;
    currPath = NULL;

    currUnit->setAnimState(Unit::IDLE);
    //std::cerr<<"Set animstate to idle"<<std::endl;
    int unitx = currUnit->getX();
    int unity = currUnit->getY();
    tiles[unitx][unity].u = currUnit;
    currUnit = NULL;
    selectUnit(unitx,unity);
  }
  else{
    //std::cerr<<"Path Not Empty"<<std::endl;
    next = Ogre::Vector3( mapToWorld((int)currPath->front().x,(int)currPath->front().y));
    next.y = next.y + currUnit->yoffset;
    Ogre::Vector3 dir(next - current);
    dir.y=0;
    dir.normalise();
    Ogre::Quaternion src = currUnit->getSceneNode()->getOrientation();
    Ogre::Quaternion dest = orig.getRotationTo(dir) * src;
    dest.normalise();
    if ((1.0f + orig.dotProduct(dir)) < 0.0001f)            // Work around 180 degree quaternion rotation quirk                         
    {
      currUnit->getSceneNode()->yaw(Ogre::Degree(180));
      animTime = 1.0;
    }
    
    animTime+= time;
    if(animTime >= 1.0){
     animTime = 0;
     //std::cerr<<next<<std::endl;
     currUnit->getSceneNode()->setPosition(next);
     std::cerr<<"set pos"<<std::endl;
     //tiles[currUnit->getX()][currUnit->getY()].u= NULL;
     Ogre::Vector2 f = currPath->front();
     //tiles[(int)f.x][(int)f.y].u = currUnit;
     currUnit->setX(currPath->front().x);
     currUnit->setY(currPath->front().y);

     currPath->pop();
     std::cout<<"Popped "<<f<<std::endl;
   }
   else{
     //std::cerr<<lerp(current,next,animTime)<<std::endl;
     currUnit->getSceneNode()->setPosition(lerp(current,next,animTime));
     //std::cerr<<"set pos"<<std::endl;
     currUnit->getSceneNode()->setOrientation(Ogre::Quaternion::Slerp(animTime,src,dest));
   }	
   currUnit->update(time);
 }

}

//std::cout<<"Start tile updates:"<<std::endl;
for(int i=0; i < maxX; i++)
{
  for(int j=0; j < maxY; j++)
  {
   if(tiles[i][j].hasUnit())
   {
	      // std::cout<<"has unit "<<i<<" "<<j<<std::endl;  
     if(tiles[i][j].u->getOwner())tiles[i][j].u->update(time);

   }
 }
}
//std::cerr<<"Map update end"<<std::endl;

}


std::string Map::attackUnit(int x, int y)
{
  if(currAnim || !tiles[x][y].hasUnit() || (currUnit->getX() == x && currUnit->getY() == y) || !tiles[x][y].u->isAlive() || !currUnit->inRange(x,y) || currUnit->getAnimState() == Unit::WALK)return "";
  std::cerr<<"Attacking"<<std::endl;
  Ogre::Vector3 srcToDst = currUnit->getSceneNode()->getPosition() - tiles[x][y].u->getSceneNode()->getPosition();
  srcToDst.normalise();
  currUnit->setDirection(-srcToDst);
  //currUnit->updateDirection();
  tiles[x][y].u->setDirection(srcToDst);
  //tiles[x][y].u->updateDirection();
  std::string ret = tiles[x][y].u->attacked(currUnit,tiles[x][y].getH(),tiles[currUnit->getX()][currUnit->getY()].getH());
  currUnit->setAnimState(Unit::ATTACK);
  return ret;
}
std::string Map::castSpell(int x, int y , const Ability& a)
{
  if(currAnim || currUnit->getAnimState() == Unit::WALK||manhattan(currUnit->getX(),currUnit->getY(),x,y) <= a.range)return "";
  if(currUnit->getSceneNode()->getPosition() !=  tiles[x][y].u->getSceneNode()->getPosition()){
    Ogre::Vector3 srcToDst = currUnit->getSceneNode()->getPosition() - tiles[x][y].u->getSceneNode()->getPosition();
    srcToDst.normalise();
    currUnit->setDirection(-srcToDst);
    for(int i = 0; i< maxX;++i)
      for(int j = 0; j<maxY;++j){
	if(tiles[i][j].hasUnit())
	  if(manhattan(x,y,i,j) <= a.area)
	    tiles[i][j].u->stats.Spell(a);
      }
  }
  
}

Unit * Map::getSelectedUnit()
{
  return currUnit;
}
Tiles * Map::getSelectedTile()
{
  return currTile;
}
void Map::deColor()
{
  //Decolor
  for(int i=0; i < visitable.size(); i++)
  {
    visitable[i]->getEntity()->setMaterialName(normal[visitable[i]->getX()][visitable[i]->getY()]->getName());
  }
  visitable.clear();
}
int Map::getCursorX()
{
  return cursor.x;
}
int Map::getCursorY()
{
  return cursor.y;
}
void Map::setCursorX(int x)
{
  cursor.x = x;
}
void Map::setCursorY(int y)
{
  cursor.y = y;
}
int Map::getHeight(int x, int y)
{
  return tiles[x][y].getH();
}
