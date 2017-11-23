#include "keyboard.hpp"

Keyboard::Keyboard()
{
    /* Enable key repeat */
    SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

void Keyboard::Listen(Uint32 key, bool pressed)
{        
    /* Set state */
    if (pressed) {
        mKeys[key] = true;
        mKeysPressed[key] = SDL_GetTicks();
    } else {
        mKeys[key] = false;
        mKeysPressed[key] = 0;
    }
}

bool Keyboard::IsPressed(Uint32 key, bool once)
{
    bool ret;
    
    /* Get key state */
    ret = mKeys[key];
    
    /* Clear key state */
    if (once) {
        mKeys[key] = false;
    }
    
    return ret;
}
