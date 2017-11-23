#include <ctime>

#include <boost/format.hpp>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "video.hpp"
#include "image.hpp"
#include "timer.hpp"

using namespace boost;


/* Video handler */
Video *gpVideo = NULL;


Video::Video(void)
{
    /* Initialize variables */
    mFps       = 0.0;
    mFpsLimit  = 61.0;
    mFpsTime   = 0;
    mFrameCnt  = 0;
    mFrameTime = 0;

    /* Initialize pointer */
    mpScreen = NULL;
}

Video::~Video(void)
{
    /* Free */
    Free();

    /* Quit SDL vide */
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Video::glSetup(void)
{
    /* Clear color */
    glClearColor(0.0, 0.0, 0.0, 1.0);

    /* Viewport */
    glViewport(0, 0, mWidth, mHeight);

    /* Set projection matrix */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* Orthogonal mode */
    gluOrtho2D(0, mWidth, 0, mHeight);

    /* Set model view matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Alpha blending */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Scissor test */
    glScissor(0, 0, mWidth, mHeight);
    glEnable(GL_SCISSOR_TEST);
}

bool Video::Init(void)
{
    /* Video already initialized */
    if (SDL_WasInit(SDL_INIT_VIDEO)) return true;

    /* Initialize SDL video */
    return !SDL_InitSubSystem(SDL_INIT_VIDEO);
}

void Video::Free(void)
{
    /* Free screen */
    if (mpScreen) SDL_FreeSurface(mpScreen);

    /* Reset pointer */
    mpScreen = NULL;
}

void Video::SetActive(void)
{
    /* Set active video */
    gpVideo = this;
}

bool Video::Configure(Uint16 width, Uint16 height)
{
    Uint32 Flags = (SDL_HWSURFACE | SDL_ASYNCBLIT | SDL_DOUBLEBUF | SDL_OPENGL);

    /* Free screen */
    Free();

    /* Set GL attributes */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /* Set video mode */
    mpScreen = SDL_SetVideoMode(width, height, 24, Flags);
    if (!mpScreen) return false;

    /* Hide cursor */
    SDL_ShowCursor(SDL_DISABLE);

    /* Store settings */
    mWidth  = width;
    mHeight = height;

    /* OpenGL setup */
    glSetup();

    return true;
}

void Video::SetCaption(const char *title)
{
    /* Set window caption */
    SDL_WM_SetCaption(title, NULL);
}

void Video::SetCaption(string title)
{
    const char *c = title.c_str();

    /* Set window caption */
    return SetCaption(c);
}

void Video::Clear(void)
{
    /* Clear screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Video::Flip(bool retard)
{
    float  elapsed;
    Uint64 time;

    /* No screen */
    if (!mpScreen) return false;

    /* Swap buffers */
    SDL_GL_SwapBuffers();
    
    /* No FPS limit */
    if (!retard) return false;

    /* FPS limit */
    Sint64 delay;

    /* Calculate delay */ //FrameTime: tiempo desde el ultimo fotograma
    delay  = mFrameTime + (NSEC_PER_SEC / mFpsLimit);
    delay -= Timer::GetTime();

    /* Sleep */
    if (delay > 0) Timer::NSleep(delay);

    /* Get time */
    time    = Timer::GetTime();
    elapsed = (time - mFpsTime) / (NSEC_PER_SEC * 1.0);

    /* Update */
    mFrameCnt++;
    mFrameTime = time;

    if (elapsed >= 1.0) {
        /* Calculate FPS */
        mFps = mFrameCnt / elapsed;

        /* Update variables */
        mFrameCnt = 0;
        mFpsTime  = time;
    }

    return true;
}

bool Video::GetScreen(void *pixels)
{
    /* No screen */
    if (!mpScreen) return false;

    /* Read pixels */
    glReadPixels(0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    /* Flip pixels */
    Image::VFlip(pixels, mWidth, mHeight, 4);

    return true;
}

bool Video::SaveScreen(string path)
{
    SDL_Surface *surface;
    struct tm   *timeinfo;
    
    time_t tim;
    char   buffer[20];
    bool   ret;

    /* Create surface */
    surface = CreateSurface(mWidth, mHeight);
    if (!surface) return false;

    /* Get screen */
    ret = GetScreen(surface->pixels);
    if (!ret) return false;

    /* Get localtime */
    tim      = time(NULL);
    timeinfo = localtime(&tim);
    
    /* Format filename */
    strftime(buffer, 20, "%d%m%y_%H%M%S.bmp",timeinfo);
    
    /* Save image */
    Image::Save(surface, path + buffer);

    /* Free surface */
    SDL_FreeSurface(surface);

    return ret;
}

SDL_Surface *Video::CreateSurface(Uint16 width, Uint16 height)
{
    Uint32 rmask;
    Uint32 gmask;
    Uint32 bmask;
    Uint32 amask;

    /* Colour mask */
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xFF000000;
        gmask = 0x00FF0000;
        bmask = 0x0000FF00;
        amask = 0x000000FF;
    #else
        rmask = 0x000000FF;
        gmask = 0x0000FF00;
        bmask = 0x00FF0000;
        amask = 0xFF000000;
    #endif

    /* Create surface */
    return SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
}

bool Video::CopySurface(SDL_Surface *src, SDL_Rect *srRect, SDL_Surface *dst, SDL_Rect *dstRect)
{
    Uint32 flags = src->flags;
    bool   ret;

    /* Disable alpha */
    if (flags & SDL_SRCALPHA) {
        SDL_SetAlpha(src, 0, 0xFF);
    }

    /* Blit surface */
    ret = !SDL_BlitSurface(src, srRect, dst, dstRect);

    /* Enable alpha */
    if (flags & SDL_SRCALPHA) {
        SDL_SetAlpha(src, flags, 0xFF);
    }

    return ret;
}

Uint16 Video::GetWidth(void)
{
    /* Get window width */
    return mWidth;
}

Uint16 Video::GetHeight(void)
{
    /* Get window height */
    return mHeight;
}

float Video::GetFps (void)
{
    /* Get frames per second */
    return mFps;
}
