#include "keyboard.h"

CKeyboard::CKeyboard()
{
	/* Enable key repeat */
    SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

void CKeyboard::Listen(Uint32 key, bool pressed)
{        
	/* Set state */
	if (pressed) {
		Keys[key] = true;
		KeysPressed[key] = SDL_GetTicks();
	} 
	else {
		Keys[key] = false;
		KeysPressed[key] = 0;
	}
}

bool CKeyboard::IsPressed(Uint32 key, bool once)
{
    bool ret;
    
    /* Get key state */
    ret = Keys[key];
    
    /* Clear key state */
    if (once)
        Keys[key] = false;
    
    return ret;
}
