#include "collision.hpp"
#include "player.hpp"
#include "rect.hpp"
#include "utils.hpp"


Uint32 Colision::Invert(Uint32 col)
{
    Uint32 ret = 0;

    /* Invert X */
    if (col & COLLISION_TOP) {
        ret |= COLLISION_BOTTOM;
    }

    if (col & COLLISION_BOTTOM) {
        ret |= COLLISION_TOP;
    }

    /* Invert Y */
    if (col & COLLISION_LEFT) {
        ret |= COLLISION_RIGHT;
    }

    if (col & COLLISION_RIGHT) {
        ret |= COLLISION_LEFT;
    }

    return ret;
}

Uint32 Colision::Check(Entity *entity1, Entity *entity2, bool fast)
{
    Rect r1 = entity1->GetRect();
    Rect r2 = entity2->GetRect();
    Rect result;
    
    vector<Rect *>::iterator it;
    vector<Rect *>::iterator jt;

    Uint32 col = 0;
    bool   ret;

    /* Same entity */
    if (entity1 == entity2) return 0;

    /* Check all collisions */
    foreach (entity1->mCollisions, it) {
        foreach (entity2->mCollisions, jt) {
            Rect *rect1 = *it;
            Rect *rect2 = *jt;
            
            rect1->Add(&r1);
            rect2->Add(&r2);
        
            /* Check intersection */
            ret = rect1->Intersection(rect2, result);
            if (!ret) continue;
            
            /* X collision */
            if (result.GetWidth() < result.GetHeight()) {
                if (rect1->GetX() == result.GetX()) {
                    col |= COLLISION_LEFT;
                }

                if (rect2->GetX() == result.GetX()) {
                    col |= COLLISION_RIGHT;
                }
            }
         
            /* Y collision */
            if (result.GetWidth() >= result.GetHeight()) {
                if (rect1->GetY() == result.GetY()) {
                    col |= COLLISION_BOTTOM;
                }
                
                if (rect2->GetY() == result.GetY()) {
                    col |= COLLISION_TOP;
                }
            }
            
            /* If collision exists and is a fast check */
            if (col && fast) return col;
        }
    }

    return col;
}

Uint32 Colision::Handle(Entity *entity1, Entity *entity2, bool fast)
{
    Uint32 col = 0;

    /* Entity dead */
    if (entity2->GetState() & ENTITY_DEAD) {
        return 0;
    }

    /* collision check */
    col = Check(entity1, entity2, fast);

    /* collision detected */
    if (col) {
        Uint32 tmp;

        /* Invert bitmap */
        tmp = Invert(col);

        /* Handle collision */
        entity1->Collision(entity2, col);
        entity2->Collision(entity1, tmp);

        /* Entity died */
        if (entity2->GetState() & ENTITY_DEAD) {
            return 0;
        }
    }	

    return col;
}

Uint32 Colision::CheckLevel(Entity *entity, Level *level)
{
    vector<Entity *>::iterator it;

    Rect   Rect = entity->GetRect();
    Uint32 col  = 0;

    /* Limits collision */
    if (Rect.GetX() <= 0) {
        col |= COLLISION_LEFT;
    }

    if ((Rect.GetX() + Rect.GetWidth()) >= level->GetWidth()) {
        col |= COLLISION_RIGHT;
    }

    /* Entity collision */
    foreach (level->mEntityList, it) {
        Entity *Target = *it;

        /* Handle collision */
        if (Target->GetCollidable()) {
            col |= Handle(entity, Target, false);
        }
    }

    return col;
}

Uint32 Colision::Detect(Entity *entity)
{
    Uint32 col = 0;

    /* Dead entity */
    if (entity->GetState() & ENTITY_DEAD) {
        return 0;
    }

    /* Level collisions */
    if (gpLevel) {
        col |= CheckLevel(entity, gpLevel);
    }

    /* Player collision */
    if (gpPlayer) {
        col |= Handle(entity, gpPlayer);
    }

    return col;
}
