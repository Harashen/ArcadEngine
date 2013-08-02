#include <GL/gl.h>
#include "rect.h"


CRect::CRect(float x, float y, Sint32 w, Sint32 h)
{
	/* Set attributes */
	Point.SetPoint(x, y);
	Width  = w;
	Height = h;
}

bool CRect::Intersection(CRect *Rect, CRect &Result)
{
	float Amin, Amax, Bmin, Bmax;

	/* Horizontal intersection */
	Amin = Point.GetX();
	Amax = Amin + Width;
	Bmin = Rect->GetX();
	Bmax = Bmin + Rect->GetWidth();

	if (Bmin > Amin)
		Amin = Bmin;
	if (Bmax < Amax)
		Amax = Bmax;

	/* No intersection */
	if (Amin >= Amax)
		return false;

	Result.SetX(Amin);
	Result.SetWidth((Sint32) (Amax - Amin));

	/* Vertical intersection */
	Amin = Point.GetY();
	Amax = Amin + Height;
	Bmin = Rect->GetY();
	Bmax = Bmin + Rect->GetHeight();

	if (Bmin > Amin)
		Amin = Bmin;
	if (Bmax < Amax)
		Amax = Bmax;

	/* No intersection */
	if (Amin >= Amax)
		return false;

	Result.SetY(Amin);
	Result.SetHeight((Sint32) (Amax - Amin));

	return true;	
}

bool CRect::Add(CRect *Rect)
{
	float x = Rect->GetX() + Initial.GetX();
	float y = Rect->GetY() + Initial.GetY();
	
	Point.SetPoint(x, y);
	
	return true;
}

void CRect::Draw(SDL_Color Color, float alpha)
{
	/* Enable alpha blending */
	if (alpha < 1.0)
		glEnable(GL_BLEND);

	/* Set rect color */
	glColor4f(Color.r, Color.g, Color.b, alpha);

	/* Draw rect */
	glBegin(GL_QUADS);
		glVertex2f(Point.GetX(), Point.GetY());
		glVertex2f(Point.GetX() + Width, Point.GetY());
		glVertex2f(Point.GetX() + Width, Point.GetY() + Height);
		glVertex2f(Point.GetX(), Point.GetY() + Height);
	glEnd();

	/* Disable alpha blending */
	glDisable(GL_BLEND);
}

void CRect::Set(float x, float y, Sint32 w, Sint32 h)
{
    Point.SetPoint(x, y);
    SetWidth(w);
    SetHeight(h);
}

void CRect::SetX (float value)
{
	Point.SetX(value);
}

void CRect::SetY(float value)
{
	Point.SetY(value);
}

void CRect::SetInitialX(float value)
{
	Initial.SetX(value);
}

void CRect::SetInitialY(float value)
{
	Initial.SetY(value);
}

void CRect::SetWidth(Sint32 value)
{
	Width = value;
}

void CRect::SetHeight(Sint32 value)
{
	Height = value;
}

float CRect::GetX(void)
{
	return Point.GetX();
}

float CRect::GetY(void)
{
	return Point.GetY();
}

Sint32 CRect::GetWidth(void)
{
	return Width;
}

Sint32 CRect::GetHeight(void)
{
	return Height;
}

