#include <boost/format.hpp>

#include "character.hpp"
#include "level.hpp"

using namespace boost;


Character::Character(void)
{
    /* Set type */
    mType = "Character";
}

void Character::Reset(void)
{
    /* Reset speed */
    mSpeedX = 0.0;
    mSpeedY = 0.0;

    /* Reset position */
    mRect.SetX(mStartX);
    mRect.SetY(mStartY);
}

bool Character::LoadSettings(const char *path)
{
    Script script;
    bool   ret;

    /* Load entity settings */
    ret = Entity::LoadSettings(path);    
    if (!ret) return false;

    /* Load script */
    ret = script.Load(path);
    if (!ret) return false;

    /* Set animations */
    mEntityNumber = script.GetValue<Uint16>("Entity_Number");
    mId           = script.GetValue<Uint16>("Id");
    
    if (!mState) {
        mState = script.GetValue<Uint16>("State");
    }

    return true;
}

bool Character::Load(const char *path)
{
    bool ret;

    /* Load entity settings */
    ret = LoadSettings(path);
    if (!ret) return false;
    
    /* Load animation */
    mAnimation.Load(mBasepath + mSprite, mWidth, mHeight);
    
    /* Set position */
    mRect.SetX(mStartX);
    mRect.SetY(mStartY);
    
    mRect.SetWidth(mWidth);
    mRect.SetHeight(mHeight);
    
    /* Reset */
    Reset();
    
    mLoaded = true;

    return true;
}

bool Character::Load(string path)
{
    const char *c = path.c_str();
    
    /* Load character */
    return Load(c);
}

bool Character::Update(void)
{
    Sint16 w = gpLevel->GetWidth();
    Sint16 h = gpLevel->GetHeight();
    
    /* Move character */
    bool ret = Entity::Update();
    if (!ret) return false;

    /* Check limits */
    if (mRect.GetX() >= w - mWidth) {
        mRect.SetX(w - mWidth);    
    }
        
    if (mRect.GetY() >= h - mHeight) {
        mRect.SetY(h - mHeight);
    }
    
    if (mRect.GetX() < 0) {
        mRect.SetX(0);
    }
        
    if (mRect.GetY() < 0) {
        mRect.SetY(0);
    }
        
    return true;
}

bool Character::Draw(bool idle)
{
    /* Character dead */
    if (mState & ENTITY_DEAD) {
        return false;
    }
    
    /* Character idle */
    if (!mState) {
        idle = true;
        
        /* Set stop animation */
        if (mAniMap[ENTITY_STOP]) {
            mAnimation.SetAnimation(mAniMap[ENTITY_STOP] - 1);
        }
    }
    
    mAnimation.Update(idle);
    
    bool ret = mAnimation.Draw(NULL, &mRect);
    if (!ret) return false;
    
    return true;
}
