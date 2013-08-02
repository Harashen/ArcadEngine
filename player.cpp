#include "collision.h"
#include "player.h"

/* Player handler */
CPlayer *pPlayer = NULL;


CPlayer::CPlayer(void)
{
	/* Set type */
	Type = "Player";

	/* Initialize variables */
	Collidable = true;
	Score      = 0;
	LevelScore = 0;
	Lives	   = 0;
	Bullets    = 3;
	
	Weapon = new CWeapon[Bullets];
}

CPlayer::~CPlayer(void)
{
	if (Weapon)
		delete Weapon;
}

void CPlayer::SetActive(void)
{
	/* Active player */
	pPlayer = this;
}

void CPlayer::Collision(CEntity *Entity, Uint32 &col)
{
	string type = Entity->GetType();

	/* Default handler */
	CEntity::Collision(Entity, col);

	/* Collision with enemy */
	if (!type.compare("Enemy")) {
		if (col) {
			/* Kill player */
			Kill();
			Sound.Play(0);
			
			/* Lose life */
			LoseLife();
		}
	}

	/* Collision with item */
	if (!type.compare("Item"))
		Score += Entity->GetPoints();
}

bool CPlayer::Load(string filepath)
{
    
    SetBasepath(filepath);
	
	/* Load player configuration */
    bool ret = CCharacter::Load(filepath + "player.ini");
    if (!ret)
        return false;
    
    for (int i = 0; i < Bullets; i++)
        Weapon[i].SetBasepath(filepath);
		
    LoadWeapon(filepath + "weapon.ini");
    
    return true;
}
	
void CPlayer::LoadWeapon(string filepath)
{
	Weapon[0].Load(filepath);
	
	for(int i = 1; i < Bullets; i++)
		Weapon[i] = Weapon[0];
}

void CPlayer::Shot(void)
{
	/* Already dead */
	if (State == ENTITY_DEAD)
		return;
		
	for (int i = 0; i < Bullets; i++) {
		bool  sound = false;
        CRect RectW = Rect;
        
        if (i == 0) {
            sound = true;
            RectW.SetY(RectW.GetY() + 10);
        }
        else {
            float x = RectW.GetX();
			
			/* Right/Left bullets */
            if (i % 2 == 0)
                RectW.SetX(x + 10);
            else
                RectW.SetX(x - 10);
        }
		
		/* Shot bullet */
        if (Weapon[i].Shot(RectW, sound) == false)
            break;
	}
}

void CPlayer::Reset(void)
{
	/* Reset weapon */
	for (int i = 0; i < Bullets; i++)
		Weapon[i].SetState(ENTITY_DEAD);
	
	/* Reset character */
	CCharacter::Reset();
	
	LevelScore = 0;
	State      = ENTITY_IDLE;
	
	/* Stop sound */
	Sound.Stop();
}

bool CPlayer::Update(void)
{	
	/* Update character */
	CCharacter::Update();
	
	/* Already dead */
	if (State & ENTITY_DEAD)
		return false;
	
	/* Update bullets */
	for (int i = 0; i < Bullets; i++) {
		bool ret = Weapon[i].Update();
		
		/* Bullets dead */
		if (!ret) {
			/* Reset weapon */
			for (int j = 0; j < Bullets; j++)
				Weapon[j].SetState(ENTITY_DEAD);
			
			return true;
		}
	}
	
	return true;
}

bool CPlayer::Draw(bool idle)
{	
	/* Draw bullets */
	for (int i = 0; i < Bullets; i++)
		Weapon[i].Draw();
		
	/* Draw character */
	return CCharacter::Draw(idle);
}

void CPlayer::LoseLife(void)
{
	if (Lives > 0)
		Lives--;
}

Uint32 CPlayer::GetLevelScore(void)
{
	return LevelScore;
}

Uint32 CPlayer::GetLives(void)
{
	return Lives;
}

Uint32 CPlayer::GetScore(void)
{
	return Score;
}

void CPlayer::SetLives(Uint32 value)
{
	Lives = value;
}

void CPlayer::SetScore(Uint32 value)
{
	Score 	   += value;
	LevelScore += value;
}

void CPlayer::ResetScore(void)
{
	Score = 0;
}

