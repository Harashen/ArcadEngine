#include <GL/gl.h>
#include "image.h"
#include "surface.h"
#include "utils.h"
#include "video.h"


CSurface::CSurface(void)
{
	/* Initialize variables */
	FlipX   = false;
	FlipY   = false;
	
	Texture = NULL;
}

CSurface::~CSurface(void)
{
	/* Unload surface */
	Unload();
}

void CSurface::SetAttributes(void)
{
	/* No texture */
	if (!Texture)
		return;

	/* Set attributes */
	this->Width  = Texture->GetWidth();
	this->Height = Texture->GetHeight();
}

bool CSurface::Load(const char *filename, Uint16 width, Uint16 height)
{
	/* Unload texture */
	Unload();
	
	Filepath = filename;

	/* Load from file */
	SDL_Surface *surface;

	/* Load image */
	surface = CImage::Load(filename);
	if (!surface)
		return false;

	/* Convert to texture */
	Load(surface, width, height);

	/* Free surface */
	SDL_FreeSurface(surface);

	if (!Texture)
		return false;

	/* Set attributes */
	SetAttributes();

	return true;
}

bool CSurface::Load(string filename, Uint16 width, Uint16 height)
{
	const char *c = filename.c_str();

	/* Load texture */
	return Load(c, width, height);
}

bool CSurface::Load(SDL_Surface *surface, Uint16 width, Uint16 height)
{
	/* Unload texture */
	Unload();

	/* No dimensions specified */
	if (!width)
		width  = surface->w;
	if (!height)
		height = surface->h;
		
	Frames	   = surface->w / width;
	Animations = surface->h / height;

	/* Create texture */
	Texture = new CTexture(surface, width, height);

	/* No memory */
	if (!Texture)
		return false;

	/* Set attributes */
	SetAttributes();

	return true;
}

void CSurface::Unload(void)
{
	/* No texture */
	if (!Texture)
		return;

	/* Reset variables */
	Filepath.clear();
	
	Texture = NULL;
}

bool CSurface::Draw(CRect *srcRect, CRect *dstRect)
{
	Uint32 handle;

	float dX, dY, dW, dH;

	/* No texture */
	if (!Texture)
		return false;

	/* Select frame */
	handle = Texture->GetFrame(CurrAnimation, CurrFrame);

	/* Surface coordinates */
	dX = (dstRect) ? dstRect->GetX() : 0.0;
	dY = (dstRect) ? dstRect->GetY() : 0.0;
	dW = dX + ((dstRect) ? dstRect->GetWidth()  : pVideo->GetWidth());
	dH = dY + ((dstRect) ? dstRect->GetHeight() : pVideo->GetHeight());

	/* Flip texture */
	if (FlipX)
		Flip<float>(dX, dW);
	if (FlipY)
		Flip<float>(dY, dH);

	/* Enable alpha blending */
	if (Texture->GetAlpha())
		glEnable(GL_BLEND);

	/* Enable textures */
	glEnable(GL_TEXTURE_2D);

	/* Bind texture */
	glBindTexture(GL_TEXTURE_2D, handle);

	/* Set opacity */
	glColor3f(1.0, 1.0, 1.0);

	/* Draw texture */
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(dX, dH);
		glTexCoord2f(1, 0); glVertex2f(dW, dH);
		glTexCoord2f(1, 1); glVertex2f(dW, dY);
		glTexCoord2f(0, 1); glVertex2f(dX, dY);
	glEnd();

	/* Disable textures */
	glDisable(GL_TEXTURE_2D);

	/* Disable alpha blending */
	glDisable(GL_BLEND);

	return true;
}

void CSurface::SetFlipX(bool value)
{
	/* Set the FlipX flag */
    FlipX = value;
}

void CSurface::SetFlipY(bool value)
{
	/* Set the FlipY flag */
    FlipY = value;
}
