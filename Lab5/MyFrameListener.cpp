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
	float _movementspeed;
	float _camAngle;

public:
	// Constructor
	MyFrameListener(RenderWindow* win, Camera* cam, CylindricalEffect *cylEffect)
	{
		_cam = cam;
		_movementspeed = 50.0f;
		_camAngle = -1*Ogre::Math::HALF_PI;
		
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

		return true;
	}

	bool MyFrameListener::axisMoved(const OIS::JoyStickEvent &e, int axis) {
		int value = e.state.mAxes[axis].abs;
		switch(axis) {
			case 1:
				std::cout << "1:" << value << "\n";
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