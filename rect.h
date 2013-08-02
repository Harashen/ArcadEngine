#ifndef _RECT_H_
#define _RECT_H_

#include <SDL/SDL.h>

#include <cstdio>
#include <cmath>
#include <sstream>
#include "point.h"

using namespace std;


/*!
    CRect is the class that manages the rectangle of a entity
	and its initial position.
    \author Javier Bermejo García
*/
class CRect {
	/* Coordinates */
	CPoint Point;		//!< Point with the rectangle position.
	CPoint Initial;		//!< Inital position of the rectangle.

	/* Dimensions */
	Sint32 Width;		//!< Width of the rectangle.
	Sint32 Height;		//!< Height of the rectangle.

public:
	/*!
        CRect constructor.
        \param x the initial value of x.
        \param y the initial value of y.
        \param w the initial value of the width.
        \param h the initial value of the height.
    */
	CRect(float x = 0, float y = 0, Sint32 w = 0, Sint32 h = 0);

	/*!
        Check if exists intersection between two rectangles.
        \param Rect rectangle to compare with.
        \param Result the resulting rectangle intersection..
        \return true if success, false otherwise.
    */
	bool Intersection(CRect *Rect, CRect &Result);
	
	/*!
        Add the actual position plus the rectangle position.
        \param Rect the rectangle whose position is to be added.
    */
	bool Add(CRect *Rect);

	/*!
        Draw the rectangle with a color and opacity.
        \param Color color to paint the area of rectangle.
        \param alpha color opacity.
    */
	void Draw(SDL_Color Color, float alpha);
	
	/*!
        Set rectangle.
        \param x coordinate X.
        \param y coordinate Y.
        \param w width of the rectangle.
        \param h height of the rectangle.
    */
    void Set(float x, float y, Sint32 w, Sint32 h);
    
    /*!
        Set coordinate X of the rectangle.
        \param value coordinate X.
    */
    void SetX(float value);
    
    /*!
        Set coordinate Y of the rectangle.
        \param value coordinate Y.
    */
    void SetY(float value);
    
    /*!
        Set coordinate X of the initial position of rectangle.
        \param value the X of the initial position.
    */
    void SetInitialX(float value);
    
    /*!
        Set coordinate Y of the initial position of rectangle.
        \param value the Y of the initial position.
    */
    void SetInitialY(float value);
    
    /*!
        Set width of the rectangle.
        \param value width of the rectangle.
    */
    void SetWidth(Sint32 value);
    
    /*!
        Set height of the rectangle.
        \param value height of the rectangle.
    */
    void SetHeight(Sint32 value);
    
    /*!
        Get coordinate X of the rectangle.
        \return the coordinate X.
    */
    float GetX(void);
    
    /*!
        Get coordinate Y of the rectangle.
        \return the coordinate Y.
    */
    float GetY(void);
    
    /*!
        Get width of the rectangle.
        \return the width.
    */
    Sint32 GetWidth(void);
    
    /*!
        Get height of the rectangle.
        \return the height.
    */
    Sint32 GetHeight(void);
};

#endif
