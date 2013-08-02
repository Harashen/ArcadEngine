#include <boost/format.hpp>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <ctime>

#include "image.h"
#include "timer.h"
#include "utils.h"
#include "video.h"

using namespace boost;


/* Video handler */
CVideo *pVideo = NULL;


CVideo::CVideo(void)
{
	/* Initialize variables */
	Fps        = 0.0;
	FpsLimit   = 61.0;
	FpsTime    = 0;
	FrameCnt   = 0;
	FrameTime  = 0;

	/* Initialize pointer */
	Screen = NULL;
}

CVideo::~CVideo(void)
{
	/* Free */
	Free();

	/* Quit SDL vide */
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void CVideo::glSetup(void)
{
	/* Clear color */
	glClearColor(0.0, 0.0, 0.0, 1.0);

	/* Viewport */
	glViewport(0, 0, Width, Height);

	/* Set projection matrix */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Orthogonal mode */
	gluOrtho2D(0, Width, 0, Height);

	/* Set model view matrix */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Alpha blending */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Scissor test */
	glScissor(0, 0, Width, Height);
	glEnable(GL_SCISSOR_TEST);
}

bool CVideo::Init(void)
{
	/* Video already initialized */
	if (SDL_WasInit(SDL_INIT_VIDEO))
		return true;

	/* Initialize SDL video */
	return !SDL_InitSubSystem(SDL_INIT_VIDEO);
}

void CVideo::Free(void)
{
	/* Free screen */
	if (Screen)
		SDL_FreeSurface(Screen);

	/* Reset pointer */
	Screen = NULL;
}

void CVideo::SetActive(void)
{
	/* Set active video */
	pVideo = this;
}

bool CVideo::Configure(Uint16 width, Uint16 height)
{
	Uint32 Flags = (SDL_HWSURFACE | SDL_ASYNCBLIT | SDL_DOUBLEBUF | SDL_OPENGL);

	/* Free screen */
	Free();

	/* Set GL attributes */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/* Set video mode */
	Screen = SDL_SetVideoMode(width, height, 24, Flags);
	if (!Screen)
		return false;

	/* Hide cursor */
	SDL_ShowCursor(SDL_DISABLE);

	/* Store settings */
	this->Width  = width;
	this->Height = height;

	/* OpenGL setup */
	glSetup();

	return true;
}

void CVideo::SetCaption(const char *title)
{
	/* Set window caption */
	SDL_WM_SetCaption(title, NULL);
}

void CVideo::SetCaption(string title)
{
	const char *c = title.c_str();

	/* Set window caption */
	return SetCaption(c);
}

void CVideo::Clear(void)
{
	/* Clear screen */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool CVideo::Flip(bool retard)
{
	float  elapsed;
	Uint64 time;

	/* No screen */
	if (!Screen)
		return false;

	/* Swap buffers */
	SDL_GL_SwapBuffers();
	
	/* No FPS limit */
	if (!retard)
		return false;

	/* FPS limit */
	Sint64 delay;

	/* Calculate delay */ //FrameTime: tiempo desde el ultimo fotograma
	delay  = FrameTime + (NSEC_PER_SEC / FpsLimit);
	delay -= CTimer::GetTime();

	/* Sleep */
	if (delay > 0)
		CTimer::NSleep(delay);

	/* Get time */
	time    = CTimer::GetTime();
	elapsed = (time - FpsTime) / (NSEC_PER_SEC * 1.0);

	/* Update */
	FrameCnt++;
	FrameTime = time;

	if (elapsed >= 1.0) {
		/* Calculate FPS */
		Fps = FrameCnt / elapsed;

		/* Update variables */
		FrameCnt = 0;
		FpsTime  = time;
	}

	return true;
}

bool CVideo::GetScreen(void *pixels)
{
	/* No screen */
	if (!Screen)
		return false;

	/* Read pixels */
	glReadPixels(0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	/* Flip pixels */
	CImage::VFlip(pixels, Width, Height, 4);

	return true;
}

bool CVideo::SaveScreen(string path)
{
	SDL_Surface *surface;
	struct tm   *timeinfo;
	
	time_t tim;
    char   buffer[20];
	bool   ret;

	/* Create surface */
	surface = CreateSurface(Width, Height);
	if (!surface)
		return false;

	/* Get screen */
	ret = GetScreen(surface->pixels);
	if (!ret)
		return false;

	/* Get localtime */
	tim      = time(NULL);
    timeinfo = localtime(&tim);
	
	/* Format filename */
    strftime(buffer, 20, "%d%m%y_%H%M%S.bmp",timeinfo);
	
	/* Save image */
	CImage::Save(surface, path + buffer);

	/* Free surface */
	SDL_FreeSurface(surface);

	return ret;
}

SDL_Surface *CVideo::CreateSurface(Uint16 Width, Uint16 Height)
{
	Uint32 rmask, gmask, bmask, amask;

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
	return SDL_CreateRGBSurface(SDL_SWSURFACE, Width, Height, 32, rmask, gmask, bmask, amask);
}

bool CVideo::CopySurface(SDL_Surface *src, SDL_Rect *srcRect, SDL_Surface *dst, SDL_Rect *dstRect)
{
	Uint32 Flags = src->flags;
	bool   ret;

	/* Disable alpha */
	if (Flags & SDL_SRCALPHA)
		SDL_SetAlpha(src, 0, 0xFF);

	/* Blit surface */
	ret = !SDL_BlitSurface(src, srcRect, dst, dstRect);

	/* Enable alpha */
	if (Flags & SDL_SRCALPHA)
		SDL_SetAlpha(src, Flags, 0xFF);

	return ret;
}

Uint16 CVideo::GetWidth(void)
{
	/* Get window width */
    return Width;
}

Uint16 CVideo::GetHeight(void)
{
	/* Get window height */
    return Height;
}

float CVideo::GetFps (void)
{
	/* Get frames per second */
    return Fps;
}
