#include <Ogre.h>

#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUIWindow.h>
//#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include "OgreCEGUIRenderer.h"
#include "OgreText.h"
#include <CEGUI/elements/CEGUIPushButton.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include "unit.h"
#include "tile.h"
#include "map.h"
#include "gamemanager.h"

#include <iostream>
using namespace std;

//#include "ExampleFrameListener.h"

using namespace Ogre;

class MyListener : public FrameListener, public OIS::MouseListener, public OIS::KeyListener
{

public:
  MyListener(OIS::Keyboard *keyboard,OIS::Mouse *mouse, Root *mRoot) : mKeyboard(keyboard),mMouse(mouse)
  {
    mWindow = mRoot->getAutoCreatedWindow();
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
    this->mRoot = mRoot;
    shutdown = false;
    up = false;
    down = false;
    left = false;
    start_menu = true;

    startText = new OgreText;
    titleText = new OgreText;

    storyText1 = new OgreText;
    storyText2 = new OgreText;
    storyText3 = new OgreText;
    storyText4 = new OgreText;



                

    amb_l = 0.4; //original ambient
    sp_dif_l = 0.3;  //original specular and diffuse
    f_cntr = 0; // Count for fade in
    total_time = 10; // Total_time it will take for screen to fade back into battle map
    crnt_amb = 0; // current ambient value
    crnt_spdif = 0; 	
    
    right = false;
    zoomIn = false;
    zoomOut = false;
  }

