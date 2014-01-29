#include "Ogre\Ogre.h"

using namespace Ogre;

class CylindricalEffect
{
private:
	Entity *_myEnt;				// any movable entity in the scene that you want to apply the animation effect to
	Ogre::Vector3 _position;	// entity's position (relative to it's parent) in Cylindrical Coordinates, that is, the vector consists of three numbers:
	Ogre::Vector3 _velocity;	// the velocity of the object along the three axes of the cylindrical coordinates, i.e. along the height axis, along the radius and the angular velocity along the circle.

public:
	CylindricalEffect(Entity* entity, Vector3 position, Vector3 velocity)
	{
		_myEnt = entity;
		_position = position;
		_velocity = velocity;
	}
};