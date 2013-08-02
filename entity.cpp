#include <cmath>
#include <boost/format.hpp>

#include "collision.h"
#include "entity.h"
#include "level.h"
#include "utils.h"
#include "video.h"

using namespace boost;


CEntity::CEntity(void)
{
	/* Default type */
	Type = "Entity";

	/* Initialize variables */
	CollisionRect = 0;
	StartX        = 0.0;
	StartY        = 0.0;
	Points		  = 0;
	Collidable    = false;
	Loaded        = false;
	noGravity     = false;
	
	EntityNumber  = 0;
	Id	          = 0;
	
	State  = ENTITY_IDLE;
	Motion = ENTITY_INACTIVE;
}

CEntity::~CEntity(void)
{
	/* Unload */
	Unload();
	
	/* Unset loaded */
	Loaded = false;
}

bool CEntity::LoadSettings(const char *filepath)
{
	CScript Script;
	bool    ret;
	string  sound;

	/* Load script */
	ret = Script.Load(filepath);
	if (!ret)
		return false;
		
	/* Set name */
	Name = Script.GetString("Name");

	/* Set dimensions */
	Width  = Script.GetValue<Uint16>("Width");
	Height = Script.GetValue<Uint16>("Height");
	
	/* Initial position */
	StartX = Script.GetValue<float>("StartX");
	StartY = Script.GetValue<float>("StartY");

	/* Set sprite */
	Sprite = Script.GetString("Sprite");
		
	/* Set speed */
	Speed = Script.GetValue<float>("Speed");

	/* Set acceleration */
	AccelX = Script.GetValue<float>("AccelX");
	AccelY = Script.GetValue<float>("AccelY");

	/* Set animation */
	bool oscillate = Script.GetBool("Oscillate");
	Animation.SetOscillate(oscillate);
	
	/* Set if gravity affects entity */
	noGravity = Script.GetBool("Gravity");
	
	/* Motion values */
	AniMap[ENTITY_LEFT]  = Script.GetValue<Uint16>("Left");
    AniMap[ENTITY_RIGHT] = Script.GetValue<Uint16>("Right");
    AniMap[ENTITY_UP]    = Script.GetValue<Uint16>("Up");
    AniMap[ENTITY_DOWN]  = Script.GetValue<Uint16>("Down");
    AniMap[ENTITY_STOP]  = Script.GetValue<Uint16>("Stop");
	
	/* Load sound */
	sound = Script.GetString("Sound");
	if (!sound.empty())
		Sound.Load(Basepath + sound);
	
	/* Set points */
	Points = Script.GetValue<Uint32>("Points");
	
	/* Set attributes */
	Collidable = Script.GetBool("Collidable");
	
	/* Load collisions */
	ret = LoadCollisions(Script);
	if (!ret)
		return false;
	
	return true;
}

bool CEntity::LoadCollisions(CScript Script)
{
    CollisionRect = Script.GetValue<Uint16>("Collisions");
    
    /* No collision rectangles */
    if (!CollisionRect) {
        CRect *Rect = new CRect;
        
        // Si no se ha creado el rectángulo
        if (!Rect)
            return false;
        
        Collisions.push_back(Rect);
        
        Rect->SetX(0);
		Rect->SetY(0);
		Rect->SetInitialX(0);
		Rect->SetInitialY(0);
		Rect->SetWidth(Width);
		Rect->SetHeight(Height);
    }
    else {
        for (Uint16 i = 0; i < CollisionRect; i++) {
            CRect *Rect = NULL;
            string suffix;
            Sint32 w, h;
            float  x, y;
            
            /* Initialize values */
            w = h = 0;
            x = y = 0.0;
            
            suffix = str(format("[%d]") % i);
            
            Rect = new CRect;
            
            if (!Rect)
                continue;
            
            Collisions.push_back(Rect);
            
			/* Get attributes */
            w = Script.GetValue<Sint32>("Width" + suffix);
            h = Script.GetValue<Sint32>("Height" + suffix);
            x = Script.GetValue<Sint32>("X" + suffix);
            y = Script.GetValue<Sint32>("Y" + suffix);
            
			/* Set rectangle */
            Rect->SetX(x);
            Rect->SetY(y);
            Rect->SetInitialX(x);
            Rect->SetInitialY(y);
            Rect->SetWidth(w);
            Rect->SetHeight(h);
        }
    }
    
    return true;
}

