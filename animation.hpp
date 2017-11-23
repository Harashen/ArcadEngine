#ifndef _ANIMATION_HPP_
#define _ANIMATION_HPP_

#include <SDL/SDL.h>


/*!
    Animation is the class to control any kind of
    animation (sprites, loops...).
    \author Javier Bermejo García
*/
class Animation {
    Sint32 mFrameInc;		//!< Frame increment per update.
    Uint32 mOldTime;		//!< Time of the last frame.

protected:
    Uint32 mCurrFrame;		//!< Current animation frame.
    Uint32 mCurrAnimation;	//!< Current animation.
    
    Uint32 mFrames;			//!< Number of total frames per animation.
    Uint32 mAnimations;		//!< Number of animations.
    Uint32 mFrameRate;		//!< Framerate speed.
    bool   mOscillate;		//!< Oscillate attribute.
    
private:
    /*!
        Reset current animation frame.
    */
    void ResetFrame(void);

public:
    /*!
        Animation constructor.
    */
    Animation(void);

    /*!
        Reset animation.
    */
    void Reset(void);

    /*!
        Update animation.
        \param idle if is a static sprite.
    */
    void Update(bool idle);
    
    /*!
        Get current animation.
        \return current animation.
    */
    Uint32 GetAnimation(void);
     
    /*!
        Set oscillate variable.
        \param value true to oscillate, false otherwise.
    */
    void SetOscillate(bool value);
    
    /*!
        Set current animation.
        \param value current animation.
    */
    void SetAnimation(Uint32 value);
};

#endif

