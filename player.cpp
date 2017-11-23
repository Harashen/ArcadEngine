#include "player.hpp"
#include "collision.hpp"

/* Player handler */
Player *gpPlayer = NULL;


Player::Player(void)
{
    /* Set type */
    mType = "Player";

    /* Initialize variables */
    mCollidable = true;
    mScore      = 0;
    mLevelScore = 0;
    mLives	    = 0;
    mBullets    = 3;
    
    mpWeapon = new Weapon[mBullets];
}

Player::~Player(void)
{
    if (mpWeapon) delete mpWeapon;
}

void Player::SetActive(void)
{
    /* Active player */
    gpPlayer = this;
}

void Player::Collision(Entity *entity, Uint32 &col)
{
    string type = entity->GetType();

    /* Default handler */
    Entity::Collision(entity, col);

    /* Collision with enemy */
    if (!type.compare("Enemy")) {
        if (col) {
            /* Kill player */
            Kill();
            mSound.Play(0);
            
            /* Lose life */
            LoseLife();
        }
    }

    /* Collision with item */
    if (!type.compare("Item")) {
        mScore += entity->GetPoints();
    }
}

bool Player::Load(string filepath)
{    
    SetBasepath(filepath);
    
    /* Load player configuration */
    bool ret = Character::Load(filepath + "player.ini");
    if (!ret) return false;
    
    for (int i = 0; i < mBullets; i++) {
        mpWeapon[i].SetBasepath(filepath);
    }
        
    LoadWeapon(filepath + "weapon.ini");
    
    return true;
}
    
void Player::LoadWeapon(string filepath)
{
    mpWeapon[0].Load(filepath);
    
    for (int i = 1; i < mBullets; i++) {
        mpWeapon[i] = mpWeapon[0];
    }
}

void Player::Shot(void)
{
    /* Already dead */
    if (mState == ENTITY_DEAD) return;
        
    for (int i = 0; i < mBullets; i++) {
        bool sound = false;
        Rect rectW = mRect;
        
        if (i == 0) {
            sound = true;
            rectW.SetY(rectW.GetY() + 10);
        } else {
            float x = rectW.GetX();
            
            /* Right/Left bullets */
            if (i % 2 == 0) {
                rectW.SetX(x + 10);
            } else {
                rectW.SetX(x - 10);
            }
        }
        
        /* Shot bullet */
        if (mpWeapon[i].Shot(rectW, sound) == false) {
            break;
        }
    }
}

void Player::Reset(void)
{
    /* Reset weapon */
    for (int i = 0; i < mBullets; i++) {
        mpWeapon[i].SetState(ENTITY_DEAD);
    }
    
    /* Reset character */
    Character::Reset();
    
    mLevelScore = 0;
    mState      = ENTITY_IDLE;
    
    /* Stop sound */
    mSound.Stop();
}

bool Player::Update(void)
{	
    /* Update character */
    Character::Update();
    
    /* Already dead */
    if (mState & ENTITY_DEAD) return false;
    
    /* Update bullets */
    for (int i = 0; i < mBullets; i++) {
        bool ret = mpWeapon[i].Update();
        
        /* Bullets dead */
        if (!ret) {
            /* Reset weapon */
            for (int j = 0; j < mBullets; j++) {
                mpWeapon[j].SetState(ENTITY_DEAD);
            }
            
            return true;
        }
    }
    
    return true;
}

bool Player::Draw(bool idle)
{	
    /* Draw bullets */
    for (int i = 0; i < mBullets; i++) {
        mpWeapon[i].Draw();
    }
        
    /* Draw character */
    return Character::Draw(idle);
}

void Player::LoseLife(void)
{
    if (mLives > 0)	mLives--;
}

Uint32 Player::GetLevelScore(void)
{
    return mLevelScore;
}

Uint32 Player::GetLives(void)
{
    return mLives;
}

Uint32 Player::GetScore(void)
{
    return mScore;
}

void Player::SetLives(Uint32 value)
{
    mLives = value;
}

void Player::SetScore(Uint32 value)
{
    mScore 	    += value;
    mLevelScore += value;
}

void Player::ResetScore(void)
{
    mScore = 0;
}

