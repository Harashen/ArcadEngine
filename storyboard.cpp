#include <boost/format.hpp>

#include "storyboard.hpp"
#include "log.hpp"
#include "level.hpp"
#include "player.hpp"
#include "script.hpp"

using namespace boost;


/* Storyboard handler */
Story *gpStory = NULL;


Story::Story(void)
{
    /* Initialize variables */
    mCurrent = 0;
    mLevels  = 0;
    mScore   = 0;
}

Story::~Story (void)
{
    while (!mList.empty()) {
        mList.pop_back();
    }
}

void Story::SetActive(void)
{
    /* Set active */
    gpStory = this;
}

bool Story::Load(string path)
{
    Script script;

    bool ret;

    /* Load script */
    ret = script.Load(path + "Storyboard\\storyboard.ini");
    if (!ret) return false;
        
    /* Number of lives */
    mLives = script.GetValue<Uint32>("Lives");

    /* Number of levels */
    mLevels = script.GetValue<Uint32>("Levels");
    
    /* Number of points per life */
    mPoints = script.GetValue<Uint32>("Points");

    /* Get levels */
    for (Uint32 i = 0; i < mLevels; i++) {
        string name, prefix;

        /* Generate prefix */
        prefix = str(format("Level[%d].") % i);
        
        /* Generate name */
        name = path + "Storyboard\\";

        /* Get level name */
        name += script.GetString(prefix + "Path");

        /* Push level */
        mList.push_back(name);
    }
    
    /* Load HighScore */
    LoadHighScore(path);

    return true;
}

bool Story::LoadHighScore(string path)
{
    string   line;
    ifstream File;
    
    /* Generate name */
    path += "Storyboard\\highscore.txt";
    const char *name = path.c_str();
    
    /* Open file */
    File.open(name);
    if (!File.is_open()) return false;
    
    getline(File, line);
    RemoveSpaces(line);
    
    mScore = lexical_cast<Uint32>(line);
    
    return true;
}

bool Story::SaveHighScore(string path)
{
    ofstream File;
    
    /* New HighScore */
    if (mScore > gpPlayer->GetScore()) {
        return false;
    }
    
    mScore = gpPlayer->GetScore();
    
    /* Generate name */
    path += "Storyboard\\highscore.txt";
    const char *name = path.c_str();
    
    /* Open file */
    File.open(name, ios::out | ios::trunc);
    if (!File.is_open()) return false;
        
    File << mScore;
    
    return true;
}

bool Story::PrevLevel(void)
{
    /* No previous */
    if (mCurrent == 0) return false;
    
    /* Set previous */
    mCurrent--;

    return true;
}

bool Story::NextLevel(void)
{
    /* No next */
    if (mCurrent >= mLevels - 1) {
        return false;
    }

    /* Set next */
    mCurrent++;

    return true;
}

bool Story::End(void)
{
    /* Check if ended */
    return (mCurrent >= mLevels);
}

bool Story::LoadLevel(void)
{
    /* No level/player */
    if (!gpLevel || !gpPlayer) {
        *gpLog << "[STORYBOARD]  ERROR: Level or Player not loaded!" << endl;
        return false;
    }

    /* Wrong level */
    if (mCurrent < 0 || mCurrent >= mLevels) {
        *gpLog << "[STORYBOARD]  ERROR: Wrong level number (" << mCurrent << ")!" << endl;
        *gpLog << "[STORYBOARD]  ERROR: or Wrong number of levels (" << mLevels << ")!" << endl;

        return false;
    }

    /* Get level */
    string name = mList[mCurrent];

    /* Load level */
    bool ret = gpLevel->Load(name);
    if (!ret) return false;

    /* Set player position */
    gpPlayer->SetStart(gpLevel->GetPlayerX(), gpLevel->GetPlayerY());

    /* Reset player */
    gpPlayer->ResetRect();

    return true;
}

void Story::UnloadLevel(void)
{
    /* Unload level */
    if (gpLevel) gpLevel->Unload();
}

void Story::SetLevels(Uint32 value)
{
    mLevels = value;
}

void Story::SetCurrent(Uint32 value)
{
    mCurrent = value;
}

void Story::SetLives (Uint16 credits)
{
    /* Initial lives */
    Uint32 lives = gpPlayer->GetLives() + mLives * credits;

    /* Set initial player lives */
    gpPlayer->SetLives(lives);
}

Uint32 Story::GetLevels(void)
{
    return mLevels;
}

Uint32 Story::GetCurrent(void)
{
    return mCurrent;
}

Uint32 Story::GetPoints(void)
{
    return mPoints;
}

Uint32 Story::GetHighscore(void)
{
    return mScore;
}