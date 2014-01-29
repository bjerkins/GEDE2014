#include "Ogre\Ogre.h"
#include "MyFrameListener.cpp"

using namespace Ogre;

class MyApplication
{

private:
	SceneManager* _sceneManager;
	Root* _root;
	MyFrameListener* _listener;
	Entity *_myOgre;
	Entity *_myCube;
	bool _keepRunning;

public:
	// Constructor
	MyApplication()
	{
		_sceneManager = NULL;
		_root = NULL;
		_listener = NULL;
		_myOgre = NULL;
		_myCube = NULL;
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

		RenderWindow* window = _root->initialise(true, "Lab 3");
		_sceneManager = _root->createSceneManager(ST_GENERIC);

		Camera* camera = _sceneManager->createCamera("Camera");
		camera->setPosition(Ogre::Vector3(0,0,50));
		camera->lookAt(Ogre::Vector3(0,0,0));
		camera->setNearClipDistance(5);

		Viewport *viewPort = window->addViewport(camera);
		viewPort->setBackgroundColour(ColourValue(0.0,0.0,0.0));

		camera->setAspectRatio(Real(viewPort->getActualWidth())/Real(viewPort->getActualHeight()));

		loadResources(); 
		createScene();

		_listener = new MyFrameListener(window, camera, new CylindricalEffect(_myCube, Ogre::Vector3(2.0, 10.0, 0.0), Ogre::Vector3(0.0, 0.0, 0.1)));
		_root->addFrameListener(_listener);
		return 0;
	}

	void createScene()
	{
		Plane plane(Ogre::Vector3::UNIT_Y, -5);

		MeshManager::getSingleton().createPlane("plane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		
		_myOgre = _sceneManager->createEntity("Sinbad.mesh");
		_myCube = _sceneManager->createEntity("Cube.mesh");

		Entity *ground = _sceneManager->createEntity("LightPlaneEntity", "plane");
		
		_sceneManager->getRootSceneNode()->attachObject(_myOgre);
		_sceneManager->getRootSceneNode()->attachObject(ground);

		SceneNode *cubeNode = _myOgre->getParentSceneNode()->createChildSceneNode();
		cubeNode->attachObject(_myCube);
		cubeNode->scale(0.01, 0.01, 0.01);
		cubeNode->setPosition(2.0, 0.0, 0.0); //Notice that this is relative to the Ogre's model origin

		// create light
		Light *light = _sceneManager->createLight("Light1");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(Ogre::Vector3(1, -1, 0));

		Light *plight = _sceneManager->createLight("Light2");
		plight->setType(Light::LT_POINT);
		cubeNode->attachObject(plight);

		// some shadows would be nice
		_sceneManager->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

		ground->setMaterialName("Examples/BeachStones");

		_sceneManager->setAmbientLight(ColourValue(0.3f, 0.3f, 0.3f));
	}

	void renderOneFrame()
	{
		WindowEventUtilities::messagePump();
		_keepRunning = _root->renderOneFrame();
	}
};