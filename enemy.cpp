#include "enemy.hpp"
#include "collision.hpp"
#include "player.hpp"


Enemy::Enemy(void)
{
    /* Set type */
    mType = "Enemy";
}

Enemy::~Enemy(void)
{
    /* Stop sound */
    mSound.Stop();
}

void Enemy::Collision(Entity *entity, Uint32 &col)
{
    /* Enemy dead */
    if (mState & ENTITY_DEAD) return;
        
    string type = entity->GetType();

    /* Default handler */
    Entity::Collision(entity, col);

    /* Collision with bullet */
    if (!type.compare("Weapon")) {
        /* Kill enemy */
        mSound.Play(0);
        Kill();
        
        /* Set player points */
        gpPlayer->SetScore(mPoints);
    }

    /* Collision with item */
    if (!type.compare("Item")) col = 0;
        
    /* Collision with enemy */
    if (!type.compare("Enemy")) col = 0;
}
