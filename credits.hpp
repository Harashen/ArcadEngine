#ifndef _CREDITS_HPP_
#define _CREDITS_HPP_

#include <SDL/SDL.h>
#include <string>
#include <vector>

#include "font.hpp"
#include "music.hpp"
#include "point.hpp"
#include "rect.hpp"
#include "text.hpp"

using namespace std;


//! Credits state values.
enum {
    CREDITS_STOPPED = 0,
    CREDITS_PAUSED  = 1,
    CREDITS_PLAYING = 2
};


/*!
    Credits is the class that manages the credits
    scene of the game.
    \author Javier Bermejo García
*/
class Credits {
    Font  mFont;		  //!< Font object.
    Music mMusic;		  //!< Music object.
    vector<Text *> mTextVect; //!< Credits text.

    Point mPosition;	  //!< Text rendering position.
    Uint16 mSize;		  //!< Font rendering size.
    Sint16 mState;		  //!< Current state.
    float  mSpeed;		  //!< Text scrolling speed.
    
    SDL_Color mColor;	   //!< Text rendering colour.
    SDL_Color mBackground; //!< Background rendering colour.
    
    float mAlpha;		  //!< Background opacity;
    Rect mScreenRect;	  //!< Screen rectangle.

    Uint32 mWidth;		  //!< Total width of the rendered text.
    Uint32 mHeight;		  //!< Total height of the rendered text.

private:
    /*!
        Load the credits settings.
        \param filepath the settings filepath.
        \return true if success, false otherwise.
    */
    bool LoadSettings(string filepath);

    /*!
        Load the credits text.
        \param filepath the credits filepath.
        \return true if success, false otherwise.
    */
    bool LoadText(string filepath);
    
    /*!
        Unload the credits scene.
    */
    void Unload(void);

public:
    /*!
        Credits constructor.
    */
    Credits(void);
    
    /*!
        Credits destructor.
    */
    ~Credits (void);

    /*!
        Load the credits scene.
        \param filepath the settings filepath.
        \return true if success, false otherwise.
    */
    bool Load(string filepath);

    /*!
        Reset the credits scene.
    */
    void Reset(void);
    
    /*!
        Set screen rectangle.
        \param width window width.
        \param height window height.
    */
    void SetScreen(Sint32 width, Sint32 height);
    
    /*!
        Update the animation of the credits scene.
        \return true if success, false otherwise.
    */
    bool Update(void);

    /*!
        Draw the credits scene on the screen.
        \return true if success, false otherwise.
    */
    bool Draw(void);

    /*!
        Start the credits scene playback.
        \return true if success, false otherwise.
    */
    bool Play(void);

    /*!
        Stop the credits scene playback.
    */
    void Stop(void);

    /*!
        Pause the credits scene playback.
    */
    void Pause(void);

    /*!
        Resume the credits scene playback.
    */
    void Resume(void);
};

#endif

