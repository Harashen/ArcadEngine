#include <cmath>

#include <boost/format.hpp>

#include "entity.hpp"
#include "collision.hpp"
#include "level.hpp"
#include "utils.hpp"
#include "video.hpp"

using namespace boost;


Entity::Entity(void)
{
    /* Default type */
    mType = "Entity";

    /* Initialize variables */
    mCollisionRect = 0;
    mStartX        = 0.0;
    mStartY        = 0.0;
    mPoints	       = 0;
    mCollidable    = false;
    mLoaded        = false;
    mNoGravity     = false;	
    mEntityNumber  = 0;
    mId            = 0;
    
    mState  = ENTITY_IDLE;
    mMotion = ENTITY_INACTIVE;
}

Entity::~Entity(void)
{
    /* Unload */
    Unload();
    
    /* Unset loaded */
    mLoaded = false;
}

bool Entity::LoadSettings(const char *filepath)
{
    Script script;
    bool   ret;
    string sound;

    /* Load script */
    ret = script.Load(filepath);
    if (!ret) return false;
        
    /* Set name */
    mName = script.GetString("Name");

    /* Set dimensions */
    mWidth  = script.GetValue<Uint16>("Width");
    mHeight = script.GetValue<Uint16>("Height");
    
    /* Initial position */
    mStartX = script.GetValue<float>("StartX");
    mStartY = script.GetValue<float>("StartY");
    
    /* Set sprite */
    mSprite = script.GetString("Sprite");
        
    /* Set speed */
    mSpeed = script.GetValue<float>("Speed");

    /* Set acceleration */
    mAccelX = script.GetValue<float>("AccelX");
    mAccelY = script.GetValue<float>("AccelY");

    /* Set animation */
    bool oscillate = script.GetBool("Oscillate");
    mAnimation.SetOscillate(oscillate);
    
    /* Set if gravity affects entity */
    mNoGravity = script.GetBool("Gravity");
    
    /* Motion values */
    mAniMap[ENTITY_LEFT]  = script.GetValue<Uint16>("Left");
    mAniMap[ENTITY_RIGHT] = script.GetValue<Uint16>("Right");
    mAniMap[ENTITY_UP]    = script.GetValue<Uint16>("Up");
    mAniMap[ENTITY_DOWN]  = script.GetValue<Uint16>("Down");
    mAniMap[ENTITY_STOP]  = script.GetValue<Uint16>("Stop");
    
    /* Load sound */
    sound = script.GetString("Sound");
    if (!sound.empty())	mSound.Load(mBasepath + sound);
    
    /* Set points */
    mPoints = script.GetValue<Uint32>("Points");
    
    /* Set attributes */
    mCollidable = script.GetBool("Collidable");
    
    /* Load collisions */
    ret = LoadCollisions(script);
    if (!ret) return false;
    
    return true;
}

bool Entity::LoadCollisions(Script script)
{
    mCollisionRect = script.GetValue<Uint16>("Collisions");
    
    /* No collision rectangles */
    if (!mCollisionRect) {
        Rect *rect = new Rect;
        
        // Si no se ha creado el rectángulo
        if (!rect) return false;
        
        mCollisions.push_back(rect);
        
        rect->SetX(0);
        rect->SetY(0);
        rect->SetInitialX(0);
        rect->SetInitialY(0);
        rect->SetWidth(mWidth);
        rect->SetHeight(mHeight);
    } else {
        for (Uint16 i = 0; i < mCollisionRect; i++) {
            Rect *rect = NULL;
            string suffix;
            Sint32 w;
            Sint32 h;
            float  x;
            float  y;
            
            /* Initialize values */
            w = h = 0;
            x = y = 0.0;
            
            suffix = str(format("[%d]") % i);
            
            rect = new Rect;
            
            if (!rect) continue;
            
            mCollisions.push_back(rect);
            
            /* Get attributes */
            w = script.GetValue<Sint32>("Width" + suffix);
            h = script.GetValue<Sint32>("Height" + suffix);
            x = script.GetValue<Sint32>("X" + suffix);
            y = script.GetValue<Sint32>("Y" + suffix);
            
            /* Set rectangle */
            rect->SetX(x);
            rect->SetY(y);
            rect->SetInitialX(x);
            rect->SetInitialY(y);
            rect->SetWidth(w);
            rect->SetHeight(h);
        }
    }
    
    return true;
}

