#include "weapon.hpp"


Weapon::Weapon(void)
{
    /* Set attributes */
    mType   = "Weapon";
    mMotion = ENTITY_UP;
    mState  = ENTITY_DEAD;
    mShow   = false;
}

void Weapon::Collision(Entity *entity, Uint32 &col)
{
    string type = entity->GetType();

    /* Default handler */
    Entity::Collision(entity, col);

    /* Collision with item/player */
    if (!type.compare("Item") || !type.compare("Player")) {
        /* Ignore collision */
        col = 0;
    }

    /* Kill bullet */
    if (col) {
        mShow = false;
        Kill();
        mSound.Stop();
    }
}

bool Weapon::Shot(Rect rect, bool sound)
{
    /* One shot */
    if (!(mState & ENTITY_DEAD)) return false;
        
    float x = rect.GetX() + (rect.GetWidth() * 0.5);
    float y = rect.GetY() + 5;
    
    /* Set start */
    SetStart(x, y);
    
    /* Shot */
    ResetRect();
    mShow  = true;
    mState = ENTITY_WALK;
    
    /* Start motion */
    Run();
    
    if (sound) mSound.Play(0);
        
    return true;
}

bool Weapon::Update(void)
{
    if (!mShow)	return false;
    
    Entity::Update();
    
    return true;
}

bool Weapon::Draw(void)
{
    if (!mShow || (mState & ENTITY_DEAD)) {
        return false;
    }
    
    /* Update animation */	
    mAnimation.Update(false);
    
    /* Draw Animation */
    mAnimation.Draw(NULL, &mRect);
    
    return true;
}
