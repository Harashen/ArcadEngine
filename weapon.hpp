#ifndef _WEAPON_HPP_
#define _WEAPON_HPP_

#include "character.hpp"


/*!
    Weapon defines a type of character (an entity) that can be
    shot by other kind of entities (players, enemies...).
    \author Javier Bermejo García
*/
class Weapon : public Character {
    bool mShow;		//!< Flag to show the shoot.
    
public:
    /*!
        Weapon constructor.
    */
    Weapon(void);

    /*!
        Bullet collision handler.
        \param entity the entity that collided with the bullet.
        \param col the collision bitmap.
    */
    void Collision(Entity *entity, Uint32 &col);
    
    /*!
        Weapon shot.
        \param Rect starting rectangle.
        \param sound flag to play sound.
        \return true if success, false otherwise.
    */
    bool Shot(Rect rect, bool sound);
    
    /*!
        Shoot update.
        \return true if success, false otherwise.
    */
    bool Update(void);
    
    /*!
        Shoot draw.
        \return true if success, false otherwise.
    */
    bool Draw(void);
};

#endif
