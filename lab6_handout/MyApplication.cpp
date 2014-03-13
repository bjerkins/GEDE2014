#include "MyApplication.h"


MyApplication::MyApplication() : _root(NULL), _sceneManager(NULL), _listener(NULL) {
	// Do some constructing
}


MyApplication::~MyApplication() {
	delete _root;
	delete _listener;
}

void MyApplication::loadResources() {
	// Load the chosen resource file.
	Ogre::ConfigFile cf;
	cf.load("c://cc//OgreCfg//resources_d.cfg");

	// Create the necessary vars for iterating the resource file.
	Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();
	Ogre::String sectionName, typeName, dataName;

	// For each section - load each resource or collection
	while (sectionIter.hasMoreElements()) {
		
		sectionName = sectionIter.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = sectionIter.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;

		for (i = settings->begin(); i != settings->end(); ++i) {
			typeName = i->first;
			dataName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataName, typeName, sectionName);
		}
	}
	// Initialize the resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}


int MyApplication::startUp() {
	// Create the root with the selected configurations
	_root = new Ogre::Root("c://cc//OgreCfg//plugins_d.cfg");

	// Show the configuration window.
	if (!_root->showConfigDialog()) {
		return -1;
	}

	// Create a Window
	Ogre::RenderWindow* window = _root->initialise(true, "Ogre3D Beginners Guide");

	// Initiate the scenemanager.
	_sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);

	// Create a camera and configure it.
	Ogre::Camera* camera = _sceneManager->createCamera("Camera");
	camera->setPosition(Ogre::Vector3(0, 0, 50));
	camera->lookAt(Ogre::Vector3(0, 0, 0));
	camera->setNearClipDistance(5);

	// Create a viewport for the camera.
	Ogre::Viewport* viewPort = window->addViewport(camera);
	viewPort->setBackgroundColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	camera->setAspectRatio(Ogre::Real(viewPort->getActualWidth() / Ogre::Real(viewPort->getActualHeight())));

	// Load the resources.
	loadResources();

	// Create the scene with the loaded resources.
	createScene();

	// Initialize the framelistener and set it as the defult frameListener for the application.
	_listener = new MyFrameListener(window, camera, _SinbadNode, _SinbadEnt);
	_root->addFrameListener(_listener);

	return 0;


}

void MyApplication::createScene() {

	// Create Sinbad the mad Ogre pirate buccaneer guy.
	_SinbadEnt = _sceneManager->createEntity("Sinbad.mesh");
	_SinbadNode = _sceneManager->getRootSceneNode()->createChildSceneNode("Sinbad");
	_SinbadNode->attachObject(_SinbadEnt);

	// Create the plane.
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -5);
	Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		1500, 1500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	// Set the plane as the ground and add a texture to it.
	Ogre::Entity* ground = _sceneManager->createEntity("LightPlaneEntity", "plane");
	_sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(ground);
	ground->setMaterialName("Examples/BeachStones");

	// Set up the scene lighting
	_sceneManager->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	Ogre::Light* light1 = _sceneManager->createLight("Light1");
	light1->setType(Ogre::Light::LT_DIRECTIONAL);
	light1->setDirection(1, -1, 0);
	_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

}

// Render the frame
void MyApplication::renderOneFrame() {
	Ogre::WindowEventUtilities::messagePump();
	_keepRunning = _root->renderOneFrame();
}

// Should the app keep rendering.
bool MyApplication::keepRunning() {
	return _keepRunning;
}