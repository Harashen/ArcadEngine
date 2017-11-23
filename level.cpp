#include <boost/format.hpp>

#include "level.hpp"
#include "enemy.hpp"
#include "item.hpp"
#include "script.hpp"
#include "utils.hpp"

using namespace boost;


/* Level handler */
Level *gpLevel = NULL;


Level::Level(void)
{
    /* Initialize variables */
    mLoaded   = false;
    mFinished = false;
    mGravity  = 0.0;
    mScore    = 0;

    /* Reset */
    Reset();
}

Level::~Level(void)
{
    /* Unload level */
    Unload();
}

void Level::SetActive(void)
{
    /* Set active level */
    gpLevel = this;
}

void Level::Reset(void)
{
    vector<Entity *>::iterator it;

    /* Reset entities */
    foreach (mEntityList, it) {
        Entity *pEntity = *it;

        /* Reset entity */
        pEntity->Reset();
    }

    /* Reset variables */
    mFinished = false;
    mScore    = 0;
}

bool Level::LoadEntities(void)
{
    Script script;
    Uint32 entities;
    
    bool ret;

    /* Load entities script */
    ret = script.Load(mBasepath + "entities.ini");
    if (!ret) return false;

    /* Number of entities */
    entities = script.GetValue<Uint32>("Entities");

    /* Create entities */
    for (Uint32 i = 0; i < entities; i++) {
        Entity *pEntity = NULL;

        /* Generate sufix */
        string sufix = str(format("[%d]") % i);

        /* Get entity info */
        string type = script.GetString("Type" + sufix);

        /* Create entity */
        if (!type.compare("Enemy"))  pEntity = new Enemy;
        if (!type.compare("Entity")) pEntity = new Entity;
        if (!type.compare("Item"))   pEntity = new Item;

        /* Entity not created */
        if (!pEntity) continue;

        /* Push entity */
        mEntityList.push_back(pEntity);
        
        /* Get entity path */
        string path = script.GetString("Path" + sufix);
        
        /* Set entity path */
        pEntity->SetBasepath(mBasepath);

        /* Load entity */
        ret = pEntity->Load(mBasepath + path);
        if (!ret) return false;

        if (!type.compare("Enemy")) {
            mScore += pEntity->GetPoints();
        }
    }

    return true;
}

bool Level::LoadSettings(string filepath)
{
    Script script;
    bool   ret;

    /* Load script */
    ret = script.Load(filepath);
    if (!ret) return false;

    /* Set dimensions */
    mWidth  = script.GetValue<Uint16>("Width");
    mHeight = script.GetValue<Uint16>("Height");

    /* Set forces */
    mFriction = script.GetValue<float>("Friction");
    mGravity  = script.GetValue<float>("Gravity");

    /* Set player position */
    mPlayerX = script.GetValue<float>("PlayerX");
    mPlayerY = script.GetValue<float>("PlayerY");
    
    /* Set entities path */
    mBasepath = script.GetString("Entities.Path");

    return true;
}

bool Level::Load(string filepath)
{
    bool ret;
    
    /* Reset score */
    mScore = 0;
    
    /* Load settings */
    ret = LoadSettings(filepath);
    if (!ret) {
        /* Unload level */
        Unload();

        return false;
    }

    /* Load entities */
    ret = LoadEntities();
    if (!ret) {
        /* Unload level */
        Unload();

        return false;
    }

    /* Set loaded */
    mLoaded = true;

    return true;
}

void Level::Unload(void)
{
    /* Not loaded */
    //if (!Loaded)
        //return;

    /* Destroy entities */
    while (!mEntityList.empty()) {
        Entity *pEntity;

        /* Pop entity */
        pEntity = mEntityList.back();
        mEntityList.pop_back();

        /* Free memory */
        delete pEntity;
    }

    /* Unset loaded */
    mLoaded   = false;
    mFinished = false;
    mScore    = 0;
}

void Level::Update(void)
{
    vector<Entity *>::iterator it;

    Uint32 nextPosition[2];
    Uint16 parent = 0;
    Uint16 value  = 0;
    Rect   rect;
    
    Uint16 motion = DEFAULT;

    /* Check entities */
    foreach (mEntityList, it) {
        Entity *pEntity = *it;
        
        /* Entity dead and inactive */
        if ((pEntity->GetState() & ENTITY_DEAD) && (pEntity->GetId() == 0)) {
            continue;
        }
        
        /* Entity dead and active */
        if ((pEntity->GetState() & ENTITY_DEAD) && (pEntity->GetId() != 0)) {
            parent = pEntity->GetId();
            pEntity->SetId(0);
            
            rect = pEntity->GetRect();
            Uint16 number = 2 * pEntity->GetEntityNumber();
            
            nextPosition[0] = number;
            nextPosition[1] = number + 1;
            
            /* Set motion */
            motion = pEntity->GetMotion();
            motion += ENTITY_INACTIVE;
            
            value = 2;
            
            continue;
        }

        /* Born child */
        if (value != 0 && parent == pEntity->GetId()) {
            for (int i = value; i >= 0; i--) {
                Uint16 entityNumber = pEntity->GetEntityNumber();
                
                /* Check if is child */
                if (nextPosition[i] == entityNumber) {
                    pEntity->SetState(ENTITY_IDLE);
                    
                    float x = rect.GetX();
                    float y = rect.GetY();
                    
                    /* Right child */
                    if (entityNumber % 2 == 0) {
                        x += rect.GetWidth() * 0.5;
                    }
                        
                    y += rect.GetHeight() * 0.75;
                    
                    /* Configure entity */
                    pEntity->SetStart(x, y);
                    pEntity->ResetRect();
                    pEntity->SetMotion(motion);
                    
                    value--;
                }
            }
        }
        
        if (pEntity->GetState() & ENTITY_INVISIBLE) {
            continue;
        }
        
        /* Update entity */
        pEntity->Update();
    }
}

bool Level::Draw(bool idle)
{
    vector<Entity *>::iterator it;

    /* No entities */
    if (mEntityList.empty()) return false;

    /* Level entities */
    foreach (mEntityList, it) {
        Entity *pEntity = *it;
        
        if (pEntity->GetState() & ENTITY_INVISIBLE) {
            continue;
        }

        /* Draw entity */
        pEntity->Draw(idle);
    }

    return true;
}

float Level::GetFriction(void)
{
    return mFriction;
}

float Level::GetGravity(void)
{
    return mGravity;
}

Uint16 Level::GetWidth(void)
{
    return mWidth;
}

Uint16 Level::GetHeight(void)
{
    return mHeight;
}

float Level::GetPlayerX(void)
{
    return mPlayerX;
}

float Level::GetPlayerY(void)
{
    return mPlayerY;
}

bool Level::GetFinished(Uint32 value)
{
    /* Level finished */
    if (mScore <= value) {
        mFinished = true;
    } else {
        mFinished = false;
    }
        
    return mFinished;
}
