#include <algorithm>
#include <cmath>

#include "point.h"

using namespace std;

CPoint::CPoint (void)
{
    X = -1;
    Y = -1;
}

CPoint::CPoint (float x, float y)
{
    X = x;
    Y = y;
}

void CPoint::SetX(float x)
{
    X = x;
}

void CPoint::SetY(float y)
{
    Y = y;
}

void CPoint::SetPoint(CPoint Point)
{
    float x, y;
    
    x = Point.GetX();
    y = Point.GetY();
    
    SetPoint (x, y);
}

void CPoint::SetPoint(float x, float y)
{
    SetX (x);
    SetY (y);
}

float CPoint::GetX (void)
{
    return X;
}

float CPoint::GetY (void)
{
    return Y;
}

bool CPoint::EqualPoints(CPoint Point)
{
    float x, y;
    
    x = Point.GetX();
    y = Point.GetY();
    
    if ( (x == X) && (y == Y) )
        return true;
    
    return false;
}

float CPoint::Distance(CPoint Point)
{
    float dist;
    
    float x, y;
    
    x = DistanceX(Point.GetX() );
    y = DistanceY(Point.GetY() );
    
    y = abs(y);
    x = abs(x);
    
    dist = max(y, x);

    return dist;
}

float CPoint::DistanceX(float x)
{    
    x -= X;

    return x;
}

float CPoint::DistanceY(float y)
{    
    y -= Y;

    return y;
}
