#ifndef _ITEM_H_
#define _ITEM_H_

#include "entity.h"


/*!
	CItem defines a type of entity that can
	be collected by the player entity.
	\author Javier Bermejo García
*/
class CItem : public CEntity {
public:
	/*!
		CItem constructor.
	*/
	CItem(void);

	/*!
		Item collision handler.
		\param Entity the entity that collided with the item.
		\param Col the collision bitmap.
	*/
	void Collision(CEntity *Entity, Uint32 &col);
};

#endif
