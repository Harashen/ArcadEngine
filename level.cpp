#include <boost/format.hpp>

#include "enemy.h"
#include "item.h"
#include "level.h"
#include "script.h"
#include "utils.h"

using namespace boost;


/* Level handler */
CLevel *pLevel = NULL;


CLevel::CLevel(void)
{
	/* Initialize variables */
	Loaded   = false;
	Finished = false;
	Gravity  = 0.0;
	Score    = 0;

	/* Reset */
	Reset();
}

CLevel::~CLevel(void)
{
	/* Unload level */
	Unload();
}

void CLevel::SetActive(void)
{
	/* Set active level */
	pLevel = this;
}

void CLevel::Reset(void)
{
	vector<CEntity *>::iterator it;

	/* Reset entities */
	foreach (EntityList, it) {
		CEntity *Entity = *it;

		/* Reset entity */
		Entity->Reset();
	}

	/* Reset variables */
	Finished = false;
	Score    = 0;
}

bool CLevel::LoadEntities(void)
{
	CScript Script;
	Uint32  entities;
	
	bool ret;

	/* Load entities script */
	ret = Script.Load(Basepath + "entities.ini");
	if (!ret)
		return false;

	/* Number of entities */
	entities = Script.GetValue<Uint32>("Entities");

	/* Create entities */
	for (Uint32 i = 0; i < entities; i++) {
		CEntity *Entity = NULL;
		string   path, sufix, type;

		/* Generate sufix */
		sufix = str(format("[%d]") % i);

		/* Get entity info */
		type = Script.GetString("Type" + sufix);

		/* Create entity */
		if (!type.compare("Enemy"))
			Entity = new CEnemy;
		if (!type.compare("Entity"))
			Entity = new CEntity;
		if (!type.compare("Item"))
			Entity = new CItem;

		/* Entity not created */
		if (!Entity)
			continue;

		/* Push entity */
		EntityList.push_back(Entity);
		
		/* Get entity path */
		path = Script.GetString("Path" + sufix);
		
		/* Set entity path */
		Entity->SetBasepath(Basepath);

		/* Load entity */
		ret = Entity->Load(Basepath + path);
		if (!ret)
			return false;

		if (!type.compare("Enemy"))
			Score += Entity->GetPoints();
	}

	return true;
}

bool CLevel::LoadSettings(string filepath)
{
	CScript Script;
	bool    ret;

	/* Load script */
	ret = Script.Load(filepath);
	if (!ret)
		return false;

	/* Set dimensions */
	Width    = Script.GetValue<Uint16>("Width");
	Height   = Script.GetValue<Uint16>("Height");

	/* Set forces */
	Friction = Script.GetValue<float>("Friction");
	Gravity  = Script.GetValue<float>("Gravity");

	/* Set player position */
	PlayerX  = Script.GetValue<float>("PlayerX");
	PlayerY  = Script.GetValue<float>("PlayerY");
	
	/* Set entities path */
	Basepath = Script.GetString("Entities.Path");

	return true;
}

bool CLevel::Load(string filepath)
{
	bool ret;
	
	/* Reset score */
	Score = 0;
	
	/* Load settings */
	ret = LoadSettings(filepath);
	if (!ret) {
		/* Unload level */
		Unload();

		return false;
	}

	/* Load entities */
	ret = LoadEntities();
	if (!ret){
		/* Unload level */
		Unload();

		return false;
	}

	/* Set loaded */
	Loaded = true;

	return true;
}

void CLevel::Unload(void)
{
	/* Not loaded */
	//if (!Loaded)
		//return;

	/* Destroy entities */
	while (!EntityList.empty()) {
		CEntity *Entity;

		/* Pop entity */
		Entity = EntityList.back();
		EntityList.pop_back();

		/* Free memory */
		delete Entity;
	}

	/* Unset loaded */
	Loaded   = false;
	Finished = false;
	Score    = 0;
}

void CLevel::Update(void)
{
	vector<CEntity *>::iterator it;

	Uint32  nextPosition[2];
    Uint16  parent = 0;
	Uint16  value  = 0;
    CRect   Rect;
    
    Uint16  motion = DEFAULT;

	/* Check entities */
	foreach (EntityList, it) {
		CEntity *Entity = *it;
		
		/* Entity dead and inactive */
		if ((Entity->GetState() & ENTITY_DEAD) && (Entity->GetId() == 0))
            continue;
        
		/* Entity dead and active */
        if ((Entity->GetState() & ENTITY_DEAD) && (Entity->GetId() != 0)) {
            parent = Entity->GetId();
            Entity->SetId(0);
            
            Rect = Entity->GetRect();
            Uint16 number = 2 * Entity->GetEntityNumber();
			
            nextPosition[0] = number;
            nextPosition[1] = number + 1;
            
			/* Set motion */
            motion = Entity->GetMotion();
            motion += ENTITY_INACTIVE;
			
			value = 2;
            
            continue;
        }

		/* Born child */
		if ((value != 0) && (parent ==  Entity->GetId())) {
            for (int i = value; i >= 0; i--) {
				Uint16 entityNumber = Entity->GetEntityNumber();
				
				/* Check if is child */
                if (nextPosition[i] ==  entityNumber) {
                    Entity->SetState(ENTITY_IDLE);
					
                    float x = Rect.GetX();
                    float y = Rect.GetY();
                    
					/* Right child */
                    if (entityNumber % 2 == 0)
                        x += Rect.GetWidth() * 0.5;
                        
                    y += Rect.GetHeight() * 0.75;
                    
					/* Configure entity */
                    Entity->SetStart(x, y);
                    Entity->ResetRect();
                    Entity->SetMotion(motion);
                    
                    value--;
                }
            }
        }
		
		if (Entity->GetState() & ENTITY_INVISIBLE)
            continue;
        
		/* Update entity */
        Entity->Update();
	}
}

bool CLevel::Draw(bool idle)
{
	vector<CEntity *>::iterator it;

	/* No entities */
	if (EntityList.empty())
		return false;

	/* Level entities */
	foreach (EntityList, it) {
		CEntity *Entity = *it;
		
		if (Entity->GetState() & ENTITY_INVISIBLE)
            continue;

		/* Draw entity */
		Entity->Draw(idle);
	}

	return true;
}

float CLevel::GetFriction(void)
{
	return Friction;
}

float CLevel::GetGravity(void)
{
	return Gravity;
}

Uint16 CLevel::GetWidth(void)
{
	return Width;
}

Uint16 CLevel::GetHeight(void)
{
	return Height;
}

float CLevel::GetPlayerX(void)
{
	return PlayerX;
}

float CLevel::GetPlayerY(void)
{
	return PlayerY;
}

bool CLevel::GetFinished(Uint32 value)
{
	/* Level finished */
	if (Score <= value)
		Finished = true;
	else
		Finished = false;
		
	return Finished;
}
