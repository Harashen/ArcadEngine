#ifndef _JOYSTICK_HPP_
#define _JOYSTICK_HPP_

#include <cmath>
#include <map>

#include <SDL/SDL.h>

using namespace std;

enum {
    BUTTON_1        = 0,
    BUTTON_2        = 1,
    BUTTON_3        = 2,
    BUTTON_4        = 3,
    BUTTON_5        = 4,
    BUTTON_6        = 5,
    BUTTON_7        = 6,
    BUTTON_8        = 7,
    BUTTON_START    = 8,
    BUTTON_PINBALL  = 9,
    BUTTON_INSERT   = 10,
    BUTTON_RESET    = 11,
    BUTTON_PAUSE    = 12,
    BUTTON_PRNT_SCR = 13,
    BUTTON_EXIT     = 14
};

/*!
    Joystick is the class that manages axis and buttons of a
    joystick as part of the input subsystem.
    \author Javier Bermejo Garc�a
*/
class Joystick {
private:
    Sint16 mJoyX;			 //!< X-axis value.
    Sint16 mJoyY;            //!< Y-axis value.
    Uint32 mButtonsNumber;   //!< Numero de botones del joystick.
    bool   mLoaded;          //!< Loaded flag.
    
    map<Uint32, bool>   mButtons;   		//!< Pressed buttons map.
    map<Uint32, Uint32> mButtonsPressed;	//!< Time map pressed buttons.
    
    SDL_Joystick *mpJoystick;		//!< Joystick handler.
    
    Uint64 mTime;				    //!< Time to adjust the movement of the joystick.

public:
    /*!
        Joystick constructor.
    */
    Joystick(void);
    
    /*!
        Joystick destructor.
    */
    ~Joystick(void);
    
    /*!
        Gets the value of X and Y axis of the joystick.
        \param x the X-axis.
        \param y the Y-axis.
        \param once only check once.
    */
    void GetAxis(Sint16 &x, Sint16 &y, bool once = false);
    
    /*!
        Set a button state into the stack.
        \param button the button that was pressed.
        \param pressed true if pressed, false if unpressed.
    */
    void Listen(Uint32 button, bool pressed);
    
    /*!
        Check if a button has been pressed.
        \param button the button to check.
        \param once only check once.
        \return true if pressed, false otherwise.
    */
    bool IsPressed(Uint32 button, bool once = false);
    
    /*!
        Get the joystick status.
        \return true if joystick exists y false otherwise.
    */
    bool IsLoaded(void);
    
    /*!
        Get the number of buttons.
        \return the number of buttons.
    */
    Uint32 GetButtonsNumber(void);
};

#endif
