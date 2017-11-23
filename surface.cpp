#include <GL/gl.h>

#include "surface.hpp"
#include "image.hpp"
#include "utils.hpp"
#include "video.hpp"


Surface::Surface(void)
{
    /* Initialize variables */
    mFlipX    = false;
    mFlipY    = false;	
    mpTexture = NULL;
}

Surface::~Surface(void)
{
    /* Unload surface */
    Unload();
}

void Surface::SetAttributes(void)
{
    /* No texture */
    if (!mpTexture)	return;

    /* Set attributes */
    mWidth  = mpTexture->GetWidth();
    mHeight = mpTexture->GetHeight();
}

bool Surface::Load(const char *filename, Uint16 width, Uint16 height)
{
    /* Unload texture */
    Unload();
    
    mFilepath = filename;

    /* Load from file */
    SDL_Surface *surface;

    /* Load image */
    surface = Image::Load(filename);
    if (!surface) return false;

    /* Convert to texture */
    Load(surface, width, height);

    /* Free surface */
    SDL_FreeSurface(surface);

    if (!mpTexture)	return false;

    /* Set attributes */
    SetAttributes();

    return true;
}

bool Surface::Load(string filename, Uint16 width, Uint16 height)
{
    const char *c = filename.c_str();

    /* Load texture */
    return Load(c, width, height);
}

bool Surface::Load(SDL_Surface *surface, Uint16 width, Uint16 height)
{
    /* Unload texture */
    Unload();

    /* No dimensions specified */
    if (!width)  width  = surface->w;
    if (!height) height = surface->h;
        
    mFrames	    = surface->w / width;
    mAnimations = surface->h / height;

    /* Create texture */
    mpTexture = new Texture(surface, width, height);

    /* No memory */
    if (!mpTexture)	return false;

    /* Set attributes */
    SetAttributes();

    return true;
}

void Surface::Unload(void)
{
    /* No texture */
    if (!mpTexture)	return;

    /* Reset variables */
    mFilepath.clear();
    
    mpTexture = NULL;
}

bool Surface::Draw(Rect *srRect, Rect *dstRect)
{
    Uint32 handle;

    /* No texture */
    if (!mpTexture) return false;

    /* Select frame */
    handle = mpTexture->GetFrame(mCurrAnimation, mCurrFrame);

    /* Surface coordinates */
    float dX = (dstRect) ? dstRect->GetX() : 0.0;
    float dY = (dstRect) ? dstRect->GetY() : 0.0;
    float dW = dX + ((dstRect) ? dstRect->GetWidth()  : gpVideo->GetWidth());
    float dH = dY + ((dstRect) ? dstRect->GetHeight() : gpVideo->GetHeight());

    /* Flip texture */
    if (mFlipX)	Flip<float>(dX, dW);
    if (mFlipY)	Flip<float>(dY, dH);

    /* Enable alpha blending */
    if (mpTexture->GetAlpha()) {
        glEnable(GL_BLEND);
    }

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

void Surface::SetFlipX(bool value)
{
    /* Set the mFlipX flag */
    mFlipX = value;
}

void Surface::SetFlipY(bool value)
{
    /* Set the mFlipY flag */
    mFlipY = value;
}
