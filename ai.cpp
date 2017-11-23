#include "ai.hpp"
#include "collision.hpp"
#include "level.hpp"
#include "utils.hpp"

/* AI handler */
Ai *pAi = NULL;


void Ai::SetActive(void)
{
    /* Set active AI */
    pAi = this;
}

void Ai::Run(Entity *entity)
{
    Uint32 collision;
    Uint16 motion = entity->GetMotion();

    /* Entity inactive */
    if (entity->GetMotion() & ENTITY_INACTIVE) {
        motion &= ~ENTITY_INACTIVE;
        motion &= ~ENTITY_RIGHT;
        motion &= ~ENTITY_LEFT;
        
        /* Start direction */
        if (entity->GetEntityNumber() % 2 == 0) {
            motion |= ENTITY_RIGHT;
        } else {
            motion |= ENTITY_LEFT;
        }
            
        motion |= ENTITY_DOWN;
        
        /* Start motion */
        entity->SetMotion(motion);
        entity->Run();
        
        return;
    }

    /* Collision detect */
    collision = Colision::Detect(entity);

    /* Top collision */
    if (collision & COLLISION_TOP) {
        motion &= ~ENTITY_UP;
        motion |=  ENTITY_DOWN;
    }
    
    /* Bottom collision */
    if (collision & COLLISION_BOTTOM) {
        motion &= ~ENTITY_DOWN;
        motion |=  ENTITY_UP;
    }
        
    /* Left collision */
    if (collision & COLLISION_LEFT) {
        motion &= ~ENTITY_LEFT;
        motion |=  ENTITY_RIGHT;
    }
    
    /* Right collision */
    if (collision & COLLISION_RIGHT) {
        motion &= ~ENTITY_RIGHT;
        motion |=  ENTITY_LEFT;
    }
    
    /* Set motion */
    entity->SetMotion(motion);
    entity->Run();
}

void Ai::Update(void)
{
    vector<Entity *>::iterator it;

    /* Level entities */
    if (gpLevel) {
        foreach (gpLevel->mEntityList, it) {
            Entity *entity = *it;
            
            /* Get type */
            string type = entity->GetType();
            
            /* Check type */
            if (type.compare("Enemy")) {
                continue;
            }
                
            Uint16 state = entity->GetState();
            
            /* Check state */
            if ((state & ENTITY_DEAD) || (state & ENTITY_INVISIBLE)) {
                continue;
            }

            /* Run entity AI */
            Run(entity);
        }
    }
}
