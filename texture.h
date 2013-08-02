#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL/SDL.h>


/*!
	CTexture is the class that manages the generation of OpenGL
	textures from different sources (static images, animations...).
	\author Javier Bermejo García
*/
class CTexture {
	Uint32 **Handle;	//!< Texture handler.

	Uint16 Width;		//!< Texture width.
	Uint16 Height;		//!< Texture height.

	Uint32 Frames;		//!< Number of frames.
	Uint32 Animations;	//!< Number of animations.
	
	bool Alpha;			//!< Alpha channel flag.

private:
	/*!
		Get a frame from an animation in SDL surface format.
		\param surface the SDL_Surface of the animation.
		\param width the frame width.
		\param height the frame height.
		\param row the numbers of animations.
		\param col the number of frames per animation.
		\return the SDL_Surface of the extracted frame.
	*/
	SDL_Surface *GetFrame(SDL_Surface *surface, Uint16 width, Uint16 height, Uint16 row, Uint16 col);
	
	/*!
		Texture free
	*/
	void Free(void);

public:
	/*!
		CTexture constructor.
		\param surface the SDL_Surface to convert.
		\param width the frame width.
		\param height the frame height.
	*/
	CTexture(SDL_Surface *surface, Uint16 width, Uint16 height);

	/*!
		CTexture destructor.
	*/
	~CTexture(void);

	/*!
		Convert a SDL surface to an OpenGL texture.
		\param surface the SDL_Surface to convert.
		\param handle the variable to store the texture handler.
	*/
	static void Convert(SDL_Surface *surface, Uint32 &handle);

	/*!
		Convert an image to an OpenGL texture.
		\param pixels the pointer to the image buffer.
		\param width the image width.
		\param height the image height.
		\param bpp the image bits per pixel.
		\param format the image format.
		\param handle the variable to store the texture handler.
	*/
	static void Convert(void *pixels, Uint16 width, Uint16 height, Uint32 bpp, Uint32 format, Uint32 &handle);
	
	/*!
        Get the texture width.
        \return the texture width.
    */
    Uint16 GetWidth(void);
    
    /*!
        Get the texture height.
        \return the texture height.
    */
    Uint16 GetHeight(void);
    
    /*!
        Get the texture alpha.
        \return the texture alpha.
    */
    bool GetAlpha(void);
    
    /*!
		Get the frame from a position
        \param row the row where is the frame.
        \param col the col where is the frame.
        \return the frame.
    */
    Uint32 GetFrame(Uint16 row, Uint16 col);
};

#endif
