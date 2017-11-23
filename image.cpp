#include <png.h>
#include <SDL/SDL_image.h>

#include "image.hpp"
#include "video.hpp"


SDL_Surface *Image::Load(const char *filename)
{
    /* Load image */
    return IMG_Load(filename);
}

SDL_Surface *Image::Load(string filename)
{
    const char *c = filename.c_str();

    /* Load image */
    return Load(c);
}

bool Image::Save(SDL_Surface *surface, const char *filename)
{
    string S(filename);

    /* Save image */
    return Save(surface, S);
}

bool Image::Save(SDL_Surface *surface, string filename)
{
    string ext;
    Uint32 pos;

    /* Find extension */
    pos = filename.find_last_of(".");

    /* Extract extension */
    if (pos != string::npos) {
        ext = filename.substr(pos + 1);
    }

    /* Save image */
    if (!ext.compare("bmp")) {
        return SaveBMP(surface, filename);
    } else {
        return SavePNG(surface, filename);
    }
}

bool Image::SaveBMP(SDL_Surface *surface, const char *filename)
{
    /* Save surface */
    return !SDL_SaveBMP(surface, filename);
}

bool Image::SaveBMP(SDL_Surface *surface, string filename)
{
    const char *c = filename.c_str();

    /* Save as BMP */
    return SaveBMP(surface, c);
}

bool Image::SavePNG(SDL_Surface *surface, const char *filename)
{
    png_bytep  *pngRows;
    png_structp pngPtr;
    png_infop   pngInfo;

    FILE  *Fp;
    Sint32 i;

    /* Create file */
    Fp = fopen(filename, "wb");
    if (!Fp) return false;

    /* Allocate memory */
    pngRows = new png_bytep[surface->h];

    /* Extract rows */
    for (i = 0; i < surface->h; i++) {
        pngRows[i] = ((Uint8 *)surface->pixels) + (surface->pitch * i);
    }

    /* Create structs */
    pngPtr  = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    pngInfo = png_create_info_struct(pngPtr);

    /* Init I/O */
    png_init_io(pngPtr, Fp);

    /* Compress image */
    png_set_IHDR(pngPtr, pngInfo, surface->w, surface->h, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    /* Write image */
    png_write_info (pngPtr, pngInfo);
    png_write_image(pngPtr, pngRows);
    png_write_end  (pngPtr, NULL);

    /* Close file */
    fclose(Fp);

    return true;
}

bool Image::SavePNG(SDL_Surface *surface, string filename)
{
    const char *c = filename.c_str();

    /* Save as PNG */
    return SavePNG(surface, c);
}

void Image::VFlip(void *pixels, Uint16 width, Uint16 height, Uint32 bpp)
{
    Uint8 *buffer;

    Uint32 size;
    Uint32 stride;
    Sint32 i;
    Sint32 j;

    /* Calculate size */
    stride = width  * bpp;
    size   = stride * height;

    /* Allocate memory */
    buffer = new Uint8[size];

    /* Copy surface */
    memcpy(buffer, pixels, size);

    /* Invert rows */
    for (i = (height - 1), j = 0; i >= 0; i--, j++) {
        void *src = ((Uint8 *)buffer) + (stride * i);
        void *dst = ((Uint8 *)pixels) + (stride * j);

        /* Copy data */
        memcpy(dst, src, stride);
    }

    /* Free memory */
    delete[] buffer;
}

