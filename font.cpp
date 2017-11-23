#include <iostream>
#include <vector>

#include <GL/gl.h>

#include "font.hpp"
#include "config.hpp"
#include "utils.hpp"
#include "video.hpp"

using namespace std;


Font::Font(void)
{
    /* Initialize variables */
    mpFont = NULL;
    mSkip = 0;
    mSize = 0;
}

Font::~Font(void)
{
    /* Unload */
    Unload();
}

bool Font::Load(const char *filepath, Uint32 size)
{
    /* Unload */
    Unload();

    /* Create font */
#ifdef HIGH_QUALITY
    mpFont = new FTGLTextureFont(filepath);
#else
    mpFont = new FTGLBitmapFont(filepath);
#endif

    /* Error */
    if (!mpFont) return false;

    /* Set size */
    SetSize(size);

    return true;
}

bool Font::Load(string filepath, Uint32 size)
{
    const char *c = filepath.c_str();

    /* Load font */
    return Load(c, size);
}

void Font::Unload(void)
{
    /* Free font */
    if (mpFont) delete mpFont;

    /* Reset pointer */
    mpFont = NULL;
}

void Font::GetLines(vector<string> &lines, string text)
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
        if (line.empty()) {
            line = " ";
        }

        /* Push line */
        lines.push_back(line);
    }
}

void Font::GetSize(string text, Uint32 &width, Uint32 &height)
{
    vector<string> lines;
    vector<string>::iterator it;
    Uint32 nLines;

    /* No font */
    if (!mpFont) return;

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

        /* Get dimensions */
        FTBBox box = mpFont->BBox(line);

        /* Get width/height */
        Uint32 w = box.Upper().X();
        Uint32 h = box.Upper().Y();

        /* Update dimensions */
        width   = (w > width) ? w : width;
        height += h;
    }

    /* Add line spacing */
    height += mSkip * (nLines - 1);
}

void Font::SetSize(Uint32 size)
{
    /* No font/size */
    if (!mpFont || !size) return;

    /* Check size */
    if (size == mSize) return;

    /* Set size */
    mpFont->FaceSize(size);

    /* Set attributes */
    mSize = size;
    mSkip = mpFont->LineHeight();
}

void Font::SetScreen(Sint32 width, Sint32 height)
{
    /* Set screen rect */
    mScreenRect.Set(0, 0, width, height);
}

bool Font::Render(SDL_Color color, string text, float x, float y, bool center)
{
    vector<string> lines;
    vector<string>::iterator it;

    /* No font */
    if (!mpFont) return false;

    /* Get lines */
    GetLines(lines, text);
    
    if (center) {
        /* Check lines */
        foreach (lines, it) {
            const char *line = (*it).c_str();

            FTBBox box;
            Uint32 w;

            /* Get dimensions */
            box = mpFont->BBox(line);

            /* Get width/height */
            w = box.Upper().X();

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
        const char *str = (*it).c_str();

        /* Render line */
        mpFont->Render(str);

        /* Move to next line */
        glRasterPos2f(0, -mSkip);
        glTranslatef (0, -mSkip, 0);
    }

    /* Pop matrix */
    glPopMatrix();

    /* Disable textures */
    glDisable(GL_TEXTURE_2D);

    return true;
}

bool Font::Render(SDL_Color color, const char *text, float x, float y, bool center)
{
    string s(text);

    /* Render text */
    return Render(color, s, x, y, center);
}
