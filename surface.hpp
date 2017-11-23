#ifndef _SURFACE_HPP_
#define _SURFACE_HPP_

#include <string>

#include <SDL/SDL.h>

#include "animation.hpp"
#include "image.hpp"
#include "rect.hpp"
#include "texture.hpp"

using namespace std;


/*!
    Surface is the class that manages the loading,
    saving and displaying of the animation textures.
    \author Javier Bermejo García
*/
class Surface : public Animation {
    Texture *mpTexture;	//!< Sprite texture.
    Image   *mpImage;	//!< Sprite image.
    
    Uint16 mWidth;		//!< Surface frame width.
    Uint16 mHeight;		//!< Surface frame height.

    bool mFlipX;			//!< Horizontal flip flag.
    bool mFlipY;			//!< Vertical flip flag.
    
    string mFilepath;	//!< Surface filepath.

private:
    /*!
        Set the surface attributes from the texture.
    */
    void SetAttributes(void);

public:
    /*!
        Surface constructor.
    */
    Surface(void);

    /*!
        Surface destructor.
    */
    ~Surface(void);

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
        \param srRect the source rect to draw.
        \param dstRect the target rect to draw.
        \return true if success, false otherwise.
    */
    bool Draw(Rect *srRect = NULL, Rect *dstRect = NULL);
    
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
