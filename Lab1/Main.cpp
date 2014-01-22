#include "Ogre\Ogre.h"

using namespace Ogre;

int main (void)
{
	Root* root = new Ogre::Root("plugins_d.cfg");
	if (!root->showConfigDialog())
	{
		return -1;
	}

	RenderWindow* window = root->initialise(true, "Ogre3D Beginners Guide");
	SceneManager* sceneManager = root->createSceneManager(Ogre::ST_GENERIC);

	Camera* camera = sceneManager->createCamera("Camera");
	camera->setPosition(Ogre::Vector3(0,0,50));
	camera->lookAt(Ogre::Vector3(0,0,0));
	camera->setNearClipDistance(5);

	Viewport *viewPort = window->addViewport(camera);
	viewPort->setBackgroundColour(ColourValue(0.0,0.0,0.0));

	camera->setAspectRatio(Real(viewPort->getActualWidth())/Real(viewPort->getActualHeight()));

	//ResourceGroupManager::getSingleton().addResourceLocation("C:/Ogre/OgreSDK_vc11_v1-9-0/media/packs/Sinbad.zip", "Zip");
	//ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
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

	Entity* ent = sceneManager->createEntity("Sinbad.mesh");
	sceneManager->getRootSceneNode()->attachObject(ent);

	sceneManager->setAmbientLight(ColourValue(0.3f, 0.3f, 0.3f));

	root->startRendering();
}