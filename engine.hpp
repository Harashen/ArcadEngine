#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include <SDL/SDL.h>

#include "ai.hpp"
#include "audio.hpp"
#include "camera.hpp"
#include "log.hpp"
#include "entity.hpp"
#include "hud.hpp"
#include "level.hpp"
#include "joystick.hpp"
#include "keyboard.hpp"
#include "player.hpp"
#include "storyboard.hpp"
#include "video.hpp"
#include "weapon.hpp"

enum {
    WAY_2_VERTICAL   = 1,
    WAY_2_HORIZONTAL = 2,
    WAY_4            = 4,
    WAY_8            = 8
};

/*!
    Engine is the main class of the engine. It manages the
    construction and destruction of the main objects and
    executes the sequence of scenes.
    \author Javier Bermejo García
*/
class Engine {
    Ai       *mpAi;			//!< AI object.
    Audio    *mpAudio;		//!< Audio object.
    Camera   *mpCamera;		//!< Camera object.
    Log      *mpLog;	    //!< Log object.
    Hud      *mpHud;		//!< Hud object.
    Level    *mpLevel;		//!< Level object.
    Keyboard *mpKeyboard;	//!< Keyboard object.
    Joystick *mpJoystick;	//!< Joystick object.
    Player   *mpPlayer;		//!< Player object.
    Story    *mpStory;		//!< Storyboard object.
    Video    *mpVideo;		//!< Video object.

    bool mExit;		    //!< Exit flag.
    bool mMenuExit;	    //!< Flag to exit the menu.
    bool mPause;		//!< Pause flag.

    Uint16 mWidth;		//!< Video width dimension.
    Uint16 mHeight;		//!< Video height dimension.
    
    string mGameName;	//!< Game name.
    string mBasepath;	//!< Base path.

    Uint16 mWays;       //!< Player movement types, 2, 4 or 8 ways.
    Uint16 mCredits;    //!< In-game credits that represents lives.

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
    bool ShowMenu(void);

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
        Engine constructor.
    */
    Engine(void);

    /*!
        Engine destructor.
    */
    ~Engine(void);

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
