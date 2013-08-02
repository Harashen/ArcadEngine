#ifndef _MENU_H_
#define _MENU_H_

#include <map>
#include <string>
#include <vector>

#include "entity.h"
#include "font.h"
#include "music.h"
#include "rect.h"
#include "surface.h"
#include "text.h"

using namespace std;

//! Menu values
enum {
    MENU_NOTHING = 0,
    MENU_PLAY    = 1,
    MENU_OPTIONS = 2,
    MENU_CREDITS = 3,
    MENU_EXIT    = 4,
    MENU_UP      = 8,
    MENU_DOWN    = 9
};


/*!
	CMenu is the class that manages the generation
	and execution of the main menu screen.
	\author Javier Bermejo García
*/
class CMenu {
	CFont    Font;				//!< Font object.
	CMusic   Music;				//!< Background music.

	map<CText *, Sint16> SelectableText;	//!< Selectable texts.
	vector<CText *> NoSelectableText;	 	//!< No Selectable texts.
	vector<CEntity *> Images;				//!< Menu images.

	SDL_Color SelectedColor;		//!< Color for the selected text.
	SDL_Color NoSelectedColor;		//!< Color for unselected text.

	CText *Selected;				//!< Currently selected text.

private:
	/*!
		Load the main menu settings.
		\param filepath the settings filepath.
		\return true if success, false otherwise.
	*/
	bool LoadSettings(string filepath);
	
	/*!
		Load the menu resources.
		\param filepath the resources filepath.
		\return true if success, false otherwise.
	*/
	bool LoadResources(string filepath);

public:
	/*!
		CMenu constructor.
	*/
	CMenu(void);
	
	/*!
		CMenu destructor.
	*/
	~CMenu(void);

	/*!
		Load the main menu resources.
		\param filepath the settings filepath.
		\return true if success, false otherwise.
	*/
	bool Load(string filepath);

	/*!
		Unload the main menu.
	*/
	void Unload(void);

	/*!
		Change the currently selected text.
		\param direction the motion to mo select new text.
	*/
	void Change(Sint16 direction);

	/*!
		Selects the current option.
		\return the current option.
	*/
	Sint16 Select(void);

	/*!
		Start the main menu playback.
		\return true if success, false otherwise.
	*/
	bool Play(void);

	/*!
		Stop the main menu playback.
	*/
	void Stop(void);

	/*!
		Pause the main menu playback.
	*/
	void Pause(void);

	/*!
		Resume the main menu playback.
	*/
	void Resume(void);

	/*!
		Draw the main menu on the screen.
		\return true if success, false otherwise.
	*/
	bool Draw(void);
};

#endif
