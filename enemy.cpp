#include "collision.h"
#include "enemy.h"
#include "player.h"


CEnemy::CEnemy(void)
{
	/* Set type */
	Type = "Enemy";
}

CEnemy::~CEnemy(void)
{
	/* Stop sound */
	Sound.Stop();
}

void CEnemy::Collision(CEntity *Entity, Uint32 &col)
{
	/* Enemy dead */
	if (State & ENTITY_DEAD)
		return;
		
	string type = Entity->GetType();

	/* Default handler */
	CEntity::Collision(Entity, col);

	/* Collision with bullet */
	if (!type.compare("Weapon")) {
		/* Kill enemy */
		Sound.Play(0);
		Kill();
		
		/* Set player points */
		pPlayer->SetScore(this->Points);
	}

	/* Collision with item */
	if (!type.compare("Item"))
		col = 0;
		
	/* Collision with enemy */
	if (!type.compare("Enemy"))
		col = 0;
}
