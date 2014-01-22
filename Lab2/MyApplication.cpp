#include "Ogre\Ogre.h"
#include "MyFrameListener.cpp"

using namespace Ogre;

class MyApplication
{

private:
	SceneManager* _sceneManager;
	Root* _root;
	MyFrameListener* _listener;
	bool _keepRunning;

public:
	// Constructor
	MyApplication()
	{
		_sceneManager = NULL;
		_root = NULL;
		_listener = NULL;
	}

	// Destructor
	~MyApplication()
	{
		delete _root;
		delete _listener;
	}

	// Getters and setters
	bool keepRunning()
	{
		return _keepRunning;
	}

	void loadResources()
	{
		ConfigFile cf;
		cf.load("resources_d.cfg");
		ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();
		String sectionName, typeName, dataName;

		while (sectionIter.hasMoreElements())
		{
			sectionName = sectionIter.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = sectionIter.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				dataName = i->second;
				ResourceGroupManager::getSingleton().addResourceLocation(dataName, typeName, sectionName);
			}
		}
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}

	int startup()
	{
		_root = new Root("plugins_d.cfg");
		
		if (!_root->showConfigDialog()) {
			return -1;
		}

		RenderWindow* window = _root->initialise(true, "Ogre3D Beginners Guide");
		_sceneManager = _root->createSceneManager(ST_GENERIC);

		_listener = new MyFrameListener(window);
		_root->addFrameListener(_listener);

		Camera* camera = _sceneManager->createCamera("Camera");
		camera->setPosition(Ogre::Vector3(0,0,50));
		camera->lookAt(Ogre::Vector3(0,0,0));
		camera->setNearClipDistance(5);

		Viewport *viewPort = window->addViewport(camera);
		viewPort->setBackgroundColour(ColourValue(0.0,0.0,0.0));

		camera->setAspectRatio(Real(viewPort->getActualWidth())/Real(viewPort->getActualHeight()));

		loadResources(); 
		createScene();
		return 0;
	}

	void createScene()
	{
		Entity *ent = _sceneManager->createEntity("Sinbad.mesh");
		_sceneManager->getRootSceneNode()->attachObject(ent);

		_sceneManager->setAmbientLight(ColourValue(0.3f, 0.3f, 0.3f));
	}

	void renderOneFrame()
	{
		WindowEventUtilities::messagePump();
		_keepRunning = _root->renderOneFrame();
	}
};