void Entity::Reset(void)
{
    /* Set direction */
    mMotion = DEFAULT;

    /* Set force/acceleration */
    mForceX = 0.0;
    mForceY = 0.0;
    mAccelX = 0.0;
    mAccelY = 0.0;
    
    /* Set speed */
    mSpeedX = 0.0;
    mSpeedY = 0.0;

    /* Set position */
    mRect.SetX(mStartX);
    mRect.SetY(mStartY);
    
    /* Set state */
    mState = ENTITY_IDLE;

    /* Reset animation */
    mAnimation.Reset();
}

bool Entity::Load(const char *filepath)
{
    bool ret;

    /* Load settings */
    ret = LoadSettings(filepath);
    if (!ret) return false;

    /* Load animation */
    ret = mAnimation.Load(mBasepath + mSprite, mWidth, mHeight);
    if (!ret) return false;

    /* Set rectangle dimension */
    mRect.SetWidth(mWidth);
    mRect.SetHeight(mHeight);

    /* Set loaded */
    mLoaded = true;

    /* Reset entity */
    Reset();

    return true;
}

bool Entity::Load(string filepath)
{
    const char *c = filepath.c_str();

    /* Load entity */
    return Load(c);
}

void Entity::Unload(void)
{
    /* Not loaded */
    if (!mLoaded) return;

    /* Unload animation */
    mAnimation.Unload();
    
    /* Clear collision vector */
    mCollisions.clear();
}

void Entity::Collision(Entity *Entity, Uint32 &col)
{
    /* Entity non-collidable */
    if (!Entity->mCollidable) {
        col = 0;
    }
}

void Entity::Physics(void)
{
    /* Walk/Run force */
    if (mState & ENTITY_WALK) {
        /* Apply force */
        if (mForceX < 0 && mSpeedX > -mSpeed) {
            mSpeedX += mForceX;
        }

        if (mForceX > 0 && mSpeedX < mSpeed) {
            mSpeedX += mForceX;
        }
            
        /* No affects gravity */
        if (mNoGravity) {
            /* Apply force */
            if (mForceY < 0 && mSpeedY > -mSpeed) {
                mSpeedY += mForceY;
            }

            if (mForceY > 0 && mSpeedY < mSpeed) {
                mSpeedY += mForceY;
            }
        }
    }

    /* Level forces */
    if (gpLevel) {
        float gravity  = gpLevel->GetGravity();
        float friction = gpLevel->GetFriction();
        
        /* Friction force */
        if (!(mState & ENTITY_WALK)) {
            if (mSpeedX < 0) {
                mSpeedX += friction;
            }

            if (mSpeedX > 0) {
                mSpeedX -= friction;
            }
        }
        
        /* Gravity/Friction force */
        if (!mNoGravity) {
            if (mSpeedY > -mAccelY) {
                mSpeedY -= gravity;
            }
        } else {
            if (!(mState & ENTITY_WALK)) {
                if (mSpeedY < 0) {
                    mSpeedY += friction;
                }

                if (mSpeedY > 0) {
                    mSpeedY -= friction;
                }
            }
        }
    }
}

bool Entity::Update(void)
{
    const float step = 0.1;

    /* Entity is dead */
    if (mState & ENTITY_DEAD)
        return false;

    /* Apply physics */
    Physics();

    /* Round speed */
    if (abs(mSpeedX) < step) {
        mSpeedX = 0;
    }

    if (abs(mSpeedY) < step) {
        mSpeedY = 0;
    }

    /* Iterations */
    float loops = MaxAbs<float>(mSpeedX, mSpeedY);

    /* Entity movement */
    for (float i = 0; i < loops; i += step) {
        Uint32 col;

        /* Detect collision */
        col = Colision::Detect(this);

        /* X coordinate */
        if (i < abs(mSpeedX)) {
            /* Collision */
            if ((col & COLLISION_LEFT  && mSpeedX < 0) ||
                (col & COLLISION_RIGHT && mSpeedX > 0)) {

                mSpeedX = 0;
            }
                    
            float value = mRect.GetX();

            /* Move entity */
            if (mSpeedX > 0) {
                value += step;
                mRect.SetX(value);
            }
            
            if (mSpeedX < 0) {
                value -= step;
                mRect.SetX(value);
            }
        }

        /* Y coordinate */
        if (i < abs(mSpeedY)) {
            /* Collision */
            if ((col & COLLISION_BOTTOM && mSpeedY < 0) ||
                (col & COLLISION_TOP    && mSpeedY > 0)) {

                mSpeedY = 0;
            }

            float value = mRect.GetY();

            /* Move entity */
            if (mSpeedY > 0) {
                value += step;
                mRect.SetY(value);
            }
            
            if (mSpeedY < 0) {
                value -= step;
                mRect.SetY(value);
            }
        }
    }
    
    return true;
}

