#include "Ogre\Ogre.h"
#include "MyFrameListener.cpp"

using namespace Ogre;

class MyApplication
{

private:
	SceneManager* _sceneManager;
	Root* _root;
	MyFrameListener* _listener;
	SceneNode* _ogreNode;
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
		cf.load("C://OgreCfg//resources_d.cfg");
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
		_root = new Root("C://OgreCfg//plugins_d.cfg");
		
		if (!_root->showConfigDialog()) {
			return -1;
		}

		RenderWindow* window = _root->initialise(true, "Lab 7");
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

		_listener = new MyFrameListener(window, camera, new CylindricalEffect(_myCube, Ogre::Vector3(2.0, 10.0, 0.0), Ogre::Vector3(0.0, 0.0, 0.1)), _ogreNode);
		_root->addFrameListener(_listener);
		return 0;
	}

	void createScene()
	{
		Plane plane(Ogre::Vector3::UNIT_Y, -5);

		MeshManager::getSingleton().createPlane("plane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		
		_myOgre = _sceneManager->createEntity("Sinbad.mesh");
		_myOgre->getSubEntity(0)->setCustomParameter(1, Ogre::Vector4(0.0, 0.0, 1.0, 1.0));
		_myOgre->setMaterialName("shader/lighting");

		_myCube = _sceneManager->createEntity("Cube.mesh");

		_ogreNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
		_ogreNode->attachObject(_myOgre);

		Entity *ground = _sceneManager->createEntity("LightPlaneEntity", "plane");
		
		_sceneManager->getRootSceneNode()->attachObject(ground);

		SceneNode *cubeNode = _myOgre->getParentSceneNode()->createChildSceneNode();
		cubeNode->attachObject(_myCube);
		cubeNode->scale(0.01, 0.01, 0.01);
		cubeNode->setPosition(2.0, 0.0, 0.0); //Notice that this is relative to the Ogre's model origin
		Light *plight = _sceneManager->createLight("Light2");
		plight->setType(Light::LT_POINT);
		cubeNode->attachObject(plight);

		Ogre::Entity* myCustomCube = _sceneManager->createEntity("MyCube", "MyCube.mesh");
		Ogre::SceneNode* myCustomCubeNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
		myCustomCubeNode->attachObject(myCustomCube);
		myCustomCubeNode->setPosition(16.0, 0, 0);
		myCustomCubeNode->setScale(5.0,5.0,5.0); // You may have to scale your object to see it well\\

		// create light
		Light *light = _sceneManager->createLight("Light1");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(Ogre::Vector3(1, -1, 0));

		// some shadows would be nice
		_sceneManager->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

		//ground->setMaterialName("Examples/BeachStones");
		ground->setMaterialName("shader/texture_move");

		_sceneManager->setAmbientLight(ColourValue(0.3f, 0.3f, 0.3f));

		// Create grass
		//createGrass();
	}

	void createGrass()
	{
		ManualObject* manual = _sceneManager->createManualObject("grass");
		manual->begin("Examples/GrassBlades", RenderOperation::OT_TRIANGLE_LIST);
		// Vertices for quad 1
		manual->position(5,0,0);
		manual->textureCoord(1,1);
		manual->position(-5,10,0);
		manual->textureCoord(0,0);
		manual->position(-5,0,0);
		manual->textureCoord(0,1);
		manual->position(5,10,0);
		manual->textureCoord(1,0);
		// Put vertices for other 2 quads here
		// quad 2
		manual->position(2.5,0,4.3);
		manual->textureCoord(1,1);
		manual->position(-2.5,10,-4.3);
		manual->textureCoord(0,0);
		manual->position(-2.0,0,-4.3);
		manual->textureCoord(0,1);
		manual->position(2.5,10,4.3);
		manual->textureCoord(1,0);
		// quad 3
		manual->position(2.5,0,-4.3);
		manual->textureCoord(1,1);
		manual->position(-2.5,10,4.3);
		manual->textureCoord(0,0);
		manual->position(-2.0,0,4.3);
		manual->textureCoord(0,1);
		manual->position(2.5,10,-4.3);
		manual->textureCoord(1,0);

		// Triangles for quad 1
		manual->index(0);
		manual->index(1);
		manual->index(2);
 
		manual->index(0);
		manual->index(3);
		manual->index(1);
		// Put triangles for other 2 quads here                
		// quad2
		manual->index(2);
		manual->index(3);
		manual->index(4);

		manual->index(2);
		manual->index(5);
		manual->index(3);

		//quad3
		manual->index(4);
		manual->index(5);
		manual->index(6);

		manual->index(4);
		manual->index(7);
		manual->index(5);

		// And now wrap up the object and add to scene graph for display
		manual->end();
		manual->convertToMesh("BladesOfGrass");
		Ogre::StaticGeometry* field = _sceneManager->createStaticGeometry("FieldOfGrass");
		// Create a bunch of grass !
		for (int i = 0; i < 49; i++) {
			for (int j = 0; j < 49; j++) {
				Ogre::Entity* ent = _sceneManager->createEntity("BladesOfGrass");
				field->addEntity(ent,Ogre::Vector3(i*3,-10,j*3));
			}
		}
		field->build();
		
	}

	void renderOneFrame()
	{
		WindowEventUtilities::messagePump();
		_keepRunning = _root->renderOneFrame();
	}
};