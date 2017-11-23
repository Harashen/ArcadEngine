#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include "utils.hpp"

using namespace std;


/*!
    Log is the class that manages the logging
    output of the application.
    \author Miguel Botón Campo
*/
class Log : public ostream {
    stringbuf mBuffer;	//!< String buffer.

public:
    /*!
        Log constructor.
    */
    Log(void) : ostream(&mBuffer) {}

    /*!
        Set the active log object.
    */
    void SetActive(void);

    /*!
        Use the standard output for logging.
        \param Value true to enable, false to disable.
    */
    void StdOut(bool value);

    /*!
        Print a message (using printf style).
        \param Format the message format.
    */
    void Print(const char *format, ...);

    /*!
        Print the string buffer to the standard output.
    */
    void PrintOut(void);
};

//! Active log object.
extern Log *gpLog;

#endif

