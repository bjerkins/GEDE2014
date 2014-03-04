#include "Ogre\Ogre.h"
#include "CylindricalEffect.cpp"
#include "OIS\OIS.h"

using namespace Ogre;
using namespace OIS;

class MyFrameListener : public FrameListener, public JoyStickListener
{
private:
	InputManager* _inputManager;
	Keyboard* _keyboard;
	Mouse* _mouse;
	JoyStick * _joyStick;
	Camera* _cam;
	CylindricalEffect* _cylEffect;
	SceneNode* _ogreNode;
	float _movementspeed;
	float _camAngle;
	int _walkMagnitude; 
	int _turnMagnitude;
	float _orientation;

public:
	// Constructor
	MyFrameListener(RenderWindow* win, Camera* cam, CylindricalEffect *cylEffect, SceneNode* ogreNode)
	{
		_cam = cam;
		_movementspeed = 10.0f;
		_camAngle = -1*Ogre::Math::HALF_PI;
		_ogreNode = ogreNode;
		_walkMagnitude = 0;
		_turnMagnitude = 0;
		_orientation = 0.0f;
		ParamList parameters;
		unsigned int windowHandle = 0;
		std::ostringstream windowHandleString;

		win->getCustomAttribute("WINDOW", &windowHandle);
		windowHandleString << windowHandle;

		parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));

		_inputManager = InputManager::createInputSystem(parameters);

		// cyl effect initialize
		_cylEffect = cylEffect;

		_keyboard = static_cast<Keyboard*>(_inputManager->createInputObject(OISKeyboard, false));
		_mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject(OIS::OISMouse, false));

		// Try to create joystick
		try {
			_joyStick = static_cast<OIS::JoyStick*>(_inputManager->createInputObject( OIS::OISJoyStick, true ));
			_joyStick->setEventCallback(this);
			std::cout << "Successfuly created Joystick";
		}
		catch(...) {
			std::cout << "Failed to initialize Joystick";
			_joyStick = 0;
		}

	}

	// Destructor
	~MyFrameListener()
	{
		_inputManager->destroyInputObject(_keyboard);
		_inputManager->destroyInputObject(_mouse);
		_inputManager->destroyInputObject(_joyStick);
		InputManager::destroyInputSystem(_inputManager);
		delete _cylEffect;
	}

	bool frameStarted(const FrameEvent& evt)
	{
		Ogre::Vector3 translate(0,0,0);

		_keyboard->capture();
		_mouse -> capture();

		if( _joyStick ) 
			_joyStick->capture(); 

		bool walked = false;
		Ogre::Vector3 SinbadTranslate(0, 0, 0);
 
		// If the joystick is not available, use the keyboard as input.
		if (!_joyStick) {
			if (_keyboard->isKeyDown(OIS::KC_UP)) {
				_walkMagnitude = 1;
			}
			else if (_keyboard->isKeyDown(OIS::KC_DOWN)) {
				_walkMagnitude = -1;
			}
			else {
				_walkMagnitude = 0;
			}
 
			if (_keyboard->isKeyDown(OIS::KC_LEFT)) {
				_turnMagnitude = 1;
			}
			else if (_keyboard->isKeyDown(OIS::KC_RIGHT)) {
				_turnMagnitude = -1;
			}
			else {
				_turnMagnitude = 0;
			}
		}
		// Create the translation vector.
		SinbadTranslate = _ogreNode->getOrientation().zAxis() * evt.timeSinceLastFrame * _movementspeed * _walkMagnitude;
		walked = true;
 
		// Increment the roation angle.
		_orientation += evt.timeSinceLastFrame * _turnMagnitude*2;
 
		// Now finally apply the rotation and translation.
		_ogreNode->translate(SinbadTranslate);
		_ogreNode->setOrientation(Ogre::Quaternion(Ogre::Radian(_orientation), Ogre::Vector3::UNIT_Y));


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

		_cylEffect->update(0.1f);

		_cam->setPosition(_ogreNode->getPosition()+Ogre::Vector3(20.0f*Ogre::Math::Cos(_camAngle), 10.0f, 20.0f*Ogre::Math::Sin(_camAngle)));
		_cam->lookAt(_ogreNode->getPosition());

		return true;
	}

	bool MyFrameListener::axisMoved(const OIS::JoyStickEvent &e, int axis) {
		int value = e.state.mAxes[axis].abs;
		switch(axis) {
			case 1:
				_camAngle = Ogre::Math::TWO_PI * ((float(-1*value) / float(_joyStick->MAX_AXIS) + 1.0f)/2.0f)+Ogre::Math::HALF_PI;
				break;
			case 2:
				_walkMagnitude = (float)value / -float(_joyStick->MAX_AXIS); // Map the range to -1 to 1
				break;
			case 3:
				_turnMagnitude = (float)value / -float(_joyStick->MAX_AXIS); // Map the range to -1 to 1
				break;
		}
		return true;
	}

	bool MyFrameListener::buttonPressed(const OIS::JoyStickEvent &arg, int button) {
		return true;
	}

	bool MyFrameListener::buttonReleased(const OIS::JoyStickEvent &arg, int button) {
		return true;
	}

	bool MyFrameListener::povMoved(const OIS::JoyStickEvent &arg, int pov) {

		return true;
	}
};