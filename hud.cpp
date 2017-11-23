#include <boost/format.hpp>

#include "hud.hpp"
#include "player.hpp"
#include "rect.hpp"
#include "script.hpp"
#include "storyboard.hpp"
#include "utils.hpp"
#include "video.hpp"

using namespace boost;


/* HUD handler */
Hud *gpHud = NULL;


bool Hud::LoadSettings(string filepath)
{
    Script script;

    Uint16 size;
    string fontPath;
    bool   ret;

    /* Load script */
    ret = script.Load(filepath + "hud.ini");
    if (!ret) return false;

    /* Get font values */
    fontPath = script.GetString("Font");
    size     = script.GetValue<Uint16>("Size");
    
    /* Load HUD font */
    ret = mFont.Load(filepath + fontPath, size);
    if (!ret) return false;
        
    /* Set score attributes */
    mScoreRect.SetX(script.GetValue<float>("Score.PosX"));
    mScoreRect.SetY(script.GetValue<float>("Score.PosY"));
    mScoreColor = script.GetColor("Score.Color");
    
    /* Set highscore attributes */
    mHighScoreRect.SetX(script.GetValue<float>("HighScore.PosX"));
    mHighScoreRect.SetY(script.GetValue<float>("HighScore.PosY"));
    mHighScoreColor = script.GetColor("HighScore.Color");
    
    /* Set FPS attributes */
    mFpsRect.SetX(script.GetValue<float>("Fps.PosX"));
    mFpsRect.SetY(script.GetValue<float>("Fps.PosY"));
    mFpsColor = script.GetColor("Fps.Color");
    
    /* Set lives attributes */
    mLivesRect.SetX(script.GetValue<float>("Lives.PosX"));
    mLivesRect.SetY(script.GetValue<float>("Lives.PosY"));
    mLivesColor = script.GetColor("Lives.Color");
    
    /* Set text attributes */
    mTextRect.SetX(script.GetValue<float>("Text.PosX"));
    mTextRect.SetY(script.GetValue<float>("Text.PosY"));
    mTextColor = script.GetColor("Text.Color");
    
    /* Set press attributes */
    mPressRect.SetX(script.GetValue<float>("Press.PosX"));
    mPressRect.SetY(script.GetValue<float>("Press.PosY"));
    
    return true;
}

bool Hud::Init(string filepath)
{
    mScreenColor.r = 0;
    mScreenColor.g = 0;
    mScreenColor.b = 0;
    
    /* Load settings */
    return LoadSettings(filepath);
}

void Hud::SetActive(void)
{
    /* Set active HUD */
    gpHud = this;
}

void Hud::SetScreen(Sint32 width, Sint32 height)
{
    /* Set screen rect */
    mScreenRect.Set(0, 0, width, height);
    
    mFont.SetScreen(width, height);
}

bool Hud::DrawString(string text, SDL_Color color, Rect rect, bool center)
{
    /* Render string */
    return mFont.Render(color, text, rect.GetX(), rect.GetY(), center);
}

bool Hud::DrawScore(void)
{
    string text;

    /* Generate string */
    text = str(format("Score %07d") % gpPlayer->GetScore());

    /* Draw Score */
    return DrawString(text, mScoreColor, mScoreRect);
}

bool Hud::DrawHighScore(void)
{
    string text;

    /* Generate string */
    text = str(format("Highscore %07d") % gpStory->GetPoints());

    /* Draw Highscore */
    return DrawString(text, mHighScoreColor, mHighScoreRect);
}

bool Hud::DrawFps(void)
{
    string text;

    /* Generate string */
    text = str(format("%.2f FPS") % gpVideo->GetFps());

    /* Draw FPS */
    return DrawString(text, mFpsColor, mFpsRect);
}

bool Hud::DrawLives(void)
{
    string text;

    /* Generate string */
    text = str(format("Lives %s") % gpPlayer->GetLives());

    /* Draw player lives */
    return DrawString(text, mLivesColor, mLivesRect);
}

bool Hud::DrawPoints(float alpha)
{
    Rect   rect = mTextRect;
    Uint32 temp = gpStory->GetPoints() * gpPlayer->GetLives();
    Uint16 size = mFont.mSize;
    string text;
    bool   ret;
    
    /* Draw background */
    mScreenRect.Draw(mScreenColor, alpha);
    
    /* Generate string */
    text = str(format("Points X Life")  );
    rect.SetY(rect.GetY() + size);
    
    /* Draw string */
    ret = DrawString(text, mTextColor, rect, true);
    if (!ret) return false;
    
    /* Generate string */
    text = str(format("%d X %d = %d") % gpStory->GetPoints() % gpPlayer->GetLives() % temp );
    rect.SetY(rect.GetY() - size);
    
    /* Draw string */
    ret = DrawString(text, mTextColor, rect, true);
    if (!ret) return false;
    
    /* Generate string */
    text = str(format("Total score: %03d") % gpPlayer->GetScore());
    rect.SetY(rect.GetY() - size);
    
    /* Draw string */
    ret = DrawString(text, mTextColor, rect, true);
    if (!ret) return false;
        
    return true;
}

bool Hud::DrawText(string text, float alpha)
{
    /* Draw background */
    mScreenRect.Draw(mScreenColor, alpha);

    /* Draw text */
    return DrawString(text, mTextColor, mTextRect, true);
}

bool Hud::DrawPress(void)
{
    string text;

    /* Generate string */
    text = str(format("Press any key to continue"));

    /* Draw press text */
    return DrawString(text, mTextColor, mPressRect, true);
}

bool Hud::DrawCredits(Uint16 credits)
{
    string text;

    /* Generate string */
    text = str(format("Credits %s") % credits);

    /* Draw actual credits */
    return DrawString(text, mTextColor, mPressRect, true);
}
