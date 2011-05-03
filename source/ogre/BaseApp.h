#ifndef _BaseApp_h_
#define _BaseApp_h_
#include "LoadingBar.h"
#include "../vdrift/settings.h"
#include <OIS/OIS.h>
#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>
#include "CarModel.h"
#include "SplitScreenManager.h"
using namespace Ogre;

namespace OISB {  class System;  };


class BaseApp :
		public Ogre::FrameListener, public Ogre::WindowEventListener,
		public OIS::KeyListener, public OIS::MouseListener
{
public:
	BaseApp();	virtual ~BaseApp();
	virtual void Run( bool showDialog );
	
	bool bLoading;
	
	// has to be in baseApp to switch camera on C press
	std::list<CarModel*> carModels;
	
	// translation
	// can't have it in c'tor, because mygui is not initialized
	virtual void setTranslations() = 0;
	
	SplitScreenManager* mSplitMgr;
	
	bool bWindowResized;  bool bSizeHUD;
	class HDRLogic* mHDRLogic;
	
	class SETTINGS* pSet;
	
	void recreateCompositor();

	SceneNode* ndSky; //-
	int roadUpCnt;
	LoadingBar mLoadingBar;

protected:
	bool mShowDialog, mShutDown;
	bool setup(), configure();  void updateStats();

	///  create
	virtual void createScene() = 0;
	virtual void destroyScene();

	void createCamera(), createFrameListener(), createViewports(), refreshCompositor();
	void setupResources(), createResourceListener(), loadResources();
	void LoadingOn(), LoadingOff();

	///  frame events
	bool frameRenderingQueued(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);
	virtual bool frameStart(Real time) = 0;
	virtual bool frameEnd(Real time) = 0;
	
	///  input events
	virtual bool keyPressed(const OIS::KeyEvent &arg);  bool keyReleased(const OIS::KeyEvent &arg);
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void windowResized(RenderWindow* rw), windowClosed(RenderWindow* rw);


	///  Ogre
	Root *mRoot;  SceneManager* mSceneMgr;
	RenderWindow* mWindow;

	///  input
	OISB::System* mOISBsys;
	OIS::InputManager* mInputManager;
public:
	OIS::Mouse* mMouse;  OIS::Keyboard* mKeyboard;
	bool isKey(OIS::KeyCode k)  {  return mKeyboard->isKeyDown(k);  }
	
	// this is set to true when the user is asked to assign a new key
	bool bAssignKey;
	OIS::KeyCode pressedKey;
	MyGUI::Widget* pressedKeySender;
protected:

	///  ovelay
	Overlay* mDebugOverlay, *mFpsOverlay;  // fps stats
	OverlayElement* mOvrFps, *mOvrTris, *mOvrBat, *mOvrDbg;

	bool alt, ctrl, shift;  // key modifiers
	bool mbLeft, mbRight, mbMiddle;  // mouse buttons
	String  mDebugText, mFilText;	// info texts
	bool mbWireFrame, mbShowCamPos;  // on/off


	///  Gui
	bool isFocGuiOrRpl()  {  return isFocGui || isFocRpl;  }
	bool isFocGui,isFocRpl;  // gui shown
	MyGUI::Gui* mGUI;		MyGUI::OgrePlatform* mPlatform;
	MyGUI::WidgetPtr mLayout, mWndOpts, mWndRpl;  // options window
	MyGUI::TabPtr mWndTabs;
};

#endif
