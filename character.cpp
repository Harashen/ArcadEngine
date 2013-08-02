#include <boost/format.hpp>
#include "character.h"
#include "level.h"

using namespace boost;


CCharacter::CCharacter(void)
{
	/* Set type */
	Type = "Character";
}

void CCharacter::Reset(void)
{
	/* Reset speed */
	SpeedX = 0.0;
	SpeedY = 0.0;

	/* Reset position */
	Rect.SetX(StartX);
	Rect.SetY(StartY);
}

bool CCharacter::LoadSettings(const char *path)
{
	CScript Script;
	bool    ret;

	/* Load entity settings */
	ret = CEntity::LoadSettings(path);
	if (!ret)
		return false;

	/* Load script */
	ret = Script.Load(path);
	if (!ret)
		return false;

	/* Set animations */
	EntityNumber = Script.GetValue<Uint16>("Entity_Number");
	Id           = Script.GetValue<Uint16>("Id");
	
	if (!State)
		State = Script.GetValue<Uint16>("State");

	return true;
}

bool CCharacter::Load(const char *path)
{
	bool ret;

	/* Load entity settings */
	ret = LoadSettings(path);
	if (!ret)
		return false;
	
	/* Load animation */
	Animation.Load(Basepath + Sprite, Width, Height);
	
	/* Set position */
	Rect.SetX(StartX);
	Rect.SetY(StartY);
	
	Rect.SetWidth(Width);
	Rect.SetHeight(Height);
	
	/* Reset */
	Reset();
	
	Loaded = true;

	return true;
}

bool CCharacter::Load(string path)
{
	const char *c = path.c_str();
	
	/* Load character */
	return Load(c);
}

bool CCharacter::Update(void)
{
	Sint16 w = pLevel->GetWidth();
	Sint16 h = pLevel->GetHeight();
	
	/* Move character */
	bool ret = CEntity::Update();
	if (!ret)
		return false;

	/* Check limits */
	if (Rect.GetX() >= w - Width)
        Rect.SetX(w - Width);
        
    if (Rect.GetY() >= h - Height)
        Rect.SetY(h - Height);
    
    if (Rect.GetX() < 0)
        Rect.SetX(0);
        
    if (Rect.GetY() < 0)
        Rect.SetY(0);
		
	return true;
}

bool CCharacter::Draw(bool idle)
{
	/* Character dead */
	if (State & ENTITY_DEAD)
		return false;
	
	/* Character idle */
	if (!State) {
		idle = true;
		
		/* Set stop animation */
		if (AniMap[ENTITY_STOP])
			Animation.SetAnimation(AniMap[ENTITY_STOP] - 1);
	}
	
	Animation.Update(idle);
	
	bool ret = Animation.Draw(NULL, &Rect);
	if (!ret)
		return false;
	
	return true;
}
