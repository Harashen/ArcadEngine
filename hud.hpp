#ifndef _HUD_HPP_
#define _HUD_HPP_

#include <map>
#include <string>

#include "font.hpp"
#include "rect.hpp"

using namespace std;


/*!
    Hud is the class that manages the in-game HUD display.
    \author Javier Bermejo García
*/
class Hud {
    Font mFont;					//!< Font object.

    SDL_Color mScoreColor;   	//!< Score string colour.
    SDL_Color mHighScoreColor;	//!< Hightscore string colour.
    SDL_Color mFpsColor;     	//!< FPS string colour.
    SDL_Color mLivesColor;    	//!< Lives string colour.
    SDL_Color mTextColor;      	//!< Text string colour.
    SDL_Color mScreenColor;		//!< Screen color
    
    Rect mScoreRect;     	//!< Score string rect.
    Rect mHighScoreRect; 	//!< Highscore string rect.
    Rect mFpsRect;      	//!< FPS string rect.
    Rect mLivesRect;     	//!< Lives string rect.
    Rect mTextRect;     	//!< Text string rect.
    Rect mPressRect;  		//!< Press string rect.
    Rect mScreenRect;       //!< Screen rectangle.

private:
    /*!
        Load the HUD settings.
        \param filepath the settings filepath.
        \return true if success, false otherwise.
    */
    bool LoadSettings(string filepath);

    /*!
        Draw a string on the screen.
        \param text the string to draw.
        \param color the string colour.
        \param rect the string drawing rect.
        \param center to center the string.
        \return true if success, false otherwise.
    */
    bool DrawString(string text, SDL_Color color, Rect rect, bool center = false);

public:
    /*!
        Initialize the HUD subsystem.
        \param filepath the settings filepath.
        \return true if success, false otherwise.
    */
    bool Init(string filepath);

    /*!
        Set the active HUD object.
    */
    void SetActive(void);
    
    /*!
        Set screen rectangle.
        \param width window width.
        \param height window height.
    */
    void SetScreen(Sint32 width, Sint32 height);

    /*!
        Draw the score string on the screen.
        \return true if success, false otherwise.
    */
    bool DrawScore(void);
    
    /*!
        Draw the highscore string on the screen.
        \return true if success, false otherwise.
    */
    bool DrawHighScore(void);

    /*!
        Draw the FPS string on the screen.
        \return true if success, false otherwise.
    */
    bool DrawFps(void);

    /*!
        Draw the player lives string on the screen.
        \return true if success, false otherwise.
    */
    bool DrawLives(void);
    
    /*!
        Draw the points string between levels on the screen.
        \param alpha background opacity.
        \return true if success, false otherwise.
    */
    bool DrawPoints(float alpha);
    
    /*!
        Draw a string on the screen.
        \param text the string to draw.
        \param alpha background opacity.
        \return true if success, false otherwise.
    */
    bool DrawText(string text, float alpha);
    
    /*!
        Draw the press string on the screen.
        \return true if success, false otherwise.
    */
    bool DrawPress(void);
    
    /*!
        Draw the credits string on the screen.
        \param credits the actual credits availables.
        \return true if success, false otherwise.
    */
    bool DrawCredits(Uint16 credits);
};

//! Active HUD object.
extern Hud *gpHud;

#endif
