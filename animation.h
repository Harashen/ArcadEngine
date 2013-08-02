#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <SDL/SDL.h>


/*!
	CAnimation is the class to control any kind of
	animation (sprites, loops...).
	\author Javier Bermejo García
*/
class CAnimation {
	Sint32 FrameInc;		//!< Frame increment per update.
	Uint32 OldTime;			//!< Time of the last frame.

protected:
	Uint32 CurrFrame;		//!< Current animation frame.
	Uint32 CurrAnimation;	//!< Current animation.
	
	Uint32 Frames;			//!< Number of total frames per animation.
	Uint32 Animations;		//!< Number of animations.
	Uint32 FrameRate;		//!< Framerate speed.
	bool   Oscillate;		//!< Oscillate attribute.
	
private:
	/*!
        Reset current animation frame.
    */
    void ResetFrame(void);

public:
	/*!
		CAnimation constructor.
	*/
	CAnimation(void);

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

