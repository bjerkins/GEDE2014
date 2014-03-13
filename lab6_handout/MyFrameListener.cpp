#include "MyFrameListener.h"
MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::SceneNode* node, Ogre::Entity* ent) : _Cam(cam), _SinbadNode(node) {
	// Vars for creating the application window.
	OIS::ParamList parameters;
	unsigned int windowHandle = 0;
	std::ostringstream windowHandleString;

	win->getCustomAttribute("WINDOW", &windowHandle);
	windowHandleString << windowHandle;
	parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));

	// Connect the application window to the input manager.
	_InputManager = OIS::InputManager::createInputSystem(parameters);

	// Init the keyboard input class.
	_Keyboard = static_cast<OIS::Keyboard*>(_InputManager->createInputObject(OIS::OISKeyboard, false));

	// Init the Mouse input class.
	_Mouse = static_cast<OIS::Mouse*>(_InputManager->createInputObject(OIS::OISMouse, false));

	// Set up some Sinbad and camera related constants.
	_movementSpeed = 50.0;
	_WalkingSpeed = 50.0;

	// Load the animation states for Sinbad walking upper and lower body.
	_aniState = ent->getAnimationState("RunBase");
	_aniState->setLoop(false);

	_aniStateTop = ent->getAnimationState("RunTop");
	_aniStateTop->setLoop(false);

}

MyFrameListener::~MyFrameListener() {
	_InputManager->destroyInputObject(_Mouse);
	_InputManager->destroyInputObject(_Keyboard);
	OIS::InputManager::destroyInputSystem(_InputManager);
}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
	// Capture the keyboard input.
	_Keyboard->capture();

	// Quit if the user presses ESC
	if (_Keyboard->isKeyDown(OIS::KC_ESCAPE)) {
		return false;
	}

	// Catch the camera movement.
	Ogre::Vector3 translate(0, 0, 0);
	if (_Keyboard->isKeyDown(OIS::KC_W)) {
		translate += Ogre::Vector3(0, 0, -1);
	}
	if (_Keyboard->isKeyDown(OIS::KC_S)) {
		translate += Ogre::Vector3(0, 0, 1);
	}
	if (_Keyboard->isKeyDown(OIS::KC_A)) {
		translate += Ogre::Vector3(-1, 0, 0);
	}
	if (_Keyboard->isKeyDown(OIS::KC_D)) {
		translate += Ogre::Vector3(1, 0, 0);
	}
	// Apply the camera movement.
	_Cam->moveRelative(translate * evt.timeSinceLastFrame * _movementSpeed);

	// capture the mouse input.
	_Mouse->capture();

	// Load the mouse delta movement between frames.
	float rotX = _Mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -1;
	float rotY = _Mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;

	// Apply the movement to the camera's rotation matrix.
	_Cam->yaw(Ogre::Radian(rotX));
	_Cam->pitch(Ogre::Radian(rotY));

	// Capture the movement for Sinbad.
	Ogre::Vector3 SinbadTranslate(0, 0, 0);
	float _rotation = 0.0f;

	bool walked = false;
	if (_Keyboard->isKeyDown(OIS::KC_UP)) {
		SinbadTranslate += Ogre::Vector3(0, 0, -1);
		walked = true;
		_rotation = 3.14f;
	}
	if (_Keyboard->isKeyDown(OIS::KC_DOWN)) {
		SinbadTranslate += Ogre::Vector3(0, 0, 1);
		walked = true;
		_rotation = 0.0f;
	}
	if (_Keyboard->isKeyDown(OIS::KC_LEFT)) {
		SinbadTranslate += Ogre::Vector3(-1, 0, 0);
		walked = true;
		_rotation = -1.57f;
	}
	if (_Keyboard->isKeyDown(OIS::KC_RIGHT)) {
		SinbadTranslate += Ogre::Vector3(1, 0, 0);
		walked = true;
		_rotation = 1.57f;
	}

	// Apply the movement to Sinbads transformation matrix.
	_SinbadNode->translate(SinbadTranslate * evt.timeSinceLastFrame * _WalkingSpeed);
	_SinbadNode->resetOrientation();
	_SinbadNode->yaw(Ogre::Radian(_rotation));

	// Animate Sinbad walking.
	if (walked) {
		_aniState->setEnabled(true);
		_aniStateTop->setEnabled(true);
		if (_aniState->hasEnded()) {
			_aniState->setTimePosition(0.0f);
		}
		if (_aniStateTop->hasEnded()) {
			_aniStateTop->setTimePosition(0.0f);
		}
	}
	else {
		_aniState->setTimePosition(0.0f);
		_aniState->setEnabled(false);
		_aniStateTop->setTimePosition(0.0f);
		_aniStateTop->setEnabled(false);
	}
	
	//Add time to the walking animations.
	_aniState->addTime(evt.timeSinceLastFrame);
	_aniStateTop->addTime(evt.timeSinceLastFrame);

	return true;
}
