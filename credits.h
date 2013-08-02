#ifndef _CREDITS_H_
#define _CREDITS_H_

#include <SDL/SDL.h>
#include <string>
#include <vector>

#include "font.h"
#include "music.h"
#include "point.h"
#include "rect.h"
#include "text.h"

using namespace std;


//! Credits state values.
enum {
	CREDITS_STOPPED = 0,
	CREDITS_PAUSED  = 1,
	CREDITS_PLAYING = 2
};


/*!
	CCredits is the class that manages the credits
	scene of the game.
	\author Javier Bermejo García
*/
class CCredits {
	CFont  Font;		  //!< Font object.
	CMusic Music;		  //!< Music object.
	vector<CText *> Text; //!< Credits text.

	CPoint Position;	  //!< Text rendering position.
	Uint16 Size;		  //!< Font rendering size.
	Sint16 State;		  //!< Current state.
	float  Speed;		  //!< Text scrolling speed.
	
	SDL_Color Color;	  //!< Text rendering colour.
	SDL_Color Background; //!< Background rendering colour.
	
	float Alpha;		  //!< Background opacity;
	CRect ScreenRect;	  //!< Screen rectangle.

	Uint32 Width;		  //!< Total width of the rendered text.
	Uint32 Height;		  //!< Total height of the rendered text.

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
		CCredits constructor.
	*/
	CCredits(void);
	
	/*!
        CCredits destructor.
    */
    ~CCredits (void);

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

