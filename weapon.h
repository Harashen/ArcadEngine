#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "character.h"


/*!
	CWeapon defines a type of character (an entity) that can be
	shot by other kind of entities (players, enemies...).
	\author Javier Bermejo García
*/
class CWeapon : public CCharacter {
	bool Show;		//!< Flag to show the shoot.
	
public:
	/*!
		CWeapon constructor.
	*/
	CWeapon(void);

	/*!
		Bullet collision handler.
		\param Entity the entity that collided with the bullet.
		\param col the collision bitmap.
	*/
	void Collision(CEntity *Entity, Uint32 &col);
	
	/*!
        Weapon shot.
        \param Rect starting rectangle.
        \param sound flag to play sound.
        \return true if success, false otherwise.
    */
	bool Shot(CRect Rect, bool sound);
	
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
