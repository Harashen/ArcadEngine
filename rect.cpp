#include <GL/gl.h>

#include "rect.hpp"


Rect::Rect(float x, float y, Sint32 w, Sint32 h)
{
    /* Set attributes */
    mPoint.SetPoint(x, y);
    mWidth  = w;
    mHeight = h;
}

bool Rect::Intersection(Rect *rect, Rect &result)
{/* Horizontal intersection */
    float aMin = mPoint.GetX();
    float aMax = aMin + mWidth;
    float bMin = rect->GetX();
    float bMax = bMin + rect->GetWidth();

    if (bMin > aMin) aMin = bMin;
    if (bMax < aMax) aMax = bMax;

    /* No intersection */
    if (aMin >= aMax) return false;

    result.SetX(aMin);
    result.SetWidth((Sint32) (aMax - aMin));

    /* Vertical intersection */
    aMin = mPoint.GetY();
    aMax = aMin + mHeight;
    bMin = rect->GetY();
    bMax = bMin + rect->GetHeight();

    if (bMin > aMin) aMin = bMin;
    if (bMax < aMax) aMax = bMax;

    /* No intersection */
    if (aMin >= aMax) return false;

    result.SetY(aMin);
    result.SetHeight((Sint32) (aMax - aMin));

    return true;	
}

bool Rect::Add(Rect *rect)
{
    float x = rect->GetX() + mInitial.GetX();
    float y = rect->GetY() + mInitial.GetY();
    
    mPoint.SetPoint(x, y);
    
    return true;
}

void Rect::Draw(SDL_Color color, float alpha)
{
    /* Enable alpha blending */
    if (alpha < 1.0) glEnable(GL_BLEND);

    /* Set rect color */
    glColor4f(color.r, color.g, color.b, alpha);

    /* Draw rect */
    glBegin(GL_QUADS);
    glVertex2f(mPoint.GetX(), mPoint.GetY());
    glVertex2f(mPoint.GetX() + mWidth, mPoint.GetY());
    glVertex2f(mPoint.GetX() + mWidth, mPoint.GetY() + mHeight);
    glVertex2f(mPoint.GetX(), mPoint.GetY() + mHeight);
    glEnd();

    /* Disable alpha blending */
    glDisable(GL_BLEND);
}

void Rect::Set(float x, float y, Sint32 w, Sint32 h)
{
    mPoint.SetPoint(x, y);
    SetWidth(w);
    SetHeight(h);
}

void Rect::SetX (float value)
{
    mPoint.SetX(value);
}

void Rect::SetY(float value)
{
    mPoint.SetY(value);
}

void Rect::SetInitialX(float value)
{
    mInitial.SetX(value);
}

void Rect::SetInitialY(float value)
{
    mInitial.SetY(value);
}

void Rect::SetWidth(Sint32 value)
{
    mWidth = value;
}

void Rect::SetHeight(Sint32 value)
{
    mHeight = value;
}

float Rect::GetX(void)
{
    return mPoint.GetX();
}

float Rect::GetY(void)
{
    return mPoint.GetY();
}

Sint32 Rect::GetWidth(void)
{
    return mWidth;
}

Sint32 Rect::GetHeight(void)
{
    return mHeight;
}

