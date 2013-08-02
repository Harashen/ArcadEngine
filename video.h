#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <SDL/SDL.h>
#include "texture.h"


/*!
	CVideo is the class that manages the video subsystem
	of the engine (using OpenGL for rendering).
	\author Javier Bermejo García
*/
class CVideo {
	SDL_Surface *Screen;	//!< Screen surface.

	Uint32 FrameCnt;		//!< Frame counter.
	Uint64 FrameTime;		//!< Time of the last displayed frame.
	Uint64 FpsTime;			//!< Time of the last FPS value.

	Uint16 Width;			//!< Video width.
	Uint16 Height;			//!< Video height.

	float Fps;				//!< Current FPS value.
	float FpsLimit;			//!< Maximum FPS limit.

private:
	/*!
		Free the video buffers.
	*/
	void Free(void);

	/*!
		Setup the OpenGL renderer.
	*/
	void glSetup(void);
	
	/*!
		Get the screen framebuffer.
		\param pixels the buffer to store the frame.
		\return true if success, false otherwise.
	*/
	bool GetScreen(void *pixels);

public:
	/*!
		CVideo constructor.
	*/
	CVideo(void);

	/*!
		CVideo destructor.
	*/
	~CVideo(void);

	/*!
		Initialize the video subsystem.
		\return true if success, false otherwise.
	*/
	bool Init(void);

	/*!
		Set the active video object.
	*/
	void SetActive(void);

	/*!
		Configure the video subsystem.
		\param Width the video render width.
		\param Height the video render height.
		\return true if success, false otherwise.
	*/
	bool Configure(Uint16 width, Uint16 height);

	/*!
		Set the window caption.
		\param Title the window caption.
	*/
	void SetCaption(const char *title);

	/*!
		Set the window caption.
		\param Title the window caption.
	*/
	void SetCaption(string title);

	/*!
		Clear the entire screen.
	*/
	void Clear(void);

	/*!
		Flip the video buffers (waiting for the video timer).
		\return true if success, false otherwise.
	*/
	bool Flip(bool retard);

	/*!
		Save the screen framebuffer in a file.
        \param path the location to save image.
		\return true if success, false otherwise.
	*/
	bool SaveScreen(string path);
	
	/*!
        Get window width.
        \return the window width.
    */
    Uint16 GetWidth(void);
    
    /*!
        Get window height.
        \return the window height.
    */
    Uint16 GetHeight(void);
    
    /*!
        Get frames per second.
        \return frames per second.
    */
    float GetFps(void);

	/*!
		Create a SDL surface.
		\param width the surface width.
		\param height the surface height.
		\return the allocated SDL_Surface or NULL if an error occured.
	*/
	static SDL_Surface *CreateSurface(Uint16 width, Uint16 height);

	/*!
		Copy from a SDL surface to another SDL surface.
		\param src the source surface.
		\param srcRect the rect area to copy from the source surface.
		\param dst the target surface.
		\param dstRect the rect area where copy the pixels onto the target surface.
		\return true if success, false otherwise.
	*/
	static bool CopySurface(SDL_Surface *src, SDL_Rect *srcRect, SDL_Surface *dst, SDL_Rect *dstRect);
};

//! Active video object.
extern CVideo *pVideo;

#endif
