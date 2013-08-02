#include "ai.h"
#include "collision.h"
#include "level.h"
#include "utils.h"

/* AI handler */
CAi *pAi = NULL;


void CAi::SetActive(void)
{
	/* Set active AI */
	pAi = this;
}

void CAi::Run(CEntity *Entity)
{
	Uint32 collision;
	Uint16 motion = Entity->GetMotion();

	/* Entity inactive */
	if (Entity->GetMotion() & ENTITY_INACTIVE) {
        motion &= ~ENTITY_INACTIVE;
        motion &= ~ENTITY_RIGHT;
        motion &= ~ENTITY_LEFT;
        
		/* Start direction */
        if (Entity->GetEntityNumber() % 2 == 0)
            motion |= ENTITY_RIGHT;
        else
            motion |= ENTITY_LEFT;
            
        motion |= ENTITY_DOWN;
        
		/* Start motion */
        Entity->SetMotion(motion);
        Entity->Run();
		
        return;
    }

	/* Collision detect */
	collision = CCollision::Detect(Entity);

	/* Top collision */
	if (collision & COLLISION_TOP){
        motion &= ~ENTITY_UP;
        motion |=  ENTITY_DOWN;
    }
	
	/* Bottom collision */
	if (collision & COLLISION_BOTTOM){
        motion &= ~ENTITY_DOWN;
        motion |=  ENTITY_UP;
    }
		
	/* Left collision */
	if (collision & COLLISION_LEFT){
		motion &= ~ENTITY_LEFT;
        motion |=  ENTITY_RIGHT;
    }
	
	/* Right collision */
	if (collision & COLLISION_RIGHT){
        motion &= ~ENTITY_RIGHT;
        motion |=  ENTITY_LEFT;
    }
	
	/* Set motion */
	Entity->SetMotion(motion);
	Entity->Run();
}

void CAi::Update(void)
{
	vector<CEntity *>::iterator it;

	/* Level entities */
	if (pLevel) {
		foreach (pLevel->EntityList, it) {
			CEntity *Entity = *it;
			
			/* Get type */
			string type = Entity->GetType();
			
			/* Check type */
			if (type.compare("Enemy") )	
				continue;
				
			Uint16 state = Entity->GetState();
			
			/* Check state */
			if ((state & ENTITY_DEAD) || (state & ENTITY_INVISIBLE))
				continue;

			/* Run entity AI */
			Run(Entity);
		}
	}
}
