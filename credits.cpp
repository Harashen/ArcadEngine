#include <fstream>
#include "credits.h"
#include "script.h"
#include "utils.h"


CCredits::CCredits(void)
{
	/* Initialize variables */
	State = CREDITS_STOPPED;
}

CCredits::~CCredits(void)
{
	/* Unload */
	Unload();
}

bool CCredits::LoadSettings(string filepath)
{
	CScript Script;
	bool    ret;
	string  fontPath;
	string  musicPath;

	/* Load script */
	ret = Script.Load(filepath + "credits.ini");
	if (!ret)
		return false;
		
	/* Get font values */
	fontPath = Script.GetString("Font");
	Size     = Script.GetValue<Uint16>("Size");
	Color    = Script.GetColor("Font.Color");
	
	/* Load credits font */
	ret = Font.Load(filepath + fontPath, Size);
	if (!ret) {
		/* Unload */
		Unload();

		return false;
	}
	
	/* Get background values */
	Background = Script.GetColor("Background.Color");
    Alpha      = Script.GetValue<float>("Background.Alpha");

	/* Set attributes */
	Speed = Script.GetValue<float>("Speed");
	
	/* Load music */
	musicPath = Script.GetString("Music");
	if (!musicPath.empty())
		Music.Load(filepath + musicPath);

	return true;
}

bool CCredits::LoadText(string filepath)
{
	CPoint   Point;
	ifstream File;
	string   line;
	
	bool   ret;

	/* Prepare path */
	filepath += "credits.txt";
    const char *path = filepath.c_str(); 
	
	/* Open file */
	File.open(path);

	/* File not opened */
	ret = File.is_open();
	if (!ret)
		return false;

	/* Set point */
	Point.SetPoint(400, 500);

	/* Read file */
	while (getline(File, line)) {
		CText *Txt = new CText;
		
		/* Remove not valid lines */
		if (line[0] == '#')
			continue;
			
		/* Set text position */
		Txt->Text  = line; 
		Txt->Point = Point;
		
		
		Text.push_back(Txt);
		
		/* Update position */
		float y = Point.GetY();
		y -= Size * 2;
		Point.SetY(y);
	}

	/* Close file */
	File.close();

	return true;
}

void CCredits::Unload(void)
{
	vector<CText *>::iterator it;
	
	/* Clear text vector */
	foreach (Text, it) {
		CText *Temp = *it;
		
		delete Temp;
	}

	/* Unload font */
	Font.Unload();

	/* Unload music */
	Music.Unload();
}

bool CCredits::Load(string filepath)
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

void CCredits::Reset(void)
{
	/* Reset coordinates */
	Position.SetX(0);
	Position.SetY(0);
}

void CCredits::SetScreen(Sint32 width, Sint32 height)
{
	/* Set screen rect */
	ScreenRect.Set(0, 0, width, height);
}

bool CCredits::Update(void)
{
	const float step = 0.1;
	vector<CText *>::iterator it;
	
	float i, y;
	
	/* Update text */
	foreach (Text, it) {
		CText *Temp  = *it;
		CPoint Point = Temp->Point;
		
		/* FIXME */
		for (i = 0; i < Size; i += step) {
			y  = Point.GetY();
			y += i;
			
			Point.SetY(y);
		}
		(*it)->Point = Point;
	}

	return true;
}

bool CCredits::Draw(void)
{
	vector<CText *>::iterator it;
	
	/* Credits not playing/paused */
	if (State != CREDITS_PLAYING &&
	    State != CREDITS_PAUSED)
		return false;
	
	/* Draw background colour */
	ScreenRect.Draw(Background, Alpha);
	
	/* Update text */
	foreach (Text, it) {
		CText *Temp  = *it;
		CPoint Point = Temp->Point;
		float  posY  = Point.GetY();
		float  posX  = Point.GetX();
		string text  = Temp->Text;
		
		/* Render text */
		Font.Render(Color, text, posX, posY);
	}

	return true;
}

bool CCredits::Play(void)
{
	bool ret;

	/* Reset */
	Reset();

	/* Play music */
	ret = Music.Play();
	if (!ret)
		return false;

	/* Set playing state */
	State = CREDITS_PLAYING;

	return true;
}

void CCredits::Stop(void)
{
	/* Credits not playing */
	if (State != CREDITS_PLAYING)
		return;

	/* Stop music */
	Music.Stop();

	/* Set stopped state */
	State = CREDITS_STOPPED;
}

void CCredits::Pause(void)
{
	/* Credits not playing */
	if (State != CREDITS_PLAYING)
		return;

	/* Pause music */
	Music.Pause();

	/* Set paused state */
	State = CREDITS_PAUSED;
}

void CCredits::Resume(void)
{
	/* Credits not paused */
	if (State != CREDITS_PAUSED)
		return;

	/* Resume music */
	Music.Resume();

	/* Set playing state */
	State = CREDITS_PLAYING;
}
