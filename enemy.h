#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "character.h"


/*!
	CEnemy defines a type of character (an entity) that
	behaves as a game enemy (can kill the player entity).
	\author Javier Bermejo García
*/
class CEnemy : public CCharacter {
public:
	/*!
		CEnemy constructor.
	*/
	CEnemy(void);
	
	/*!
		CEnemy destructor.
	*/
	~CEnemy(void);

	/*!
		Enemy collision handler.
		\param Entity the entity that collided with the enemy.
		\param col the collision bitmap.
	*/
	void Collision(CEntity *Entity, Uint32 &col);
};

#endif
