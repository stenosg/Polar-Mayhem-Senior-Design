 #include "unit.h"
 #include "map.h"
 #include "w_map.h"
 #include "player.h"
 #include "aiplayer.h"
 #include "textrenderer.h"
 #include "soundmanager.h"
 #include "fishinghole.h"
 #include <OIS/OIS.h>
 #include <OGRE/Ogre.h>
 #include <vector>

 class GameManager
 {
  private:
   Map *map; 
 //  Map *map;

    enum Game_State{WORLD,BATTLE}gameState;
    int playerTurnSpeed;
    Player *playerOne;
    Player *playerTwo;
   Unit *fishingHole;
    TextRenderer *textRenderer;
    Unit *currUnit;
    std::queue<Unit *> readyUnits;
   std::queue<Unit *> readyWorldUnits;
    std::vector<Unit *> activeUnits;
   std::vector<Unit *> activeWorldUnits;
    OIS::KeyCode leftKey;
    OIS::KeyCode rightKey;
    OIS::KeyCode upKey;
    OIS::KeyCode downKey;
   SoundManager soundManager;
   std::string gameInfo;
    void makeUnitReady();
   std::string checkVictory();
  
  public:
   GameManager(Ogre::SceneManager *s);
   void update(Ogre::SceneManager *sm,Ogre::Real time);
  
   bool keyReleased( const OIS::KeyEvent& evt );
   bool keyPressed( const OIS::KeyEvent& evt );
  void cameraChanged(int index);
  void generateMap(Ogre::SceneManager *s,std::string mesh,std::string mat,int maxX,int maxY,int tileW,int tileH,int tileD);
 //bool keyPressedWorld( const OIS::KeyEvent& evt );
  //void generateWorldMap(Ogre::SceneManager *s,std::string mesh,std::string mat,int maxX,int maxY,int tileW,int tileH);
 
 
 };
