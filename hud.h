#ifndef _HUD_H_
#define _HUD_H_

#include <map>
#include <string>
#include "font.h"
#include "rect.h"

using namespace std;


/*!
	CHud is the class that manages the in-game HUD display.
	\author Javier Bermejo García
*/
class CHud {
	CFont Font;					//!< Font object.

	SDL_Color ScoreColor;   	//!< Score string colour.
    SDL_Color HighScoreColor;	//!< Hightscore string colour.
    SDL_Color FpsColor;     	//!< FPS string colour.
    SDL_Color LivesColor;    	//!< Lives string colour.
    SDL_Color TextColor;      	//!< Text string colour.
	SDL_Color ScreenColor;		//!< Screen color
    
    CRect ScoreRect;     	//!< Score string rect.
    CRect HighScoreRect; 	//!< Highscore string rect.
    CRect FpsRect;      	//!< FPS string rect.
    CRect LivesRect;     	//!< Lives string rect.
    CRect TextRect;     	//!< Text string rect.
    CRect PressRect;  		//!< Press string rect.
    CRect ScreenRect;       //!< Screen rectangle.

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
		\param Color the string colour.
		\param Rect the string drawing rect.
		\param center to center the string.
		\return true if success, false otherwise.
	*/
	bool DrawString(string text, SDL_Color Color, CRect Rect, bool center = false);

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
extern CHud *pHud;

#endif
