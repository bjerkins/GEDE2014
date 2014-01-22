#include "Ogre\Ogre.h"
#include "OIS\OIS.h"

using namespace Ogre;
using namespace OIS;

class MyFrameListener : public FrameListener
{
private:
	InputManager* _inputManager;
	Keyboard* _keyboard;
	Mouse* _mouse;
	Camera* _cam;
	float _movementspeed;

public:
	// Constructor
	MyFrameListener(RenderWindow* win, Camera* cam)
	{
		_cam = cam;
		_movementspeed = 50.0f;
		
		ParamList parameters;
		unsigned int windowHandle = 0;
		std::ostringstream windowHandleString;

		win->getCustomAttribute("WINDOW", &windowHandle);
		windowHandleString << windowHandle;

		parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));

		_inputManager = InputManager::createInputSystem(parameters);

		_keyboard = static_cast<Keyboard*>(_inputManager->createInputObject(OISKeyboard, false));
		_mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject(OIS::OISMouse, false));
	}

	// Destructor
	~MyFrameListener()
	{
		_inputManager->destroyInputObject(_keyboard);
		_inputManager->destroyInputObject(_mouse);
		InputManager::destroyInputSystem(_inputManager);
	}

	bool frameStarted(const FrameEvent& evt)
	{
		Ogre::Vector3 translate(0,0,0);

		_keyboard->capture();
		_mouse -> capture();

		if (_keyboard->isKeyDown(KC_ESCAPE))
		{
			return false;
		}
		if (_keyboard->isKeyDown(KC_W))
			translate += Ogre::Vector3(0,0,-1);
		if (_keyboard->isKeyDown(KC_S))
			translate += Ogre::Vector3(0,0,1);
		if (_keyboard->isKeyDown(KC_A))
			translate += Ogre::Vector3(-1,0,0);
		if (_keyboard->isKeyDown(KC_D))
			translate += Ogre::Vector3(1,0,0);

		_cam->moveRelative(translate*evt.timeSinceLastFrame * _movementspeed);

		float rotX = _mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -1;
		float rotY = _mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;

		_cam->yaw(Radian(rotX));
		_cam->pitch(Radian(rotY));

		return true;
	}
};