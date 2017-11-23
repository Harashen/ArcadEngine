#include <boost/format.hpp>

#include "menu.hpp"
#include "script.hpp"
#include "utils.hpp"

using namespace boost;


Menu::Menu(void)
{	
    mpSelected = NULL;
}

Menu::~Menu(void)
{	
    /* Unload */
    Unload();
}

bool Menu::LoadSettings(string filepath)
{
    Script script;
    bool   ret;
    string fontPath;
    string musicPath;
    Uint16 size;

    /* Load script */
    ret = script.Load(filepath + "menu.ini");
    if (!ret) return false;
        
    /* Get font values */
    fontPath = script.GetString("Font");
    size     = script.GetValue<Uint16>("Size");
    
    /* Load menu font */
    ret = mFont.Load(filepath + fontPath, size);
    if (!ret) {
        /* Unload */
        Unload();

        return false;
    }
    
    /* Load colours */
    mSelectedColor   = script.GetColor("Selected.Color");
    mNoSelectedColor = script.GetColor("NoSelected.Color");
    
    /* Load music */
    musicPath = script.GetString("Music");
    if (!musicPath.empty()) {
        mMusic.Load(filepath + musicPath);
    }

    return true;
}

bool Menu::LoadResources(string filepath)
{
    Script script;
    bool   ret;
    Sint16 images;
    Sint16 selectable;
    Sint16 noSelectable;
    Sint16 options;
    Sint16 i;

    /* Load script */
    ret = script.Load(filepath + "menu.ini");
    if (!ret) return false;
    
    /* Get number of resources */
    images       = script.GetValue<Uint16>("Images");
    selectable   = script.GetValue<Uint16>("Selectable");
    noSelectable = script.GetValue<Uint16>("NoSelectable");
    options      = script.GetValue<Uint16>("Options");
        
    /* Load images */
    for (i = 0; i < images; i++) {
        Entity *entity = new Entity;
        string  path, prefix;
        
        /* Generate prefix */
        prefix = str(format("Image[%d].") % i);
        
        /* Get image path */
        path = script.GetString(prefix + "Path");
        
        /* Push image */
        mImages.push_back(entity);
        
        entity->SetBasepath(filepath);
        ret = entity->Load(filepath + path);
        if (!ret) return false;
    }
    
    /* Load selectable text */
    for (i = 0; i < selectable; i++) {
        Text  *text = new Text;
        Point  point;
        string prefix;
        string value;
        Sint16 action;
        
        /* Generate prefix */
        prefix = str(format("Text[%d].") % i);
        
        /* Get selectable text */
        value = script.GetString(prefix + "Text");
        
        /* Get position */
        float x = script.GetValue<float>(prefix + "PosX");
        float y = script.GetValue<float>(prefix + "PosY");
        
        point.SetPoint(x, y);
        
        /* Set text */
        text->mText  = value;
        text->mPoint = point;
        
        /* Get selectable text action*/
        action = script.GetValue<Sint16>(prefix + "Action");
        
        /* Select default text */
        if (i == 0) mpSelected = text;
        
        /* Insert text */
        mSelectableText.insert(pair<Text *, Sint16>(text, action));
    }
    
    /* Load non selectable text */
    for (i = 0; i < noSelectable; i++) {
        Text  *text = new Text;
        Point  point;
        string prefix;
        string value;
        
        /* Generate prefix */
        prefix = str(format("TextN[%d].") % i);
        
        /* Get selectable text */
        value = script.GetString(prefix + "Text");
        
        /* Get position */
        float x = script.GetValue<float>(prefix + "PosX");
        float y = script.GetValue<float>(prefix + "PosY");
        
        point.SetPoint(x, y);
        
        /* Set text */
        text->mText  = value;
        text->mPoint = point;
        
        /* Push text */
        mNoSelectableText.push_back(text);
    }
    
    /* Load options */
    for (i = 0; i < options; i++) {

    }

    return true;
}

bool Menu::Load(string filepath)
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

void Menu::Unload(void)
{
    vector<Entity *>::iterator    it1;
    map<Text *, Sint16>::iterator it2;
    vector<Text *>::iterator      it3;

    /* Unload images */
    foreach(mImages, it1) {
        Entity *entity = *it1;

        /* Free Image */
        delete entity;
    }

    /* Unload selectable texts */
    foreach(mSelectableText, it2) {
        Text *text = (*it2).first;

        /* Free selectable text */
        delete text;
        mSelectableText.erase(it2);
    }

    /* Unload non selectable texts */
    foreach(mNoSelectableText, it3) {
        Text *text = *it3;

        /* Free Image */
        delete text;
    }

    /* Unload selected element */
    if (mpSelected)	delete mpSelected;

    /* Unload font */
    mFont.Unload();

    /* Unload music */
    mMusic.Unload();
}

void Menu::Change(Sint16 direction)
{
    map<Text *, Sint16>::iterator it;
    
    /* Get element selected */
    it = mSelectableText.find(mpSelected);
    
    /* Change direction */
    switch (direction) {
        case MENU_UP:
            if (it == mSelectableText.end()) {
                it = mSelectableText.end();
            } else {
                it++;
            }
            
            if (it == mSelectableText.end()) {
                it--;
            }
            
            mpSelected = (*it).first;
        
            break;
        
        case MENU_DOWN:
            if (it == mSelectableText.begin()) {
                it = mSelectableText.begin();
            } else {
                it--;
            }
            
            mpSelected = (*it).first;
        
            break;
    }
}

Sint16 Menu::Select(void)
{
    return mSelectableText.find(mpSelected)->second;
}

bool Menu::Play(void)
{
    /* Play music */
    return mMusic.Play();
}

void Menu::Stop(void)
{
    /* Stop music */
    mMusic.Stop();
}

void Menu::Pause(void)
{
    /* Pause music */
    mMusic.Pause();
}

void Menu::Resume(void)
{
    /* Resume music */
    mMusic.Resume();
}

bool Menu::Draw(void)
{
    vector<Entity *>::iterator    it1;
    map<Text *, Sint16>::iterator it2;
    vector<Text *>::iterator      it3;

    bool ret;
    
    /* Draw images */
    foreach(mImages, it1) {
        Entity *entity = *it1;

        /* Draw entity */
        ret = entity->Draw();
        if (!ret) return false;
    }

    /* Draw selectable texts */
    foreach(mSelectableText, it2) {
        Text  *text  = (*it2).first;
        Point  point = text->mPoint;
        string str   = text->mText;
        
        
        /* Defacult colour */
        SDL_Color color = mNoSelectedColor;

        /* Change colour */
        if (mpSelected == text) {
            color = mSelectedColor;
        }
            
        /* Draw text */
        ret = mFont.Render(color, str, point.GetX(), point.GetY());
        if (!ret) return false;
    }

    /* Draw non selectable texts */
    foreach(mNoSelectableText, it3) {
        Text  *text  = *it3;
        Point  point = text->mPoint;
        string str   = text->mText;
            
        /* Draw text */
        ret = mFont.Render(mNoSelectedColor, str, point.GetX(), point.GetY());
        if (!ret) return false;
    }
    
    return true;
}
