#include "joystick.hpp"

Joystick::Joystick(void)
{
    mpJoystick = NULL;
    
    SDL_Init(SDL_INIT_JOYSTICK);
    
    /* Load joystick */
    if (SDL_NumJoysticks() >= 1) {
        mpJoystick = SDL_JoystickOpen(0);
        SDL_JoystickEventState(SDL_ENABLE);
        
        /* Get number of buttons */
        mButtonsNumber = SDL_JoystickNumButtons(mpJoystick);
    }
    
    if (mpJoystick) {
        mLoaded = true;
    } else {
        mLoaded = false;
    }
    
    mTime = SDL_GetTicks();
}

Joystick::~Joystick(void)
{
    /* Unload joystick */
    if (SDL_JoystickOpened(0)) {
        SDL_JoystickClose(mpJoystick);
    }
}

void Joystick::GetAxis(Sint16 &x, Sint16 &y, bool once)
{   
    /* Get axis */
    x = SDL_JoystickGetAxis(mpJoystick, 0);
    y = SDL_JoystickGetAxis(mpJoystick, 1);
    
    /* One move */
    if (once) {
        Uint64 time = SDL_GetTicks();
        
        if (mTime <= time) {
            mTime = time + 350;
        } else {
            x = 0;
            y = 0;
        }    
    }
}

void Joystick::Listen(Uint32 button, bool pressed)
{
    /* Set state */
    if (pressed) {
        mButtons[button] = true;
        mButtonsPressed[button] = SDL_GetTicks();
    } else {
        mButtons[button] = false;
        mButtonsPressed[button] = 0;
    }
}

bool Joystick::IsPressed(Uint32 button, bool once)
{
    bool ret;
    
    /* Get key state */
    ret = mButtons[button];
    
    /* Clear key state */
    if (once) {
        mButtons[button] = false;
    }
    
    return ret;
}

bool Joystick::IsLoaded(void)
{
    return mLoaded;
}

Uint32 Joystick::GetButtonsNumber(void)
{
    return mButtonsNumber;
}
