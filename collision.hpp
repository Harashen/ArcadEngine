#ifndef _COLLISION_HPP_
#define _COLLISION_HPP_

#include "entity.hpp"
#include "level.hpp"


//! Collision bitmap values.
enum {
    COLLISION_NONE	 = 0,
    COLLISION_TOP	 = 1,
    COLLISION_BOTTOM = 2,
    COLLISION_LEFT	 = 4,
    COLLISION_RIGHT	 = 8
};

/*!
    Colision is the class that manages the
    collisions between the game entities.
    \author Javier Bermejo García
*/
class Colision {
    /*!
        Invert the collision bitmap.
        \param col the collision bitmap.
        \return the collision bitmap inverted.
    */
    static Uint32 Invert(Uint32 col);

    /*!
        Check if an entity is colliding with the level entities.
        \param Entity the entity to check if collided.
        \param Level the level to check the collisions with the entity.
        \return the collision bitmap.
    */
    static Uint32 CheckLevel(Entity *entity, Level *level);

public:
    /*!
        Check if two entities are colliding.
        \param entity1 the first entity.
        \param entity2 the second entity.
        \param fast if true, only detect one collision. Otherwise get all collisions.
        \return the collision bitmap.
    */
    static Uint32 Check(Entity *entity1, Entity *entity2, bool fast);

    /*!
        Check and handle the collision between two entities.
        \param entity1 the first entity.
        \param entity2 the second entity.
        \param fast to do fast checks.
        \return the collision bitmap.
    */
    static Uint32 Handle(Entity *entity1, Entity *entity2, bool fast = true);

    /*!
        Detect all the collision that occur in an entity.
        \param entity the entity to check if collided.
        \return the collision bitmap.
    */
    static Uint32 Detect(Entity *entity);
};

#endif
