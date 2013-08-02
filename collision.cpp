#include "collision.h"
#include "player.h"
#include "rect.h"
#include "utils.h"


Uint32 CCollision::Invert(Uint32 col)
{
	Uint32 ret = 0;

	/* Invert X */
	if (col & COLLISION_TOP)
		ret |= COLLISION_BOTTOM;
	if (col & COLLISION_BOTTOM)
		ret |= COLLISION_TOP;

	/* Invert Y */
	if (col & COLLISION_LEFT)
		ret |= COLLISION_RIGHT;
	if (col & COLLISION_RIGHT)
		ret |= COLLISION_LEFT;

	return ret;
}

Uint32 CCollision::Check(CEntity *Entity1, CEntity *Entity2, bool fast)
{
	CRect R1 = Entity1->GetRect();
	CRect R2 = Entity2->GetRect();
	CRect Result;
	
	vector<CRect *>::iterator it, jt;

	Uint32 col = 0;
	bool   ret;

	/* Same entity */
	if (Entity1 == Entity2)
		return 0;

	/* Check all collisions */
	foreach (Entity1->Collisions, it) {
		foreach (Entity2->Collisions, jt) {
			CRect *Rect1 = *it;
			CRect *Rect2 = *jt;
			
			Rect1->Add(&R1);
			Rect2->Add(&R2);
		
			/* Check intersection */
			ret = Rect1->Intersection(Rect2, Result);
			if (!ret)
				continue;
			
			/* X collision */
			if (Result.GetWidth() < Result.GetHeight()) {
				if (Rect1->GetX() == Result.GetX())
					col |= COLLISION_LEFT;
				if (Rect2->GetX() == Result.GetX())
					col |= COLLISION_RIGHT;
			}
		 
			/* Y collision */
			if (Result.GetWidth() >= Result.GetHeight()) {
				if (Rect1->GetY() == Result.GetY())
					col |= COLLISION_BOTTOM;
				if (Rect2->GetY() == Result.GetY())
					col |= COLLISION_TOP;
			}
			
			/* If collision exists and is a fast check */
			if (col && fast)
				return col;
		}
	}

	return col;
}

Uint32 CCollision::Handle(CEntity *Entity1, CEntity *Entity2, bool fast)
{
	Uint32 col = 0;

	/* Entity dead */
	if (Entity2->GetState() & ENTITY_DEAD)
		return 0;

	/* collision check */
	col = Check(Entity1, Entity2, fast);

	/* collision detected */
	if (col) {
		Uint32 tmp;

		/* Invert bitmap */
		tmp = Invert(col);

		/* Handle collision */
		Entity1->Collision(Entity2, col);
		Entity2->Collision(Entity1, tmp);

		/* Entity died */
		if (Entity2->GetState() & ENTITY_DEAD)
			return 0;
	}	

	return col;
}

Uint32 CCollision::CheckLevel(CEntity *Entity, CLevel *Level)
{
	vector<CEntity *>::iterator it;

	CRect  Rect = Entity->GetRect();
	Uint32 col  = 0;

	/* Limits collision */
	if (Rect.GetX() <= 0)
		col |= COLLISION_LEFT;
	if ((Rect.GetX() + Rect.GetWidth()) >= Level->GetWidth())
		col |= COLLISION_RIGHT;

	/* Entity collision */
	foreach (Level->EntityList, it) {
		CEntity *Target = *it;

		/* Handle collision */
		if (Target->GetCollidable())
			col |= Handle(Entity, Target, false);
	}

	return col;
}

Uint32 CCollision::Detect(CEntity *Entity)
{
	Uint32 col = 0;

	/* Dead entity */
	if (Entity->GetState() & ENTITY_DEAD)
		return 0;

	/* Level collisions */
	if (pLevel)
		col |= CheckLevel(Entity, pLevel);

	/* Player collision */
	if (pPlayer)
		col |= Handle(Entity, pPlayer);

	return col;
}