  void initialize()
  {
    //Create the scene manager
    mSceneMgr = mRoot->getSceneManager("Default SceneManager");
    mSceneMgr->setAmbientLight(ColourValue(0, 0, 0));
    mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);

		
    //Setup the camera
    Camera *mCamera = mSceneMgr->createCamera("MyCamera");
    mCamera->setNearClipDistance(5);
    Viewport *mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);

    //			Camera *mCamera = mRoot->getSceneManager("Default SceneManager")->getCamera("MyCamera");
    mCamera->setPosition(-800,600,0);
    mCamera->lookAt(200,0,-200);
    cameraIndex = 0;
    firstPerson = false;
    // TEST PLANE
    /*
      Plane plane(Vector3::UNIT_Y, 0);
      MeshManager::getSingleton().createPlane("ground",
      ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
      150000,150000,20,20,true,1,5,5,Vector3::UNIT_Z);
      Entity* ent = mSceneMgr->createEntity("GroundEntity", "ground");
      ent->setMaterialName("OceanHLSL_GLSL");
      ent->setCastShadows(false);

      SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
      node->attachObject(ent);
      node->translate(0,0,0);
    */
    int xAmount = 8;
    int yAmount = 8;
    int width = 100;
    int height = 100;
    int depth = 25;
    cameraSpeed = 5;
    cameraPositions[0] = Vector3(-100,500,500);
    cameraPositions[1] = Vector3(1300,500,700);
    cameraPositions[2] = Vector3(1300,500,-700);
    cameraPositions[3] = Vector3(-100,500,-500);
    gameManager = new GameManager(mSceneMgr);
    //hack for txt bug
    //mRoot->renderOneFrame();
    gameManager->generateMap(mSceneMgr,"ground","SnowFloor",xAmount,yAmount,width,height,depth);
    mapCenter = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CameraCenter",Vector3((xAmount*width)/2,0,(-yAmount*height)/2));
    cameraNode = mapCenter->createChildSceneNode("CameraNode",mCamera->getPosition());
    mCamera->setAutoTracking(true,mapCenter);
    cameraNode->attachObject(mCamera);
    cameraNode->setPosition(cameraPositions[cameraIndex]);
    // Light
    mSceneMgr->setAmbientLight(ColourValue(0, 0, 0));
    light = mSceneMgr->createLight("Light1");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDirection(Vector3(-1, -1, -1));
    light->setPosition(Vector3(0, 25000, 25000));
    light->setDiffuseColour(0, 0, 0);
    light->setSpecularColour(0, 0, 0);

    // FOG
    ColourValue fadeColour(0.2, 0.2, 0.2);
    mViewport->setBackgroundColour(fadeColour);
    mSceneMgr->setFog(FOG_LINEAR, fadeColour, 0.0, 500, 50000);



    //SKYBOX
    mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 5000, false);
  }
  bool keyPressed(const OIS::KeyEvent& evt)
  {
    Camera *mCamera = mRoot->getSceneManager("Default SceneManager")->getCamera("MyCamera");
    switch(evt.key)
      {
      case OIS::KC_ESCAPE:
      shutdown = true;
      delete startText; //memory leaks otherwise
      delete titleText;

      delete storyText1; //memory leaks otherwise
      delete storyText2;
      delete storyText3; //memory leaks otherwise
      delete storyText4;
      break;

      case OIS::KC_O:
      start_menu = false;
				
      startText->setText("");
      titleText->setText("");

      storyText1->setText("");
      storyText2->setText("");
      storyText3->setText("");
      storyText4->setText("");
      break;
      case OIS::KC_UP:
	up = true;
	break;
      case OIS::KC_DOWN:
	down = true;
	break;	
      case OIS::KC_LEFT:
	left = true;
	break;
      case OIS::KC_RIGHT:
	right = true;
	break;
      case OIS::KC_SUBTRACT:
	zoomOut = true;
	break;
      case OIS::KC_ADD:
	zoomIn = true;
	break;
				
      case OIS::KC_F1:
	firstPerson = true;
	break;
      case OIS::KC_F2:
	mCamera->setPosition(-800,600,0);
	mCamera->lookAt(200,0,-200);
	firstPerson = false;
	break;
      case OIS::KC_F4:
          
	break;
      default:
	return gameManager->keyPressed(evt);
      }


    return true;
  }
  bool keyReleased( const OIS::KeyEvent& evt )
  {

    switch(evt.key)
      {
       case OIS::KC_ESCAPE:
        shutdown = true;
        delete startText; //memory leaks otherwise
        delete titleText;

        delete storyText1; //memory leaks otherwise
        delete storyText2;
        delete storyText3; //memory leaks otherwise
        delete storyText4;
        break;

        case OIS::KC_O:
        start_menu = false;
				
        startText->setText("");
        titleText->setText("");

        storyText1->setText("");
        storyText2->setText("");
        storyText3->setText("");
        storyText4->setText("");
        break;
      case OIS::KC_UP:
	up = false;
	break;
      case OIS::KC_DOWN:
	down = false;
	break;	
      case OIS::KC_LEFT:
	cameraIndex--;
	if(cameraIndex < 0)cameraIndex=3;
	cameraNode->setPosition(cameraPositions[cameraIndex]);
	gameManager->cameraChanged(cameraIndex);
	left = false;
	break;
      case OIS::KC_RIGHT:
	right = false;
	cameraIndex++;
	if(cameraIndex > 3)cameraIndex=0;
	cameraNode->setPosition(cameraPositions[cameraIndex]);
	gameManager->cameraChanged(cameraIndex);
	break;
      case OIS::KC_SUBTRACT:
	zoomOut = false;
	break;
      case OIS::KC_ADD:
	zoomIn = false;
	break;
				
      default:
	return gameManager->keyReleased(evt);
      }
    return true;
  }
  bool mouseMoved( const OIS::MouseEvent& evt )
  {
    std::cerr<<"Mouse "<<evt.state.X.abs<<" "<<evt.state.Y.abs<<std::endl;
    float x = ((evt.state.X.abs)/50.0)*mWindow->getWidth();
    float y = ((evt.state.Y.abs)/50.0)*mWindow->getHeight();
    CEGUI::System::getSingleton().injectMousePosition(x,y);
    return true;
  }
  bool mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id )
  {
    return true;

  }
  bool mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){return true;}
		
  bool frameStarted(const FrameEvent& evt)
  {
    //get keyboard input
    mKeyboard->capture();
    mMouse->capture();
    //std::cerr<<"Current Index = ("<<x<<","<<y<<")"<<std::endl;
    gameManager->update(mSceneMgr,evt.timeSinceLastFrame);

    //Close the window
    if( mWindow->isClosed() || shutdown)
      return false;



    if(start_menu)
    {
         titleText->setText("POLAR MAYHEM")
         titleText->setCol(1.0f,1.0f,1.0f,1.0); 
         titleText->setPos(0.4f,0.125f);


         storyText1->setText("The Pirate Penguins stormed into Siberia, taking all that they saw as their own.");
         storyText1->setCol(1.0f,1.0f,1.0f,1.0);
         storyText1->setPos(0.03f,0.2f);

         storyText2->setText("Initially, the local bear inhabitants were wiped out by the fearsome penguins.");
         storyText2->setCol(1.0f,1.0f,1.0f,1.0);
         storyText2->setPos(0.03f,0.275f);

         storyText3->setText("A group of special Siberian Communist Bears is the last hope for the Motherland!!");
         storyText3->setCol(1.0f,1.0f,1.0f,1.0);
         storyText3->setPos(0.03f,0.350f);

          storyText4->setText("Lead your team to victory, massacring any despicable Pirate Penguin on your way!");
          storyText4->setCol(1.0f,1.0f,1.0f,1.0);
	  storyText4->setPos(0.03f,0.425f);

          startText->setText("Press 'O' to commence the carnage!");
          startText->setPos(0.35f,0.85f);        
           startText->setCol(1.0f,1.0f,1.0f,1.0);
 
	}	

                
               if(!start_menu)
		  {
                     mSceneMgr = mRoot->getSceneManager("Default SceneManager");
		     mSceneMgr->setAmbientLight(ColourValue(0.5f, 0.5f, 0.5f));
		     mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);  

		     startText->setText("");
				titleText->setText("");

		     storyText1->setText("");
		     storyText2->setText("");
		     storyText3->setText("");
		     storyText4->setText("");

		  }



                if(!start_menu)
               {
		  mSceneMgr->setAmbientLight(ColourValue(0.4, 0.4, 0.4));

	          light->setType(Light::LT_DIRECTIONAL);
		  light->setDirection(Vector3(-1, -1, -1));
		  light->setPosition(Vector3(0, 25000, 25000));
		  light->setDiffuseColour(0.3, 0.3, 0.3);
		  light->setSpecularColour(0.3, 0.3, 0.3);

		  startText->setText("");
		  titleText->setText("");

		  storyText1->setText("");
		  storyText2->setText("");
		  storyText3->setText("");
		  storyText4->setText("");

               }





    Camera *mCamera = mRoot->getSceneManager("Default SceneManager")->getCamera("MyCamera");
    Vector3 dirToCenter;
    if(up)
      {
	cameraNode->setPosition(cameraNode->getPosition()+Vector3(0,cameraSpeed,0));
				
      }
    if(down)
      {
	cameraNode->setPosition(cameraNode->getPosition()+Vector3(0,-cameraSpeed,0));
      }
    if(left)
      {
	//mapCenter->yaw(Radian(cameraSpeed/20.0));
				
      }
    if(right)
      {
	//mapCenter->yaw(Radian(-cameraSpeed/20.0));
				
      }
    if(zoomIn)
      {
	dirToCenter = mapCenter->getPosition()-cameraNode->getPosition();
	dirToCenter.normalise();
	std::cerr<<dirToCenter<<std::endl;
	dirToCenter = dirToCenter*cameraSpeed;
	cameraNode->setPosition(cameraNode->getPosition()+dirToCenter);
      }
    if(zoomOut)
      {
	dirToCenter = mapCenter->getPosition()-cameraNode->getPosition();
	dirToCenter.normalise();
	std::cerr<<dirToCenter<<std::endl;
	dirToCenter = -dirToCenter*cameraSpeed;
	cameraNode->setPosition(cameraNode->getPosition()+dirToCenter);
      }
			
			
    return true;
  }

