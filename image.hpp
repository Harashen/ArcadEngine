#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

#include <string>

#include <SDL/SDL.h>

using namespace std;


/*!
    Image provides a set of tools for image saving
    and image loading in several formats.
    \author Javier Bermejo García
*/
class Image {
public:
    /*!
        Load an image to a SDL surface.
        \param filename the image filename.
        \return a SDL_Surface of the image or NULL if an error occurs.
    */
    static SDL_Surface *Load(const char *filename);

    /*!
        Load an image to a SDL surface.
        \param filename the image filename.
        \return a SDL_Surface of the image or NULL if an error occurs.
    */
    static SDL_Surface *Load(string filename);

    /*!
        Save a SDL surface in an image file
        (format specified in the filename extension).
        \param surface the SDL_Surface to save.
        \param filename the target filename.
        \return true if success, false otherwise.
    */
    static bool Save(SDL_Surface *surface, const char *filename);

    /*!
        Save a SDL surface in an image file
        (format specified in the filename extension).
        \param surface the SDL_Surface to save.
        \param filename the target filename.
        \return true if success, false otherwise.
    */
    static bool Save(SDL_Surface *surface, string filename);

    /*!
        Save a SDL surface in a BMP image file.
        \param surface the SDL_Surface to save.
        \param filename the target filename.
        \return true if success, false otherwise.
    */
    static bool SaveBMP(SDL_Surface *surface, const char *filename);

    /*!
        Save a SDL surface in a BMP image file.
        \param surface the SDL_Surface to save.
        \param filename the target filename.
        \return true if success, false otherwise.
    */
    static bool SaveBMP(SDL_Surface *surface, string filename);

    /*!
        Save a SDL surface in a PNG image file.
        \param surface the SDL_Surface to save.
        \param filename the target filename.
        \return true if success, false otherwise.
    */
    static bool SavePNG(SDL_Surface *surface, const char *filename);

    /*!
        Save a SDL surface in a PNG image file.
        \param surface the SDL_Surface to save.
        \param filename the target filename.
        \return true if success, false otherwise.
    */
    static bool SavePNG(SDL_Surface *surface, string filename);

    /*!
        Do a vertical flip of the image.
        \param pixels the pointer to the image buffer.
        \param width the image width.
        \param height the image height.
        \param bpp the image bits per pixel.
    */
    static void VFlip(void *pixels, Uint16 width, Uint16 height, Uint32 bpp);
};

#endif

