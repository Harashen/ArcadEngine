#ifndef _FONT_HPP_
#define _FONT_HPP_

#include <string>
#include <vector>

#include <FTGL/ftgl.h>
#include <SDL/SDL.h>

#include "rect.hpp"

using namespace std;


/*!
    Font is the class that provides the subsystem to render texts.
    It uses truetype fonts and supports OpenGL rendering.
    \author Javier Bermejo García
*/
class Font {
    FTFont *mpFont;		 //!< Truetype font.
    float   mSkip;		 //!< Line skip value.
    Rect    mScreenRect; //!< Screen rectangle.
    
public:
    Uint32 mSize;	//!< Font render size.

private:
    /*!
        Get the lines from a text.
        \param lines the array to store the lines.
        \param text the text that contains the lines to extract.
    */
    void GetLines(vector<string> &lines, string text);

public:
    /*!
        Font constructor.
    */
    Font(void);

    /*!
        Font destructor.
    */
    ~Font(void);

    /*!
        Load a truetype font.
        \param filepath the font file.
        \param size the font render size.
        \return true if success, false otherwise.
    */
    bool Load(const char *filepath, Uint32 size = 10);

    /*!
        Load a truetype font.
        \param filepath the font file.
        \param size the font render size.
        \return true if success, false otherwise.
    */
    bool Load(string filepath, Uint32 size = 10);

    /*!
        Unload the truetype font.
    */
    void Unload(void);
    
    /*!
        Set screen rectangle.
        \param width window width.
        \param height window height.
    */
    void SetScreen(Sint32 width, Sint32 height);

    /*!
        Render a text and display it on the screen.
        \param color the text colour.
        \param text the text to render.
        \param x the X-axis position of the text.
        \param y the Y-axis position of the text.
        \param size the text render size.
        \return true if success, false otherwise.
    */
    bool Render(SDL_Color color, const char *text, float x, float y, bool center = false);

    /*!
        Render a text and display it on the screen.
        \param color the text colour.
        \param text the text to render.
        \param x the X-axis position of the text.
        \param y the Y-axis position of the text.
        \param size the text render size.
        \param center center text.
        \return true if success, false otherwise.
    */
    bool Render(SDL_Color color, string text, float x, float y, bool center = false);
    
    /*!
        Set the font render size.
        \param size the new font size.
    */
    void SetSize(Uint32 size);
    
    /*!
        Get the size of a text (the render width and height).
        \param text the text to render.
        \param width the variable to store the width.
        \param height the variable to store the height.
    */
    void GetSize(string text, Uint32 &width, Uint32 &height);
};

#endif