private:
  SceneNode *mapCenter;
  SceneNode *cameraNode;
  GameManager *gameManager;
  CEGUI::System *mSystem;
  OIS::Keyboard *mKeyboard;
  RenderWindow *mWindow;
  OIS::Mouse *mMouse;
  SceneManager *mSceneMgr;
  Root* mRoot;
  Light *light;
  

  float amb_l; //original ambient
  float sp_dif_l;  //original specular and diffuse
  float f_cntr; // Count for fade in 
  float total_time; // Total_time it will take for screen to fade back into battle map
  float crnt_amb; // current ambient value
  float crnt_spdif; // current specular and diffuse values


  OgreText *startText;
  OgreText *titleText;
  OgreText *storyText1;
  OgreText *storyText2;
  OgreText *storyText3;
  OgreText *storyText4;


  bool start_menu;
  bool fader;
  bool can_calculate
  Vector3 cameraPositions[4];
  int cameraIndex;
  bool firstPerson;
  bool shutdown;
  bool left;
  bool right;
  bool up;
  bool down;
  bool zoomIn;
  bool zoomOut;
  int cameraSpeed;

};

class MyApplication
{
public:
  MyApplication()
  {
    createRoot();
    defineResources();
    setupRenderSystem();
    createRenderWindow();
    initializeResourceGroups();
    setupScene();
    setupInputSystem();
    setupCEGUI();
    setupGame();
    createFrameListener();
    startRenderLoop();
  }

