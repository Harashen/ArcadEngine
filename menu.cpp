#include <boost/format.hpp>
#include "menu.h"
#include "script.h"
#include "utils.h"

using namespace boost;


CMenu::CMenu(void)
{	
	Selected = NULL;
}

CMenu::~CMenu(void)
{	
	/* Unload */
	Unload();
}

bool CMenu::LoadSettings(string filepath)
{
	CScript Script;
	bool    ret;
	string  fontPath;
	string  musicPath;
	Uint16  size;

	/* Load script */
	ret = Script.Load(filepath + "menu.ini");
	if (!ret)
		return false;
		
	/* Get font values */
	fontPath = Script.GetString("Font");
	size     = Script.GetValue<Uint16>("Size");
	
	/* Load menu font */
	ret = Font.Load(filepath + fontPath, size);
	if (!ret) {
		/* Unload */
		Unload();

		return false;
	}
	
	/* Load colours */
	SelectedColor   = Script.GetColor("Selected.Color");
	NoSelectedColor = Script.GetColor("NoSelected.Color");
	
	/* Load music */
	musicPath = Script.GetString("Music");
	if (!musicPath.empty())
		Music.Load(filepath + musicPath);

	return true;
}

bool CMenu::LoadResources(string filepath)
{
	CScript Script;
	bool    ret;
	Sint16  images, selectable, noSelectable, options;
	Sint16  i;

	/* Load script */
	ret = Script.Load(filepath + "menu.ini");
	if (!ret)
		return false;
	
	/* Get number of resources */
	images       = Script.GetValue<Uint16>("Images");
    selectable   = Script.GetValue<Uint16>("Selectable");
    noSelectable = Script.GetValue<Uint16>("NoSelectable");
    options      = Script.GetValue<Uint16>("Options");
		
	/* Load images */
	for (i = 0; i < images; i++) {
		CEntity *Entity = new CEntity;
		string   path, prefix;
		
		/* Generate prefix */
		prefix = str(format("Image[%d].") % i);
        
		/* Get image path */
        path = Script.GetString(prefix + "Path");
        
		/* Push image */
        Images.push_back(Entity);
        
        Entity->SetBasepath(filepath);
        ret = Entity->Load(filepath + path);
        if (!ret)
            return false;
	}
	
	/* Load selectable text */
	for (i = 0; i < selectable; i++) {
		CText *Text = new CText;
		CPoint Point;
		string prefix, value;
		Sint16 action;
		
		/* Generate prefix */
		prefix = str(format("Text[%d].") % i);
        
		/* Get selectable text */
        value = Script.GetString(prefix + "Text");
		
		/* Get position */
		float x = Script.GetValue<float>(prefix + "PosX");
		float y = Script.GetValue<float>(prefix + "PosY");
		
		Point.SetPoint(x, y);
		
		/* Set text */
		Text->Text  = value;
		Text->Point = Point;
		
		/* Get selectable text action*/
        action = Script.GetValue<Sint16>(prefix + "Action");
		
		/* Select default text */
		if (i == 0)
			Selected = Text;
        
		/* Insert text */
		SelectableText.insert(pair<CText *, Sint16>(Text, action));
	}
	
	/* Load non selectable text */
	for (i = 0; i < noSelectable; i++) {
		CText *Text = new CText;
		CPoint Point;
		string prefix, value;
		
		/* Generate prefix */
		prefix = str(format("TextN[%d].") % i);
        
		/* Get selectable text */
        value = Script.GetString(prefix + "Text");
		
		/* Get position */
		float x = Script.GetValue<float>(prefix + "PosX");
		float y = Script.GetValue<float>(prefix + "PosY");
		
		Point.SetPoint(x, y);
		
		/* Set text */
		Text->Text  = value;
		Text->Point = Point;
		
		/* Push text */
        NoSelectableText.push_back(Text);
	}

	return true;
}

bool CMenu::Load(string filepath)
{
	bool ret;

	/* Load settings */
	ret = LoadSettings(filepath);
	if (!ret) {
		/* Unload */
		Unload();

		return false;
	}
	
	/* Load settings */
	ret = LoadResources(filepath);
	if (!ret) {
		/* Unload */
		Unload();

		return false;
	}

	return true;

}

void CMenu::Unload(void)
{
	vector<CEntity *>::iterator    it1;
	map<CText *, Sint16>::iterator it2;
	vector<CText *>::iterator      it3;

	/* Unload images */
	foreach(Images, it1) {
		CEntity *Entity = *it1;

		/* Free Image */
		delete Entity;
	}

	/* Unload selectable texts */
	foreach(SelectableText, it2) {
		CText *Text = (*it2).first;

		/* Free selectable text */
		delete Text;
		SelectableText.erase(it2);
	}

	/* Unload non selectable texts */
	foreach(NoSelectableText, it3) {
		CText *Text = *it3;

		/* Free Image */
		delete Text;
	}

	/* Unload selected element */
	if (Selected)
		delete Selected;

	/* Unload font */
	Font.Unload();

	/* Unload music */
	Music.Unload();
}

void CMenu::Change(Sint16 direction)
{
	map<CText *, Sint16>::iterator it;
	
	/* Get element selected */
	it = SelectableText.find(Selected);
	
	/* Change direction */
	switch (direction) {
	case MENU_UP:
		if (it == SelectableText.end())
			it = SelectableText.end();
		else
			it++;
			
		if (it == SelectableText.end())
			it--;
			
		Selected = (*it).first;
		
		break;
		
	case MENU_DOWN:
		if (it == SelectableText.begin())
			it = SelectableText.begin();
		else
			it--;
			
		Selected = (*it).first;
		
		break;
	}
}

Sint16 CMenu::Select(void)
{
	return SelectableText.find(Selected)->second;
}

bool CMenu::Play(void)
{
	/* Play music */
	return Music.Play();
}

void CMenu::Stop(void)
{
	/* Stop music */
	Music.Stop();
}

void CMenu::Pause(void)
{
	/* Pause music */
	Music.Pause();
}

void CMenu::Resume(void)
{
	/* Resume music */
	Music.Resume();
}

bool CMenu::Draw(void)
{
	vector<CEntity *>::iterator    it1;
	map<CText *, Sint16>::iterator it2;
	vector<CText *>::iterator      it3;

	bool ret;
	
	/* Draw images */
	foreach(Images, it1) {
		CEntity *Entity = *it1;

		/* Draw entity */
		ret = Entity->Draw();
		if (!ret)
			return false;
	}

	/* Draw selectable texts */
	foreach(SelectableText, it2) {
		CText *Text  = (*it2).first;
		CPoint Point = Text->Point;
		string text  = Text->Text;
		
		
		/* Defacult colour */
		SDL_Color Color = NoSelectedColor;

		/* Change colour */
		if (Selected == Text)
			Color = SelectedColor;
			
		/* Draw text */
		ret = Font.Render(Color, text, Point.GetX(), Point.GetY());
		if (!ret)
			return false;
	}

	/* Draw non selectable texts */
	foreach(NoSelectableText, it3) {
		CText *Text  = *it3;
		CPoint Point = Text->Point;
		string text  = Text->Text;
			
		/* Draw text */
		ret = Font.Render(NoSelectedColor, text, Point.GetX(), Point.GetY());
		if (!ret)
			return false;
	}
	
	return true;
}
