#ifndef _TEXT_HPP_
#define _TEXT_HPP_

#include <string>

#include "point.hpp"

using namespace std;

/*!
    Text is the class that has a string to show
    and a position.
    \author Javier Bermejo Garc�a
*/
class Text {
public:
    string mText;        //!< The string to draw.
    Point  mPoint;       //!< Text rendering position.
};
#endif
