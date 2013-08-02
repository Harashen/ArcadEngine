#include "collision.h"
#include "item.h"


CItem::CItem(void)
{
	/* Set type */
	Type = "Item";
	
	Collidable = true;
}

void CItem::Collision(CEntity *Entity, Uint32 &col)
{
	string type = Entity->GetType();

	/* Default handler */
	CEntity::Collision(Entity, col);

	/* Collision with player */
	if (!type.compare("Player"))
		Kill();
}
