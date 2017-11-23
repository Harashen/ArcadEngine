#include <algorithm>
#include <cmath>

#include "point.hpp"

using namespace std;

Point::Point (void)
{
    mX = -1;
    mY = -1;
}

Point::Point (float x, float y)
{
    mX = x;
    mY = y;
}

void Point::SetX(float x)
{
    mX = x;
}

void Point::SetY(float y)
{
    mY = y;
}

void Point::SetPoint(Point point)
{
    float x = point.GetX();
    float y = point.GetY();
    
    SetPoint(x, y);
}

void Point::SetPoint(float x, float y)
{
    SetX(x);
    SetY(y);
}

float Point::GetX (void)
{
    return mX;
}

float Point::GetY (void)
{
    return mY;
}

bool Point::EqualPoints(Point point)
{
    float x = point.GetX();
    float y = point.GetY();
    
    if (x == mX && y == mY) return true;
    
    return false;
}

float Point::Distance(Point point)
{  
    float x = DistanceX(point.GetX());
    float y = DistanceY(point.GetY());

    y = abs(y);
    x = abs(x);
    
    return max(y, x);
}

float Point::DistanceX(float x)
{    
    x -= mX;

    return x;
}

float Point::DistanceY(float y)
{    
    y -= mY;

    return y;
}