bool Entity::Draw(bool idle)
{
    if (mState & ENTITY_DEAD) return false;

    Rect srRect;
    
    /* Set rect */
    srRect.SetX(0.0);
    srRect.SetY(0.0);
    srRect.SetWidth(mRect.GetWidth());
    srRect.SetHeight(mRect.GetHeight());
    
    if (mState & ENTITY_IDLE) {
        idle = true;
    }

    /* Animate entity */
    mAnimation.Update(idle);

    /* Draw entity */
    return mAnimation.Draw(&srRect, &mRect);
}

void Entity::Move(Uint16 dir)
{	
    if (!mNoGravity){
        if (dir & ENTITY_UP) {
            Uint32 col;

            /* Detect collision */
            col = Colision::Detect(this);
            
            if (col & COLLISION_BOTTOM) {
                mSpeedY = mAccelY;
            }
        }
    } else {
        if (dir & ENTITY_DOWN) {
            mForceY = -mAccelY;
        }
        
        if (dir & ENTITY_UP) {
            mForceY = mAccelY;
        }
    }
    
    /* Set X acceleration */
    if (dir & ENTITY_LEFT) {
        mForceX = -mAccelX;
    }
    
    if (dir & ENTITY_RIGHT) {
        mForceX = mAccelX;
    }

    /* Set walk state */
    mState |= ENTITY_WALK;
}

void Entity::Run()
{
    mAnimation.SetFlipX(false);
    mAnimation.SetFlipY(false);
    
    switch (mMotion) {
        case ENTITY_LEFT:
            if (mAniMap[ENTITY_LEFT]) {
                mAnimation.SetAnimation(mAniMap[ENTITY_LEFT] - 1);
            } else {
                mAnimation.SetAnimation(mAniMap[ENTITY_RIGHT] - 1);
                mAnimation.SetFlipX(true);
            }

            break;
            
        case ENTITY_RIGHT:
            if (mAniMap[ENTITY_RIGHT]) {
                mAnimation.SetAnimation(mAniMap[ENTITY_RIGHT] - 1);
            } else {
                mAnimation.SetAnimation(mAniMap[ENTITY_LEFT] - 1);
                mAnimation.SetFlipX(true);
            }

            break;
            
        case ENTITY_UP:
            if (mAniMap[ENTITY_UP]) {
                mAnimation.SetAnimation(mAniMap[ENTITY_UP] - 1);
            } else {
                mAnimation.SetAnimation(mAniMap[ENTITY_DOWN] - 1);
                mAnimation.SetFlipY(true);
            }

            break;
            
        case ENTITY_DOWN:
            if (mAniMap[ENTITY_DOWN]) {
                mAnimation.SetAnimation(mAniMap[ENTITY_DOWN] - 1);   
            } else {
                mAnimation.SetAnimation(mAniMap[ENTITY_UP] - 1);
                mAnimation.SetFlipY(true);
            }

            break;

        /*case ENT_STOP:
            mAnimation.SetAnimation(mAniMap[ENT_STOP] - 1);
            break;*/
    }
    
    
    /* Entity move */
    Move(mMotion);
}

void Entity::Stop(void)
{
    mSpeedX *= 0.5;
    mSpeedY *= 0.5;
    
    /* Unset walk state */
    mState &= ~ENTITY_WALK;
    mState |=  ENTITY_IDLE;
}

void Entity::Kill(void)
{
    /* Already dead */
    if (mState & ENTITY_DEAD) return;

    /* Set dead state */
    mState |= ENTITY_DEAD;
}

string Entity::GetType(void)
{
    return mType;
}
    
string Entity::GetName(void)
{
    return mName;
}

Uint32 Entity::GetPoints(void)
{
    return mPoints;
}

Rect Entity::GetRect(void)
{
    return mRect;
}

bool Entity::GetCollidable(void)
{
    return mCollidable;
}

Uint16 Entity::GetState(void)
{
    return mState;
}

Uint16 Entity::GetMotion(void)
{
    return mMotion;
}

Uint16 Entity::GetId(void)
{
    return mId;
}

Uint16 Entity::GetEntityNumber(void)
{
    return mEntityNumber;
}

void Entity::SetMotion(Uint16 value)
{
    mMotion = value;
}

void Entity::SetStart(float x, float y)
{
    mStartX = x;
    mStartY = y;
}

void Entity::SetState(Uint16 value)
{
    mState = value;
}

void Entity::SetId(Uint16 value)
{
    mId = value;
}

void Entity::SetBasepath(string value)
{
    mBasepath = value;
}

void Entity::ResetRect(void)
{
    mRect.SetX(mStartX);
    mRect.SetY(mStartY);
}