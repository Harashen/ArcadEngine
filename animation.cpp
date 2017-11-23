#include "animation.hpp"


Animation::Animation(void)
{
    /* Initialize variables */
    mFrames        = 0;
    mFrameRate     = 101;
    mCurrFrame     = 0;
    mCurrAnimation = 0;
    mOscillate     = false;

    /* Reset animation */
    Reset();
}

void Animation::Reset(void)
{
    /* Reset variables */
    mFrameInc = 1;
    mOldTime  = 0;
}

void Animation::ResetFrame(void)
{
    /* Reset current frame */
    mCurrFrame = 0;
}

void Animation::Update(bool idle)
{
    /* Animation idle */
    if (idle) return;

    Uint32 ticks = SDL_GetTicks();

    /* Keep current frame */
    if ((mOldTime + mFrameRate) > ticks) {
        return;
    }

    /* Set time */
    mOldTime = ticks;

    /* Select next frame */
    mCurrFrame += mFrameInc;

    /* Animation oscillation */
    if (mOscillate) {
        /* Modify increment */
        if ((mFrameInc > 0) && (mCurrFrame >= (mFrames - 1))) {
            mFrameInc = -mFrameInc;
        }

        if ((mFrameInc < 0) && (mCurrFrame <= 0)) {
            mFrameInc = -mFrameInc;        
        }
    }

    /* Check limit */
    if (mCurrFrame >= mFrames) {
        ResetFrame();
    }
}

Uint32 Animation::GetAnimation(void)
{
    /* Get current animation */
    return mCurrAnimation;
}

void Animation::SetOscillate(bool value)
{
    /* Set oscillate */
    mOscillate = value;
}

void Animation::SetAnimation(Uint32 value)
{
    /* Set current animation */
    mCurrAnimation = value;
}
