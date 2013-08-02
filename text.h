#ifndef _TEXT_H_
#define _TEXT_H_

#include <string>

#include "point.h"

using namespace std;

/*!
    CText is the class that has a string to show
	and a position.
    \author Javier Bermejo García
*/
class CText {
public:
    string Text;        //!< The string to draw.
    CPoint Point;       //!< Text rendering position.
};
#endif
