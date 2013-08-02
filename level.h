#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <string>
#include <vector>
#include <SDL/SDL.h>

#include "entity.h"

using namespace std;


/*!
	CLevel is the class that defines the in-game levels.
	It also manages the level entities (updating and destroying
	the entities after every update).
	\author Javier Bermejo García
*/
class CLevel {
	float Friction;			//!< Friction force value.
	float Gravity;			//!< Gravity force value.
	
	Uint16 Width;			//!< Level width.
	Uint16 Height;			//!< Level height.
	
	Uint32 Score;			//!< Score to finish level.
	
	float PlayerX;			//!< Player X-axis start position.
	float PlayerY;			//!< Player Y-axis start position.

	bool Finished;			//!< Finished flag.
	bool Loaded;			//!< Loaded flag.

	string Basepath;		//!< Level path.

public:
	vector<CEntity *> EntityList;	//!< Array of level entities.	

protected:
	/*!
		Load the level settings.
		\param filepath the settings filepath.
		\return true if success, false otherwise.
	*/
	bool LoadSettings(string filepath);
	
	/*!
		Load the level entities.
		\return true if success, false otherwise.
	*/
	bool LoadEntities();

public:
	/*!
		CLevel constructor.
	*/
	CLevel(void);

	/*!
		CLevel destructor.
	*/
	~CLevel(void);

	/*!
		Set the active level object.
	*/
	void SetActive(void);

	/*!
		Reset the level.
	*/
	void Reset(void);

	/*!
		Load a level.
		\param filepath the settings filepath.
		\return true if success, false otherwise.
	*/
	bool Load(string filepath);

	/*!
		Unload the level.
	*/
	void Unload(void);

	/*!
		Update the level state (and its entities).
	*/
	void Update(void);

	/*!
		Draw the level (background and entities) on the screen.
		\param idle if the character is idle.
		\return true if success, false otherwise.
	*/
	bool Draw(bool idle = false);

	/*!
        Get friction of the current level.
        \return the friction.
    */
    float GetFriction(void);
    
    /*!
        Get gravity of the current level.
        \return the gravity.
    */
    float GetGravity(void);
	
	/*!
        Get the width of the current level.
        \return the gravity.
    */
    Uint16 GetWidth(void);
    
    /*!
        Get the height of the current level.
        \return the gravity.
    */
    Uint16 GetHeight(void);
    
    /*!
        Get the X position of the player.
        \return the X position of the player.
    */
    float GetPlayerX(void);
    
    /*!
        Get the Y position of the player.
        \return the Y position of the player.
    */
    float GetPlayerY(void);
    
    /*!
        Check if the level is finished.
		\param value to check with the max points of level.
        \return true true if finished, false otherwise.
    */
    bool GetFinished(Uint32 value);
};

//! Active level object.
extern CLevel *pLevel;

#endif
