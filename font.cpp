#include <GL/gl.h>
#include <vector>
#include "config.h"
#include "font.h"
#include "utils.h"
#include "video.h"
#include <iostream>

using namespace std;


CFont::CFont(void)
{
	/* Initialize variables */
	Font = NULL;
	Skip = 0;
	Size = 0;
}

CFont::~CFont(void)
{
	/* Unload */
	Unload();
}

bool CFont::Load(const char *filepath, Uint32 size)
{
	/* Unload */
	Unload();

	/* Create font */
#ifdef HIGH_QUALITY
	Font = new FTGLTextureFont(filepath);
#else
	Font = new FTGLBitmapFont(filepath);
#endif

	/* Error */
	if (!Font)
		return false;

	/* Set size */
	SetSize(size);

	return true;
}

bool CFont::Load(string filepath, Uint32 size)
{
	const char *c = filepath.c_str();

	/* Load font */
	return Load(c, size);
}

void CFont::Unload(void)
{
	/* Free font */
	if (Font)
		delete Font;

	/* Reset pointer */
	Font = NULL;
}

void CFont::GetLines(vector<string> &lines, string text)
{
	Sint32 n = 0;

	/* Get lines */
	while (n >= 0) {
		string line;

		/* Find '\n' */
		n = text.find('\n', 0);

		/* Get line */
		line = text.substr(0, n);
		text = text.substr(n + 1, -1);

		/* Empty line */
		if (line.empty())
			line = " ";

		/* Push line */
		lines.push_back(line);
	}
}

void CFont::GetSize(string text, Uint32 &width, Uint32 &height)
{
	vector<string> lines;
	vector<string>::iterator it;
	Uint32 nLines;

	/* No font */
	if (!Font)
		return;

	/* Get lines */
	GetLines(lines, text);

	/* Number of lines */
	nLines = lines.size();

	/* Initial dimensions */
	width  = 0;
	height = 0;

	/* Check lines */
	foreach (lines, it) {
		const char *line = (*it).c_str();

		FTBBox Box;
		Uint32 w, h;

		/* Get dimensions */
		Box = Font->BBox(line);

		/* Get width/height */
		w = Box.Upper().X();
		h = Box.Upper().Y();

		/* Update dimensions */
		width   = (w > width) ? w : width;
		height += h;
	}

	/* Add line spacing */
	height += Skip * (nLines - 1);
}

void CFont::SetSize(Uint32 size)
{
	/* No font/size */
	if (!Font || !size)
		return;

	/* Check size */
	if (size == this->Size)
		return;

	/* Set size */
	Font->FaceSize(size);

	/* Set attributes */
	this->Size = size;
	this->Skip = Font->LineHeight();
}

void CFont::SetScreen(Sint32 width, Sint32 height)
{
	/* Set screen rect */
	ScreenRect.Set(0, 0, width, height);
}

bool CFont::Render(SDL_Color color, string text, float x, float y, bool center)
{
	vector<string> lines;
	vector<string>::iterator it;

	/* No font */
	if (!Font)
		return false;

	/* Get lines */
	GetLines(lines, text);
	
	if (center) {
		/* Check lines */
		foreach (lines, it) {
			const char *line = (*it).c_str();

			FTBBox Box;
			Uint32 w;

			/* Get dimensions */
			Box = Font->BBox(line);

			/* Get width/height */
			w = Box.Upper().X();

			/* Update dimensions */
			x -= w * 0.5;
		}			
	}

	/* Set colour */
	glColor3f(color.r, color.g, color.b);

	/* Set size */
	//SetSize(size);

	/* Enable textures */
	glEnable(GL_TEXTURE_2D);

	/* Push matrix */
	glPushMatrix();
	glLoadIdentity();

	/* Translate */ 
	glRasterPos2f(x, y);
	glTranslatef (x, y, 0);

	/* Render lines */
	foreach (lines, it) {
		const char *Str = (*it).c_str();

		/* Render line */
		Font->Render(Str);

		/* Move to next line */
		glRasterPos2f(0, -Skip);
		glTranslatef (0, -Skip, 0);
	}

	/* Pop matrix */
	glPopMatrix();

	/* Disable textures */
	glDisable(GL_TEXTURE_2D);

	return true;
}

bool CFont::Render(SDL_Color color, const char *text, float x, float y, bool center)
{
	string s(text);

	/* Render text */
	return Render(color, s, x, y, center);
}
