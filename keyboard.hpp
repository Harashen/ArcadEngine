#ifndef _KEYBOARD_HPP_
#define _KEYBOARD_HPP_

#include <map>

#include <SDL/SDL.h>

using namespace std;

/*!
    Keyboard is the class that manages the input keyboard subsystem
    to have support for HID (Human Interface Device) devices.
    \author Javier Bermejo Garc�a
*/
class Keyboard{
private:
    map<Uint32, bool>   mKeys;        //!< Pressed keys map.
    map<Uint32, Uint32> mKeysPressed; //!< Time map pressed keys.
    
public:
    /*!
        Keyboard constructor.
    */
    Keyboard();
    
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
