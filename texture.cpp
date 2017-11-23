#include <GL/gl.h>
#include <GL/glu.h>

#include "texture.hpp"
#include "config.hpp"
#include "utils.hpp"
#include "video.hpp"


Texture::Texture(SDL_Surface *surface, Uint16 width, Uint16 height)
{
    /* Calculate frames and animations */
    mFrames     = (surface->w / width);
    mAnimations = (surface->h / height);

    /* Allocate memory */
    mpHandle = new Uint32*[mAnimations];
    
    for (Uint32 i = 0; i < mAnimations; i++) {
        mpHandle[i] = new Uint32[mFrames];
    }

    /* Extract frames */
    for (Uint32 i = 0; i < mAnimations; i++) {
        for (Uint32 j = 0; j < mFrames; j++) {
            SDL_Surface *temp;

            temp = GetFrame(surface, width, height, i, j);
            if (!temp) break;
            
            Convert(temp, mpHandle[i][j]);
            
            SDL_FreeSurface(temp);
        }
    }

    /* Set attributes */
    mWidth  = PowerOfTwo(width);
    mHeight = PowerOfTwo(height);
    mAlpha  = (surface->flags & SDL_SRCALPHA);
}

Texture::~Texture(void)
{
    /* Delete texture */
    if (mpHandle) {
        for (Uint32 i = 0; i < mAnimations; i++) {
            if (mpHandle[i]) {
                glDeleteTextures (mFrames, mpHandle[i]);
                delete[] mpHandle[i];
            }
        }
    }

    /* Free memory */
    delete[] mpHandle;
}

SDL_Surface *Texture::GetFrame(SDL_Surface *surface, Uint16 width, Uint16 height, Uint16 row, Uint16 col)
{
    /* Texture dimensions */
    width  = PowerOfTwo(width);
    height = PowerOfTwo(height);
    
    SDL_Surface *temp = NULL;
    SDL_Rect     rect = {static_cast<Sint16>(col * width), static_cast<Sint16>(row * height), width, height};

    /* Create surface */
    temp = Video::CreateSurface(width, height);
    if (!temp) return NULL;

    /* Copy surface */
    Video::CopySurface(surface, &rect, temp, NULL);

    return temp;
}

void Texture::Convert(SDL_Surface *surface, Uint32 &mpHandle)
{
    Uint32 bpp    =  surface->format->BytesPerPixel;
    Uint32 format = (surface->flags & SDL_SRCALPHA) ? GL_RGBA : GL_RGB;

    /* Convert surface */
    Convert(surface->pixels, surface->w, surface->h, bpp, format, mpHandle);
}

void Texture::Convert(void *pixels, Uint16 width, Uint16 height, Uint32 bpp, Uint32 format, Uint32 &mpHandle)
{
    /* Enable textures */
    glEnable(GL_TEXTURE_2D);

    /* Generate texture */
    glGenTextures(1, &mpHandle);

    /* Bind texture */
    glBindTexture(GL_TEXTURE_2D, mpHandle);

    /* Set parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MIN);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MAX);

    /* Convert to texture */
    glTexImage2D(GL_TEXTURE_2D, 0, bpp, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

    /* Disable textures */
    glDisable(GL_TEXTURE_2D);
}

Uint16 Texture::GetWidth(void)
{
    /* Get width */
    return mWidth;
}

Uint16 Texture::GetHeight(void)
{
    /* Get height */
    return mHeight;
}

bool Texture::GetAlpha(void)
{
    /* Get alpha */
    return mAlpha;
}

Uint32 Texture::GetFrame(Uint16 row, Uint16 col)
{
    /* Return a specific frame */
    return mpHandle[row][col];
}