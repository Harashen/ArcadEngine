#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"
#include "weapon.h"


/*!
	CPlayer defines a character (an entity) that works
	as the human-controlled entity.
	\author Javier Bermejo García
*/
class CPlayer : public CCharacter {
	Uint32 Score;		//!< Player score.
	Uint32 LevelScore;	//!< Player level score.
	Uint32 Lives;		//!< Player lives.

	CWeapon *Weapon;	//!< Player weapon.
	
	Sint32 Position;	//!< Weapon position.
	Sint32 Bullets;		//!< Number of bullets.

public:
	/*!
		CPlayer constructor.
	*/
	CPlayer(void);
	
	/*!
		CPlayer destructor.
	*/
	~CPlayer(void);

	/*!
		Set the active player object.
	*/
	void SetActive(void);

	/*!
		Player collision handler.
		\param Entity the entity that collided with the player.
		\param col the collision bitmap.
	*/
	void Collision(CEntity *Entity, Uint32 &col);
	
	/*!
		Load player.
		\param filepath the settings file path.
		\return true if success, false otherwise.
	*/
	bool Load(string filepath);
	
	/*!
		Load player weapon.
		\param filepath the settings file path.
	*/
	void LoadWeapon(string filepath);
	
	/*!
        Weapon shot.
	*/
	void Shot(void);
	
	/*!
		Reset the player.
	*/
	void Reset(void);
	
	/*!
		Update the player state.
		\return true if success, false otherwise.
	*/
	bool Update(void);
	
	/*!
		Draw the player on the screen.
		\param idle if the character is idle.
		\return true if success, false otherwise.
	*/
	bool Draw(bool idle = false);
	
	/*!
		The player lose a life.
	*/
	void LoseLife(void);
	
	/*!
        Get level score.
        \return the level score.
    */
    Uint32 GetLevelScore(void);
    
	/*!
		Get player lives.
		\return the number of lives.
	*/
    Uint32 GetLives(void);
	
	/*!
        Get the score.
        \return the score.
    */
    Uint32 GetScore(void);
    
	/*!
		Set the number of lives.
		\param value the number of lives.
	*/
	void SetLives(Uint32 value);
	
	/*!
		Set the score.
		\param value the new score.
	*/
	void SetScore(Uint32 value);
    
	/*!
		Reset score.
	*/
	void ResetScore(void);
};

//! Active player object.
extern CPlayer *pPlayer;

#endif
