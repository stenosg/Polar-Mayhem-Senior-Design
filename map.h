#ifndef _MAP_H_
#define _MAP_H_
#include "unit.h"
#include "tile.h"
#include <queue>
#include <vector>
class Tiles;

class Map
{
 private:
  int maxX;
  int maxY;
  int tileW;
  int tileH;

  int tileD;
  Ogre::Real animTime;
  Tiles *currTile;
  Tiles **tiles; //array of tiles,our grid //access tiles using tiles[i][j]
  std::vector<Tiles*> visitable;
  std::queue<Ogre::Vector2> *currPath;
  Unit *currUnit;
  Ogre::AnimationState*  currAnim;
  Ogre::Vector2 cursor;
  std::vector< std::vector<Ogre::Material*> > normal;
  std::vector< std::vector<Ogre::Material*> > highlight;
  
  
  friend class SearchNode;
  
 public:
  Map(Ogre::SceneManager *s,std::string mesh,std::string mat,int maxX,int maxY,int tileW,int tileH,int tileD);
  ~Map();
  Ogre::Vector2 worldToMap(const Ogre::Vector3 &v);
  Ogre::Vector3 mapToWorld(int x, int y);
  std::vector<Tiles*> unitMoveBFS(Unit *u);
  int manhattan(int srcx,int srcy,int dstx, int dsty);
  bool BFSIsIn(std::vector<Tiles*> &v,Tiles *t);
  bool moveUnit(Unit *u,int x,int y);
  bool addUnit(Unit *u,int x,int y);
  std::string selectTile(int x,int y);
  bool selectUnit(Unit *u);
  bool selectUnit(int x, int y);
  std::string attackUnit(int x, int y);
  bool moveTile();
  Tiles * getSelectedTile();
  Unit * getSelectedUnit();
  std::queue<Ogre::Vector2> *findPath(int srcx,int srcy,int dstx, int dsty); //Vector2 corresponds to x,y index in tiles array
  void update(Ogre::SceneManager *sm,Ogre::Real time);
  int getMaxX();
  int getMaxY();
  int getCursorX();
  int getCursorY();
  void deColor();
  void setCursorX(int x);
  void setCursorY(int y);
  int getHeight(int x, int y);
  std::string castSpell(int x, int y, const Ability & a);


};

#endif
