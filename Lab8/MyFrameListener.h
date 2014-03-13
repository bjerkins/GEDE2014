#ifndef MYFRAMELISTENER_H
#define MYFRAMELISTENER_H
#include "OGRE\Ogre.h"
#include "OIS\OIS.h"

class MyFrameListener : public Ogre::FrameListener
{
private:
	OIS::InputManager*		_InputManager;
	OIS::Keyboard*			_Keyboard;

	OIS::Mouse*				_Mouse;
	Ogre::Camera*			_Cam;
	float					_movementSpeed;

	float					_WalkingSpeed;
	Ogre::SceneNode*		_SinbadNode;

	Ogre::AnimationState*	_aniState;
	Ogre::AnimationState*	_aniStateTop;

public:
	MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::SceneNode* node, Ogre::Entity* ent);
	~MyFrameListener();

	bool frameStarted(const Ogre::FrameEvent& evt);

};

#endif //MYFRAMELISTENER_H