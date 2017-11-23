#include "item.hpp"
#include "collision.hpp"


Item::Item(void)
{
    /* Set type */
    mType = "Item";
    
    mCollidable = true;
}

void Item::Collision(Entity *entity, Uint32 &col)
{
    string type = entity->GetType();

    /* Default handler */
    Entity::Collision(entity, col);

    /* Collision with player */
    if (!type.compare("Player")) {
        Kill();
    }
}
