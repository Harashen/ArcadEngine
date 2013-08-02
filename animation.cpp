#include "animation.h"


CAnimation::CAnimation(void)
{
	/* Initialize variables */
	Frames        = 0;
	FrameRate     = 101;
	CurrFrame     = 0;
	CurrAnimation = 0;
	Oscillate     = false;

	/* Reset animation */
	Reset();
}

void CAnimation::Reset(void)
{
	/* Reset variables */
	FrameInc = 1;
	OldTime  = 0;
}

void CAnimation::ResetFrame(void)
{
	/* Reset current frame */
    CurrFrame = 0;
}

void CAnimation::Update(bool idle)
{
	/* Animation idle */
	if (idle)
		return;

	Uint32 ticks = SDL_GetTicks();

	/* Keep current frame */
	if ((OldTime + FrameRate) > ticks)
		return;

	/* Set time */
	OldTime = ticks;

	/* Select next frame */
	CurrFrame += FrameInc;

	/* Animation oscillation */
	if (Oscillate) {
		/* Modify increment */
		if ((FrameInc > 0) && (CurrFrame >= (Frames - 1)))
			FrameInc = -FrameInc;
		if ((FrameInc < 0) && (CurrFrame <= 0))
			FrameInc = -FrameInc;
	}

	/* Check limit */
	if (CurrFrame >= Frames)
		ResetFrame();
}

Uint32 CAnimation::GetAnimation(void)
{
	/* Get current animation */
    return CurrAnimation;
}

void CAnimation::SetOscillate(bool value)
{
	/* Set oscillate */
    Oscillate = value;
}

void CAnimation::SetAnimation(Uint32 value)
{
	/* Set current animation */
    CurrAnimation = value;
}