void CEntity::Reset(void)
{
	/* Set direction */
	Motion = DEFAULT;

	/* Set force/acceleration */
	ForceX = 0.0;
	ForceY = 0.0;
	AccelX = 0.0;
	AccelY = 0.0;
	
	/* Set speed */
	SpeedX = 0.0;
	SpeedY = 0.0;

	/* Set position */
	Rect.SetX(StartX);
	Rect.SetY(StartY);
	
	/* Set state */
	State = ENTITY_IDLE;

	/* Reset animation */
	Animation.Reset();
}

bool CEntity::Load(const char *filepath)
{
	bool ret;

	/* Load settings */
	ret = LoadSettings(filepath);
	if (!ret)
		return false;

	/* Load animation */
	ret = Animation.Load(Basepath + Sprite, Width, Height);
	if (!ret)
		return false;

	/* Set rectangle dimension */
	Rect.SetWidth(Width);
	Rect.SetHeight(Height);

	/* Set loaded */
	Loaded = true;

	/* Reset entity */
	Reset();

	return true;
}

bool CEntity::Load(string filepath)
{
	const char *c = filepath.c_str();

	/* Load entity */
	return Load(c);
}

void CEntity::Unload(void)
{
	/* Not loaded */
	if (!Loaded)
		return;

	/* Unload animation */
	Animation.Unload();
	
	/* Clear collision vector */
	Collisions.clear();
}

void CEntity::Collision(CEntity *Entity, Uint32 &col)
{
	/* Entity non-collidable */
	if (!Entity->Collidable)
		col = 0;
}

void CEntity::Physics(void)
{
	/* Walk/Run force */
	if (State & ENTITY_WALK) {
		/* Apply force */
		if (ForceX < 0 && SpeedX > -Speed)
			SpeedX += ForceX;
		if (ForceX > 0 && SpeedX <  Speed)
			SpeedX += ForceX;
			
		/* No affects gravity */
		if (noGravity) {
			/* Apply force */
			if (ForceY < 0 && SpeedY > -Speed)
				SpeedY += ForceY;
			if (ForceY > 0 && SpeedY <  Speed)
				SpeedY += ForceY;
		}
	}

	/* Level forces */
	if (pLevel) {
		float gravity  = pLevel->GetGravity();
		float friction = pLevel->GetFriction();
		
		/* Friction force */
		if (!(State & ENTITY_WALK)) {
			if (SpeedX < 0)
				SpeedX += friction;
			if (SpeedX > 0)
				SpeedX -= friction;
		}
		
		/* Gravity/Friction force */
		if (!noGravity) {
			if (SpeedY > -AccelY)
				SpeedY -= gravity;
		}
		else {
			if (!(State & ENTITY_WALK)) {
				if (SpeedY < 0)
					SpeedY += friction;
				if (SpeedY > 0)
					SpeedY -= friction;
			}
		}
	}
}

bool CEntity::Update(void)
{
	const float step = 0.1;

	/* Entity is dead */
	if (State & ENTITY_DEAD)
		return false;

	/* Apply physics */
	Physics();

	/* Round speed */
	if (abs(SpeedX) < step)
		SpeedX = 0;
	if (abs(SpeedY) < step)
		SpeedY = 0;

	/* Iterations */
	float loops = MaxAbs<float>(SpeedX, SpeedY);

	/* Entity movement */
	for (float i = 0; i < loops; i += step) {
		Uint32 col;

		/* Detect collision */
		col = CCollision::Detect(this);

		/* X coordinate */
		if (i < abs(SpeedX)) {
			/* Collision */
			if ( (col & COLLISION_LEFT  && SpeedX < 0) ||
			     (col & COLLISION_RIGHT && SpeedX > 0) )
			     	SpeedX = 0;
					
			float value = Rect.GetX();

			/* Move entity */
			if (SpeedX > 0) {
				value += step;
				Rect.SetX(value);
			}
			
			if (SpeedX < 0) {
				value -= step;
				Rect.SetX(value);
			}
		}

		/* Y coordinate */
		if (i < abs(SpeedY)) {
			/* Collision */
			if ( (col & COLLISION_BOTTOM && SpeedY < 0) ||
			     (col & COLLISION_TOP    && SpeedY > 0) )
			     	SpeedY = 0;

			float value = Rect.GetY();

			/* Move entity */
			if (SpeedY > 0) {
				value += step;
				Rect.SetY(value);
			}
			
			if (SpeedY < 0) {
				value -= step;
				Rect.SetY(value);
			}
		}
	}
	
	return true;
}

