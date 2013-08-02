#include <GL/gl.h>
#include <GL/glu.h>
#include "config.h"
#include "texture.h"
#include "utils.h"
#include "video.h"


CTexture::CTexture(SDL_Surface *surface, Uint16 width, Uint16 height)
{
	/* Calculate frames and animations */
	Frames     = (surface->w / width);
	Animations = (surface->h / height);

	/* Allocate memory */
	Handle = new Uint32*[Animations];
	
	for (Uint32 i = 0; i < Animations; i++)
        Handle[i] = new Uint32[Frames];

	/* Extract frames */
	for (Uint32 i = 0; i < Animations; i++) {
        for (Uint32 j = 0; j < Frames; j++) {
            SDL_Surface *temp;

            temp = GetFrame(surface, width, height, i, j);
            if (!temp)
                break;
            
            Convert(temp, Handle[i][j]);
            
            SDL_FreeSurface(temp);
        }
    }

	/* Set attributes */
	this->Width  = PowerOfTwo(width);
	this->Height = PowerOfTwo(height);
	this->Alpha  = (surface->flags & SDL_SRCALPHA);
}

CTexture::~CTexture(void)
{
	/* Delete texture */
	if (Handle){
        for (Uint32 i = 0; i < Animations; i++) {
			if (Handle[i]) {
				glDeleteTextures (Frames, Handle[i]);
				delete[] Handle[i];
			}
		}
    }

	/* Free memory */
	delete[] Handle;
}

SDL_Surface *CTexture::GetFrame(SDL_Surface *surface, Uint16 width, Uint16 height, Uint16 row, Uint16 col)
{
	/* Texture dimensions */
	width  = PowerOfTwo(width);
	height = PowerOfTwo(height);
	
	SDL_Surface *temp = NULL;
	SDL_Rect     rect = { col * width, row * height, width, height };

	/* Create surface */
	temp = CVideo::CreateSurface(width, height);
	if (!temp)
		return NULL;

	/* Copy surface */
	CVideo::CopySurface(surface, &rect, temp, NULL);

	return temp;
}

void CTexture::Convert(SDL_Surface *surface, Uint32 &Handle)
{
	Uint32 bpp    =  surface->format->BytesPerPixel;
	Uint32 format = (surface->flags & SDL_SRCALPHA) ? GL_RGBA : GL_RGB;

	/* Convert surface */
	Convert(surface->pixels, surface->w, surface->h, bpp, format, Handle);
}

void CTexture::Convert(void *pixels, Uint16 width, Uint16 height, Uint32 bpp, Uint32 format, Uint32 &Handle)
{
	/* Enable textures */
	glEnable(GL_TEXTURE_2D);

	/* Generate texture */
	glGenTextures(1, &Handle);

	/* Bind texture */
	glBindTexture(GL_TEXTURE_2D, Handle);

	/* Set parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MIN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MAX);

	/* Convert to texture */
	glTexImage2D(GL_TEXTURE_2D, 0, bpp, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

	/* Disable textures */
	glDisable(GL_TEXTURE_2D);
}

Uint16 CTexture::GetWidth(void)
{
	/* Get width */
    return Width;
}

Uint16 CTexture::GetHeight(void)
{
	/* Get height */
    return Height;
}

bool CTexture::GetAlpha(void)
{
	/* Get alpha */
    return Alpha;
}

Uint32 CTexture::GetFrame(Uint16 row, Uint16 col)
{
	/* Return a specific frame */
    return Handle[row][col];
}