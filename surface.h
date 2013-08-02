#ifndef _SURFACE_H_
#define _SURFACE_H_

#include <string>
#include <SDL/SDL.h>
#include "animation.h"
#include "image.h"
#include "rect.h"
#include "texture.h"

using namespace std;


/*!
	CSurface is the class that manages the loading,
	saving and displaying of the animation textures.
	\author Javier Bermejo García
*/
class CSurface : public CAnimation {
	CTexture *Texture;	//!< Sprite texture.
	CImage   *Image;	//!< Sprite image.
	
	Uint16 Width;		//!< Surface frame width.
	Uint16 Height;		//!< Surface frame height.

	bool FlipX;			//!< Horizontal flip flag.
	bool FlipY;			//!< Vertical flip flag.
	
	string Filepath;	//!< Surface filepath.

private:
	/*!
		Set the surface attributes from the texture.
	*/
	void SetAttributes(void);

public:
	/*!
		CSurface constructor.
	*/
	CSurface(void);

	/*!
		CSurface destructor.
	*/
	~CSurface(void);

	/*!
		Load a surface from a file.
		\param Filename the surface file.
		\param width the surface frame width (set as 0 if no animation).
		\param height the surface frame height (set as 0 if no animation).
		\return true if success, false otherwise.
	*/
	bool Load(const char *filename, Uint16 width = 0, Uint16 height = 0);

	/*!
		Load a surface from a file.
		\param Filename the surface file.
		\param width the surface frame width (set as 0 if no animation).
		\param height the surface frame height (set as 0 if no animation).
		\return true if success, false otherwise.
	*/
	bool Load(string filename, Uint16 width = 0, Uint16 height = 0);

	/*!
		Load a surface from a SDL surface
		\param surface the SDL_Surface pointer.
		\param width the surface frame width (set as 0 if no animation).
		\param height the surface frame height (set as 0 if no animation)..
		\return true if success, false otherwise.
	*/
	bool Load(SDL_Surface *surface, Uint16 width = 0, Uint16 height = 0);

	/*!
		Unload the surface.
	*/
	void Unload(void);

	/*!
		Draw the surface on the screen.
		\param srcRect the source rect to draw.
		\param dstRect the target rect to draw.
		\return true if success, false otherwise.
	*/
	bool Draw(CRect *srcRect = NULL, CRect *dstRect = NULL);
	
	/*!
        Set the FlipX flag.
        \param value true to flip surface, false otherwise.
    */
    void SetFlipX(bool value);
    
    /*!
        Set the FlipY flag.
        \param value true to flip surface, false otherwise.
    */
    void SetFlipY(bool value);
};

#endif
