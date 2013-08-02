#include "weapon.h"


CWeapon::CWeapon(void)
{
	/* Set attributes */
	Type   = "Weapon";
	Motion = ENTITY_UP;
	State  = ENTITY_DEAD;
	Show   = false;
}

void CWeapon::Collision(CEntity *Entity, Uint32 &col)
{
	string type = Entity->GetType();

	/* Default handler */
	CEntity::Collision(Entity, col);

	/* Collision with item/player */
	if (!type.compare("Item") || !type.compare("Player")) {
		/* Ignore collision */
		col = 0;
	}

	/* Kill bullet */
	if (col) {
		Show = false;
		Kill();
		Sound.Stop();
	}
}

bool CWeapon::Shot(CRect Rect, bool sound)
{
	/* One shot */
	if (!(State & ENTITY_DEAD))
		return false;
		
	float x = Rect.GetX() + (Rect.GetWidth() * 0.5);
	float y = Rect.GetY() + 5;
	
	/* Set start */
	SetStart(x, y);
	
	/* Shot */
	ResetRect();
	Show  = true;
	State = ENTITY_WALK;
	
	/* Start motion */
	Run();
	
	if (sound)
		Sound.Play(0);
		
	return true;
}

bool CWeapon::Update(void)
{
	if (!Show)
		return false;
	
	CEntity::Update();
	
	return true;
}

bool CWeapon::Draw(void)
{
	if (!Show || (State & ENTITY_DEAD))
		return false;
	
	/* Update animation */	
	Animation.Update(false);
	
	/* Draw Animation */
	Animation.Draw(NULL, &Rect);
	
	return true;
}
