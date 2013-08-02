#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "entity.h"
#include "level.h"


//! Collision bitmap values.
enum {
	COLLISION_NONE		= 0,
	COLLISION_TOP		= 1,
	COLLISION_BOTTOM	= 2,
	COLLISION_LEFT		= 4,
	COLLISION_RIGHT		= 8
};

/*!
	CCollision is the class that manages the
	collisions between the game entities.
	\author Javier Bermejo García
*/
class CCollision {
	/*!
		Invert the collision bitmap.
		\param col the collision bitmap.
		\return the collision bitmap inverted.
	*/
	static Uint32 Invert(Uint32 col);

	/*!
		Check if an entity is colliding with the level entities.
		\param Entity the entity to check if collided.
		\param Level the level to check the collisions with the entity.
		\return the collision bitmap.
	*/
	static Uint32 CheckLevel(CEntity *Entity, CLevel *Level);

public:
	/*!
		Check if two entities are colliding.
		\param Entity1 the first entity.
		\param Entity2 the second entity.
		\param fast if true, only detect one collision. Otherwise get all collisions.
		\return the collision bitmap.
	*/
	static Uint32 Check(CEntity *Entity1, CEntity *Entity2, bool fast);

	/*!
		Check and handle the collision between two entities.
		\param Entity1 the first entity.
		\param Entity2 the second entity.
		\param fast to do fast checks.
		\return the collision bitmap.
	*/
	static Uint32 Handle(CEntity *Entity1, CEntity *Entity2, bool fast = true);

	/*!
		Detect all the collision that occur in an entity.
		\param Entity the entity to check if collided.
		\return the collision bitmap.
	*/
	static Uint32 Detect(CEntity *Entity);
};

#endif
