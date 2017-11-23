#ifndef _ITEM_HPP_
#define _ITEM_HPP_

#include "entity.hpp"


/*!
    Item defines a type of entity that can
    be collected by the player entity.
    \author Javier Bermejo García
*/
class Item : public Entity {
public:
    /*!
        Item constructor.
    */
    Item(void);

    /*!
        Item collision handler.
        \param Entity the entity that collided with the item.
        \param Col the collision bitmap.
    */
    void Collision(Entity *entity, Uint32 &col);
};

#endif