bool CEntity::Draw(bool idle)
{
	if (State & ENTITY_DEAD)
		return false;

	CRect srcRect;
	
	/* Set rect */
	srcRect.SetX(0.0);
	srcRect.SetY(0.0);
	srcRect.SetWidth(Rect.GetWidth());
	srcRect.SetHeight(Rect.GetHeight());
	
	if (State & ENTITY_IDLE)
		idle = true;

	/* Animate entity */
	Animation.Update(idle);

	/* Draw entity */
	return Animation.Draw(&srcRect, &Rect);
}

void CEntity::Move(Uint16 dir)
{	
	if (!noGravity){
		if (dir & ENTITY_UP) {
			Uint32 col;

			/* Detect collision */
			col = CCollision::Detect(this);
			
			if (col & COLLISION_BOTTOM)
				SpeedY = AccelY;
		}
	}
	else {
		if (dir & ENTITY_DOWN)
			ForceY = -AccelY;
		
		if (dir & ENTITY_UP)
			ForceY = AccelY;
	}
	
	/* Set X acceleration */
	if (dir & ENTITY_LEFT)
		ForceX = -AccelX;
	
	if (dir & ENTITY_RIGHT)
		ForceX = AccelX;

    /* Set walk state */
	State |= ENTITY_WALK;
}

void CEntity::Run()
{
	Animation.SetFlipX(false);
	Animation.SetFlipY(false);
	
	switch (Motion) {
	case ENTITY_LEFT:
		if (AniMap[ENTITY_LEFT])
			Animation.SetAnimation(AniMap[ENTITY_LEFT] - 1);
		else {
			Animation.SetAnimation(AniMap[ENTITY_RIGHT] - 1);
			Animation.SetFlipX(true);
		}
		break;
		
	case ENTITY_RIGHT:
		if (AniMap[ENTITY_RIGHT])
			Animation.SetAnimation(AniMap[ENTITY_RIGHT] - 1);
		else {
			Animation.SetAnimation(AniMap[ENTITY_LEFT] - 1);
			Animation.SetFlipX(true);
		}
		break;
		
	case ENTITY_UP:
		if (AniMap[ENTITY_UP])
			Animation.SetAnimation(AniMap[ENTITY_UP] - 1);
		else {
			Animation.SetAnimation(AniMap[ENTITY_DOWN] - 1);
			Animation.SetFlipY(true);
		}
		break;
		
	case ENTITY_DOWN:
		if (AniMap[ENTITY_DOWN])
			Animation.SetAnimation(AniMap[ENTITY_DOWN] - 1);   
		else {
			Animation.SetAnimation(AniMap[ENTITY_UP] - 1);
			Animation.SetFlipY(true);
		}
		break;
	/*case ENT_STOP:
		Animation.SetAnimation(AniMap[ENT_STOP] - 1);
		break;*/
    }
	
	
	/* Entity move */	Move(Motion);
}

void CEntity::Stop(void)
{
	SpeedX *= 0.5;
	SpeedY *= 0.5;
	
	/* Unset walk state */
	State &= ~ENTITY_WALK;
	State |=  ENTITY_IDLE;
}

void CEntity::Kill(void)
{
	/* Already dead */
	if (State & ENTITY_DEAD)
		return;

	/* Set dead state */
	State |= ENTITY_DEAD;
}

string CEntity::GetType(void)
{
	return Type;
}
    
string CEntity::GetName(void)
{
	return Name;
}

Uint32 CEntity::GetPoints(void)
{
	return Points;
}

CRect CEntity::GetRect(void)
{
	return Rect;
}

bool CEntity::GetCollidable(void)
{
	return Collidable;
}

Uint16 CEntity::GetState(void)
{
	return State;
}

Uint16 CEntity::GetMotion(void)
{
	return Motion;
}

Uint16 CEntity::GetId(void)
{
	return Id;
}

Uint16 CEntity::GetEntityNumber(void)
{
	return EntityNumber;
}

void CEntity::SetMotion(Uint16 value)
{
	Motion = value;
}

void CEntity::SetStart(float x, float y)
{
	StartX = x;
	StartY = y;
}

void CEntity::SetState(Uint16 value)
{
	State = value;
}

void CEntity::SetId(Uint16 value)
{
	Id = value;
}

void CEntity::SetBasepath(string value)
{
	Basepath = value;
}

void CEntity::ResetRect(void)
{
	Rect.SetX(StartX);
	Rect.SetY(StartY);
}