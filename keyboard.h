#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <map>
#include <SDL/SDL.h>

using namespace std;

/*!
    CKeyboard is the class that manages the input keyboard subsystem
	to have support for HID (Human Interface Device) devices.
    \author Javier Bermejo García
*/
class CKeyboard{
private:
    map<Uint32, bool>   Keys;        //!< Pressed keys map.
    map<Uint32, Uint32> KeysPressed; //!< Time map pressed keys.
    
public:
    /*!
		CKeyboard constructor.
	*/
    CKeyboard();
    
    /*!
        Set a key state into the stack.
        \param key the keycode that was pressed.
        \param pressed true if pressed, false if unpressed.
    */
    void Listen(Uint32 key, bool pressed);
    
    /*!
		Check if a key has been pressed.
		\param key the keycode to check.
		\param once only check once.
		\return true if pressed, false otherwise.
	*/
    bool IsPressed(Uint32 key, bool once = false);
};

#endif