  ~MyApplication()
  {
    mInputManager->destroyInputObject(mKeyboard);
    OIS::InputManager::destroyInputSystem(mInputManager);

    //delete mRenderer;
    //delete mSystem;
    delete mRoot;
  }

private:
  Root *mRoot;
  OIS::Keyboard *mKeyboard;
  OIS::Mouse *mMouse;
  OIS::InputManager *mInputManager;
  //CEGUI::OgreRenderer *mRenderer;
  CEGUI::OgreCEGUIRenderer *mRenderer;
  CEGUI::System *mSystem;

  MyListener *mMyListener;



  void createRoot()
  {
    mRoot = new Root();
  }

  void defineResources()
  {
    String secName, typeName, archName;
    ConfigFile cf;
    cf.load("resources.cfg");

    ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
      {
	secName = seci.peekNextKey();
	ConfigFile::SettingsMultiMap *settings = seci.getNext();
	ConfigFile::SettingsMultiMap::iterator i;
	for (i = settings->begin(); i != settings->end(); ++i)
	  {
	    typeName = i->first;
	    archName = i->second;
	    ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
	  }
      }
  }

  void setupRenderSystem()
  {
    if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
      {
	throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
      }
  }

  void createRenderWindow()
  {
    mRoot->initialise(true, "Polar Mayhem");
  }

  void initializeResourceGroups()
  {
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
			
  }

  void setupScene()
  {
    SceneManager *mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "Default SceneManager");
  }

  void setupInputSystem()
  {
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    OIS::ParamList pl;
    RenderWindow *win = mRoot->getAutoCreatedWindow();

    win->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    mInputManager = OIS::InputManager::createInputSystem(pl);
    try
      {
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
	//mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, false));
      }
    catch (const OIS::Exception &e)
      {
	throw new Exception(42, e.eText, "Application::setupInputSystem");
      }
  }

  void setupCEGUI()
  {
    SceneManager *mgr = mRoot->getSceneManager("Default SceneManager");
    RenderWindow *win = mRoot->getAutoCreatedWindow();


    // CEGUI setup
    // mRenderer = &CEGUI::OgreRenderer::create();
    //     	CEGUI::System::create(*mRenderer);
    //     	// Initialisation Area
    //     	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
    // 			rp->setResourceGroupDirectory("schemes", "./media/gui/");
    // 			rp->setResourceGroupDirectory("imageset", "./media/gui/");
    // 			rp->setResourceGroupDirectory("fonts", "./media/gui/");
    // 			rp->setResourceGroupDirectory("looknfeel", "./media/gui/");
    // 			CEGUI::Scheme::setDefaultResourceGroup("schemes");
    // 			CEGUI::Imageset::setDefaultResourceGroup("imageset");
    // 			CEGUI::Font::setDefaultResourceGroup("fonts");
    // 			CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
    // 		  CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative); // this is recommended to help with debugging, but not neccessary
    // 	    CEGUI::SchemeManager::getSingleton().create((CEGUI::utf8*)"TaharezLookSkin.scheme");
    // 	    CEGUI::System::getSingleton().setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    // 	    CEGUI::Window* mRootWindow= CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultWindow", (CEGUI::utf8*)"RootWindow");  
    // 	    CEGUI::System::getSingleton().setGUISheet(mRootWindow); // set active Window*/

	    
    mRenderer = new CEGUI::OgreCEGUIRenderer(win, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mgr);
    mSystem = new CEGUI::System(mRenderer);
			
    CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative); // this is recommended to help with debugging, but not neccessary
    CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
    CEGUI::System::getSingleton().setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    CEGUI::Window* mRootWindow= CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultWindow", (CEGUI::utf8*)"RootWindow");  
    CEGUI::System::getSingleton().setGUISheet(mRootWindow); // set active Window
			
  }

  void setupGame()
  {
    //Graphical Stuff
    RenderWindow *mWindow = mRoot->getAutoCreatedWindow();
  }

  void createFrameListener()
  {
    mMyListener = new MyListener(mKeyboard,mMouse,mRoot);
    mRoot->addFrameListener(mMyListener);
    mMyListener->initialize();
  }

  void startRenderLoop()
  {
    mRoot->startRendering();
  }
};





#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
  int main(int argc, char **argv)
#endif
{
  try
    {
      MyApplication game;
    }
  catch(Exception& e)
    {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      fprintf(stderr, "An exception has occurred: %s\n", e.getFullDescription().c_str());
#endif
    }

  return 0;
}

