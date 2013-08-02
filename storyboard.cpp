#include <boost/format.hpp>

#include "level.h"
#include "player.h"
#include "script.h"
#include "storyboard.h"
#include "utils.h"

using namespace boost;


/* Storyboard handler */
CStory *pStory = NULL;


CStory::CStory(void)
{
	/* Initialize variables */
	Current = 0;
	Levels  = 0;
	Score   = 0;
}

CStory::~CStory (void)
{
    while (!List.empty())
		List.pop_back();
}

void CStory::SetActive(void)
{
	/* Set active */
	pStory = this;
}

bool CStory::Load(string path)
{
	CScript Script;

	bool ret;

	/* Load script */
	ret = Script.Load(path + "Storyboard\\storyboard.ini");
	if (!ret)
		return false;
		
	/* Number of lives */
	Lives = Script.GetValue<Uint32>("Lives");

	/* Number of levels */
	Levels = Script.GetValue<Uint32>("Levels");
	
	/* Number of points per life */
	Points = Script.GetValue<Uint32>("Points");

	/* Get levels */
	for (Uint32 i = 0; i < Levels; i++) {
		string name, prefix;

		/* Generate prefix */
		prefix = str(format("Level[%d].") % i);
		
		/* Generate name */
		name = path + "Storyboard\\";

		/* Get level name */
		name += Script.GetString(prefix + "Path");

		/* Push level */
		List.push_back(name);
	}
	
	/* Load HighScore */
	LoadHighScore(path);

	return true;
}

bool CStory::LoadHighScore(string path)
{
    string   line;
    ifstream File;
    
	/* Generate name */
    path += "Storyboard\\highscore.txt";
    const char *name = path.c_str();
    
	/* Open file */
    File.open(name);
    if (!File.is_open())
        return false;
    
    getline(File, line);
    RemoveSpaces(line);
    
    Score = lexical_cast<Uint32>(line);
    
    return true;
}

bool CStory::SaveHighScore(string path)
{
    ofstream File;
    
	/* New HighScore */
    if (this->Score > pPlayer->GetScore())
        return false;
    
    this->Score = pPlayer->GetScore();
    
	/* Generate name */
    path += "Storyboard\\highscore.txt";
    const char *name = path.c_str();
    
	/* Open file */
    File.open(name, ios::out | ios::trunc);
    if (!File.is_open() )
        return false;
        
    File << this->Score;
    
    return true;
}

bool CStory::PrevLevel(void)
{
	/* No previous */
	if (Current == 0)
		return false;
	
	/* Set previous */
	Current--;

	return true;
}

bool CStory::NextLevel(void)
{
	/* No next */
	if (Current >= Levels - 1)
		return false;

	/* Set next */
	Current++;

	return true;
}

bool CStory::End(void)
{
	/* Check if ended */
	return (Current >= Levels);
}

bool CStory::LoadLevel(void)
{
	/* No level/player */
	if (!pLevel || !pPlayer)
		return false;

	/* Wrong level */
	if (Current < 0 || Current >= Levels)
		return false;

	/* Get level */
	string name = List[Current];

	/* Load level */
	bool ret = pLevel->Load(name);
	if (!ret)
		return false;

	/* Set player position */
	pPlayer->SetStart(pLevel->GetPlayerX(),	pLevel->GetPlayerY());

	/* Reset player */
	pPlayer->ResetRect();

	return true;
}

void CStory::UnloadLevel(void)
{
	/* Unload level */
	if (pLevel)
		pLevel->Unload();
}

void CStory::SetLevels(Uint32 value)
{
    Levels = value;
}

void CStory::SetCurrent(Uint32 value)
{
    Current = value;
}

void CStory::SetLives (Uint16 credits)
{
	/* Initial lives */
	Uint32 lives = pPlayer->GetLives() + Lives * credits;

	/* Set initial player lives */
    pPlayer->SetLives(lives);
}

Uint32 CStory::GetLevels(void)
{
    return Levels;
}

Uint32 CStory::GetCurrent(void)
{
    return Current;
}

Uint32 CStory::GetPoints(void)
{
    return Points;
}

Uint32 CStory::GetHighscore(void)
{
    return Score;
}