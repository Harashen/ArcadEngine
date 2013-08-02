#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL/SDL.h>

#include "ai.h"
#include "audio.h"
#include "camera.h"
#include "entity.h"
#include "hud.h"
#include "level.h"
#include "joystick.h"
#include "keyboard.h"
#include "player.h"
#include "storyboard.h"
#include "video.h"
#include "weapon.h"

enum {
    WAY_2_VERTICAL   = 1,
    WAY_2_HORIZONTAL = 2,
    WAY_4            = 4,
    WAY_8            = 8
};

/*!
	CEngine is the main class of the engine. It manages the
	construction and destruction of the main objects and
	executes the sequence of scenes.
	\author Javier Bermejo García
*/
class CEngine {
	CAi       *Ai;			//!< AI object.
	CAudio    *Audio;		//!< Audio object.
	CCamera   *Camera;		//!< Camera object.
	CHud      *Hud;			//!< Hud object.
	CLevel    *Level;		//!< Level object.
	CKeyboard *Keyboard;	//!< Keyboard object.
	CJoystick *Joystick;	//!< Joystick object.
	CPlayer   *Player;		//!< Player object.
	CStory    *Story;		//!< Storyboard object.
	CVideo    *Video;		//!< Video object.

	bool Exit;		//!< Exit flag.
	bool MenuExit;	//!< Flag to exit the menu.
	bool Pause;		//!< Pause flag.

	Uint16 Width;		//!< Video width dimension.
	Uint16 Height;		//!< Video height dimension.
	
	string GameName;	//!< Game name.
	string Basepath;	//!< Base path.

	Uint16 Ways;		//!< Player movement types, 2, 4 or 8 ways.
	Uint16 Credits;		//!< In-game credits that represents lives.

private:
	/*!
		Load the engine settings.
		\return true if success, false otherwise.
	*/
	bool LoadSettings(void);
	
	/*!
		Delivery points to the player and displays.
	*/
	void DeliveryPoints(void);

	/*!
		Manage the engine events.
	*/
	void Events(void);

	/*!
		Show the main menu screen.
		\return true if success, false otherwise.
	*/
	bool Menu(void);

	/*!
        Manage the input with 2 horizontal ways.
    */
    void Input_2Horizontal(void);
    
    /*!
        Manage the input with 2 vertical ways.
    */
    void Input_2Vertical(void);
    
    /*!
        Manage the input with 4 ways.
    */
    void Input_4Ways(void);

	/*!
		Manage the in-game input.
	*/
	void IngameInput(void);
	
	/*!
		Pause the in-game execution.
	*/
	void IngamePause(void);
	
	/*!
		Update the in-game objects.
	*/
	void IngameUpdate(void);

	/*!
		Draw the in-game objects.
	*/
	void IngameDraw(void);

	/*!
		Run the in-game sequence.
		\return the different way to exit. Dead, end of level...
	*/
	Sint16 Ingame(void);

	/*!
		Show the credits scene.
	*/
	void ShowCredits(void);

public:
	/*!
		CEngine constructor.
	*/
	CEngine(void);

	/*!
		CEngine destructor.
	*/
	~CEngine(void);

	/*!
		Initialize the engine subsystems.
		\return true if success, false otherwise.
	*/
	bool Init(void);

	/*!
		Configure the engine subsystems using the loaded parameters.
		\return true if success, false otherwise.
	*/
	bool Configure(void);

	/*!
		Run the engine main sequence.
	*/
	void Run(void);
};

#endif
