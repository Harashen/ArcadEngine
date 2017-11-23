#include <fstream>

#include "credits.hpp"
#include "script.hpp"
#include "utils.hpp"


Credits::Credits(void)
{
    /* Initialize variables */
    mState = CREDITS_STOPPED;
}

Credits::~Credits(void)
{
    /* Unload */
    Unload();
}

bool Credits::LoadSettings(string filepath)
{
    Script script;
    bool   ret;
    string fontPath;
    string musicPath;

    /* Load script */
    ret = script.Load(filepath + "credits.ini");
    if (!ret) return false;
        
    /* Get font values */
    fontPath = script.GetString("Font");
    mSize    = script.GetValue<Uint16>("Size");
    mColor   = script.GetColor("Font.Color");

    /* Load credits font */
    ret = mFont.Load(filepath + fontPath, mSize);
    
    if (!ret) {
        /* Unload */
        Unload();

        return false;
    }
    
    /* Get background values */
    mBackground = script.GetColor("Background.Color");
    mAlpha      = script.GetValue<float>("Background.Alpha");

    /* Set attributes */
    mSpeed = script.GetValue<float>("Speed");
    
    /* Load music */
    musicPath = script.GetString("Music");
    
    if (!musicPath.empty()) {
        mMusic.Load(filepath + musicPath);
    }

    return true;
}

bool Credits::LoadText(string filepath)
{
    Point    point;
    ifstream File;
    string   line;	
    bool     ret;

    /* Prepare path */
    filepath += "credits.txt";
    const char *path = filepath.c_str(); 
    
    /* Open file */
    File.open(path);

    /* File not opened */
    ret = File.is_open();
    if (!ret) return false;

    /* Set point */
    point.SetPoint(400, 500);

    /* Read file */
    while (getline(File, line)) {
        Text *text = new Text;
        
        /* Remove not valid lines */
        if (line[0] == '#') {
            continue;
        }
            
        /* Set text position */
        text->mText  = line; 
        text->mPoint = point;		
        
        mTextVect.push_back(text);
        
        /* Update position */
        float y = point.GetY();
        y -= mSize * 2;
        point.SetY(y);
    }

    /* Close file */
    File.close();

    return true;
}

void Credits::Unload(void)
{
    vector<Text *>::iterator it;
    
    /* Clear text vector */
    foreach (mTextVect, it) {
        Text *temp = *it;
        
        delete temp;
    }

    /* Unload font */
    mFont.Unload();

    /* Unload music */
    mMusic.Unload();
}

bool Credits::Load(string filepath)
{
    bool ret;

    /* Load settings */
    ret = LoadSettings(filepath);
    if (!ret) {
        /* Unload */
        Unload();

        return false;
    }

    /* Load text */
    ret = LoadText(filepath);
    if (!ret) {
        /* Unload */
        Unload();

        return false;
    }

    return true;
}

void Credits::Reset(void)
{
    /* Reset coordinates */
    mPosition.SetX(0);
    mPosition.SetY(0);
}

void Credits::SetScreen(Sint32 width, Sint32 height)
{
    /* Set screen rect */
    mScreenRect.Set(0, 0, width, height);
}

bool Credits::Update(void)
{
    const float step = 0.1;
    vector<Text *>::iterator it;
    
    /* Update text */
    foreach (mTextVect, it) {
        Text *temp  = *it;
        Point point = temp->mPoint;
        
        /* FIXME */
        for (float i = 0; i < mSize; i += step) {
            float y = point.GetY();
            y += i;
            
            point.SetY(y);
        }

        (*it)->mPoint = point;
    }

    return true;
}

bool Credits::Draw(void)
{
    vector<Text *>::iterator it;
    
    /* Credits not playing/paused */
    if (mState != CREDITS_PLAYING && mState != CREDITS_PAUSED) {

        return false;
    }
    
    /* Draw background colour */
    mScreenRect.Draw(mBackground, mAlpha);
    
    /* Update text */
    foreach (mTextVect, it) {
        Text  *temp  = *it;
        Point  point = temp->mPoint;
        float  posY  = point.GetY();
        float  posX  = point.GetX();
        string text  = temp->mText;
        
        /* Render text */
        mFont.Render(mColor, text, posX, posY);
    }

    return true;
}

bool Credits::Play(void)
{
    bool ret;

    /* Reset */
    Reset();

    /* Play music */
    ret = mMusic.Play();
    if (!ret) return false;

    /* Set playing state */
    mState = CREDITS_PLAYING;

    return true;
}

void Credits::Stop(void)
{
    /* Credits not playing */
    if (mState != CREDITS_PLAYING) {
        return;
    }

    /* Stop music */
    mMusic.Stop();

    /* Set stopped state */
    mState = CREDITS_STOPPED;
}

void Credits::Pause(void)
{
    /* Credits not playing */
    if (mState != CREDITS_PLAYING) {
        return;
    }

    /* Pause music */
    mMusic.Pause();

    /* Set paused state */
    mState = CREDITS_PAUSED;
}

void Credits::Resume(void)
{
    /* Credits not paused */
    if (mState != CREDITS_PAUSED) {
        return;
    }

    /* Resume music */
    mMusic.Resume();

    /* Set playing state */
    mState = CREDITS_PLAYING;
}
