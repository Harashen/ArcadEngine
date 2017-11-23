#ifndef _ENEMY_HPP_
#define _ENEMY_HPP_

#include "character.hpp"


/*!
    Enemy defines a type of character (an entity) that
    behaves as a game enemy (can kill the player entity).
    \author Javier Bermejo García
*/
class Enemy : public Character {
public:
    /*!
        Enemy constructor.
    */
    Enemy(void);
    
    /*!
        Enemy destructor.
    */
    ~Enemy(void);

    /*!
        Enemy collision handler.
        \param entity the entity that collided with the enemy.
        \param col the collision bitmap.
    */
    void Collision(Entity *entity, Uint32 &col);
};

#endif
