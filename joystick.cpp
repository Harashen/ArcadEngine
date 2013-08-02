#include "joystick.h"

CJoystick::CJoystick(void)
{
    Joystick = NULL;
    
    SDL_Init(SDL_INIT_JOYSTICK);
    
	/* Load joystick */
    if (SDL_NumJoysticks() >= 1) {
    	Joystick = SDL_JoystickOpen(0);
    	SDL_JoystickEventState(SDL_ENABLE);
		
		/* Get number of buttons */
    	ButtonsNumber = SDL_JoystickNumButtons(Joystick);
    }
    
    if (Joystick)
        Loaded = true;
    else
        Loaded = false;
    
    Time = SDL_GetTicks();
}

CJoystick::~CJoystick(void)
{
	/* Unload joystick */
    if (SDL_JoystickOpened(0) )
        SDL_JoystickClose(Joystick);
}

void CJoystick::GetAxis(Sint16 &x, Sint16 &y, bool once)
{   
	/* Get axis */
    x = SDL_JoystickGetAxis(Joystick, 0);
    y = SDL_JoystickGetAxis(Joystick, 1);
    
    /* One move */
    if (once) {
        Uint64 time = SDL_GetTicks();
        
        if (Time <= time){
            Time = time + 350;
        }
        else {
            x = 0;
            y = 0;
        }    
    }
}

void CJoystick::Listen(Uint32 button, bool pressed)
{
	/* Set state */
	if (pressed) {
		Buttons[button] = true;
		ButtonsPressed[button] = SDL_GetTicks();
	} 
	else {
		Buttons[button] = false;
		ButtonsPressed[button] = 0;
	}
}

bool CJoystick::IsPressed(Uint32 button, bool once)
{
    bool ret;
    
    /* Get key state */
    ret = Buttons[button];
    
    /* Clear key state */
    if (once)
        Buttons[button] = false;
    
    return ret;
}

bool CJoystick::IsLoaded(void)
{
    return Loaded;
}

Uint32 CJoystick::GetButtonsNumber(void)
{
    return ButtonsNumber;
}
