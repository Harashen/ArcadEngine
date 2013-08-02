#include <boost/format.hpp>

#include "hud.h"
#include "player.h"
#include "rect.h"
#include "script.h"
#include "storyboard.h"
#include "utils.h"
#include "video.h"

using namespace boost;


/* HUD handler */
CHud *pHud = NULL;


bool CHud::LoadSettings(string filepath)
{
	CScript Script;

	Uint16 size;
	string fontPath;
	bool   ret;

	/* Load script */
	ret = Script.Load(filepath + "hud.ini");
	if (!ret)
		return false;

	/* Get font values */
	fontPath = Script.GetString("Font");
	size     = Script.GetValue<Uint16>("Size");
	
	/* Load HUD font */
	ret = Font.Load(filepath + fontPath, size);
	if (!ret)
		return false;
		
	/* Set score attributes */
    ScoreRect.SetX( Script.GetValue<float>("Score.PosX") );
    ScoreRect.SetY( Script.GetValue<float>("Score.PosY") );
    ScoreColor = Script.GetColor("Score.Color");
    
    /* Set highscore attributes */
    HighScoreRect.SetX( Script.GetValue<float>("HighScore.PosX") );
    HighScoreRect.SetY( Script.GetValue<float>("HighScore.PosY") );
    HighScoreColor = Script.GetColor("HighScore.Color");
    
    /* Set FPS attributes */
    FpsRect.SetX( Script.GetValue<float>("Fps.PosX") );
    FpsRect.SetY( Script.GetValue<float>("Fps.PosY") );
    FpsColor = Script.GetColor("Fps.Color");
    
    /* Set lives attributes */
    LivesRect.SetX( Script.GetValue<float>("Lives.PosX") );
    LivesRect.SetY( Script.GetValue<float>("Lives.PosY") );
    LivesColor = Script.GetColor("Lives.Color");
    
    /* Set text attributes */
    TextRect.SetX( Script.GetValue<float>("Text.PosX") );
    TextRect.SetY( Script.GetValue<float>("Text.PosY") );
    TextColor = Script.GetColor("Text.Color");
    
    /* Set press attributes */
    PressRect.SetX( Script.GetValue<float>("Press.PosX") );
    PressRect.SetY( Script.GetValue<float>("Press.PosY") );

	return true;
}

bool CHud::Init(string filepath)
{
	ScreenColor.r = 0;
    ScreenColor.g = 0;
    ScreenColor.b = 0;
	
	/* Load settings */
	return LoadSettings(filepath);
}

void CHud::SetActive(void)
{
	/* Set active HUD */
	pHud = this;
}

void CHud::SetScreen(Sint32 width, Sint32 height)
{
	/* Set screen rect */
	ScreenRect.Set(0, 0, width, height);
	
	Font.SetScreen(width, height);
}

bool CHud::DrawString(string text, SDL_Color Color, CRect Rect, bool center)
{
	/* Render string */
	return Font.Render(Color, text, Rect.GetX(), Rect.GetY(), center);
}

bool CHud::DrawScore(void)
{
	string text;

	/* Generate string */
	text = str(format("Score %07d") % pPlayer->GetScore());

	/* Draw Score */
	return DrawString(text, ScoreColor, ScoreRect);
}

bool CHud::DrawHighScore(void)
{
	string text;

	/* Generate string */
	text = str(format("Highscore %07d") % pStory->GetPoints());

	/* Draw Highscore */
	return DrawString(text, HighScoreColor, HighScoreRect);
}

bool CHud::DrawFps(void)
{
	string text;

	/* Generate string */
	text = str(format("%.2f FPS") % pVideo->GetFps());

	/* Draw FPS */
	return DrawString(text, FpsColor, FpsRect);
}

bool CHud::DrawLives(void)
{
	string text;

	/* Generate string */
	text = str(format("Lives %s") % pPlayer->GetLives());

	/* Draw player lives */
	return DrawString(text, LivesColor, LivesRect);
}

bool CHud::DrawPoints(float alpha)
{
	CRect  Rect = TextRect;
	Uint32 temp = pStory->GetPoints() * pPlayer->GetLives();
    Uint16 size = Font.Size;
    string text;
	bool   ret;
    
	/* Draw background */
    ScreenRect.Draw(ScreenColor, alpha);
    
	/* Generate string */
    text = str(format("Points X Life")  );
    Rect.SetY(Rect.GetY() + size);
	
	/* Draw string */
    ret = DrawString(text, TextColor, Rect, true);
    if (!ret)
        return false;
    
	/* Generate string */
    text = str(format("%d X %d = %d") % pStory->GetPoints() % pPlayer->GetLives() % temp );
    Rect.SetY(Rect.GetY() - size);
	
	/* Draw string */
    ret = DrawString(text, TextColor, Rect, true);
    if (!ret)
        return false;
    
	/* Generate string */
    text = str(format("Total score: %03d") % pPlayer->GetScore() );
    Rect.SetY(Rect.GetY() - size);
	
	/* Draw string */
    ret = DrawString(text, TextColor, Rect, true);
    if (!ret)
        return false;
        
    return true;
}

bool CHud::DrawText(string text, float alpha)
{
	/* Draw background */
	ScreenRect.Draw(ScreenColor, alpha);

	/* Draw text */
	return DrawString(text, TextColor, TextRect, true);
}

bool CHud::DrawPress(void)
{
	string text;

	/* Generate string */
	text = str(format("Press any key to continue"));

	/* Draw press text */
	return DrawString(text, TextColor, PressRect, true);
}

bool CHud::DrawCredits(Uint16 credits)
{
	string text;

	/* Generate string */
	text = str(format("Credits %s") % credits);

	/* Draw actual credits */
	return DrawString(text, TextColor, PressRect, true);
}
