#include "Ogre\Ogre.h"
#include "OgreMath.h"

using namespace Ogre;

class CylindricalEffect
{
private:
	Entity *_myEnt;				// any movable entity in the scene that you want to apply the animation effect to
	Ogre::Vector3 _position;	// entity's position (relative to it's parent) in Cylindrical Coordinates, that is, the vector consists of three numbers:
	Ogre::Vector3 _velocity;	// the velocity of the object along the three axes of the cylindrical coordinates, i.e. along the height axis, along the radius and the angular velocity along the circle.

public:
	CylindricalEffect(Entity* entity, Ogre::Vector3 position, Ogre::Vector3 velocity)
	{
		_myEnt = entity;
		_position = position;
		_velocity = velocity;
	}

	Ogre::Vector3 GetCartesian()
	{
		// (height,radius,angle) = (h, r, α)
		double x = Math::Cos(_position.z) * _position.y;
		double y = _position.x;
		double z = Math::Sin(_position.z) * _position.y;
		return Ogre::Vector3(x,y,z);
	}

	void update(float dt) {
		_position = _position + dt*_velocity;
		_myEnt->getParentSceneNode()->setPosition(GetCartesian());	
	}
};