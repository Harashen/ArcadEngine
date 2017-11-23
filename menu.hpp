#ifndef _MENU_HPP_
#define _MENU_HPP_

#include <map>
#include <string>
#include <vector>

#include "entity.hpp"
#include "font.hpp"
#include "music.hpp"
#include "rect.hpp"
#include "surface.hpp"
#include "text.hpp"

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
    Menu is the class that manages the generation
    and execution of the main menu screen.
    \author Javier Bermejo García
*/
class Menu {
    Font    mFont;				//!< Font object.
    Music   mMusic;				//!< Background music.

    map<Text *, Sint16> mSelectableText;	//!< Selectable texts.
    vector<Text *>      mNoSelectableText;	//!< No Selectable texts.
    vector<Entity *>    mImages;			//!< Menu images.

    SDL_Color mSelectedColor;		//!< Color for the selected text.
    SDL_Color mNoSelectedColor;		//!< Color for unselected text.

    Text *mpSelected;				//!< Currently selected text.

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
        Menu constructor.
    */
    Menu(void);
    
    /*!
        Menu destructor.
    */
    ~Menu(void);

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
