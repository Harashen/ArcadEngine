#include <cstdio>
#include <cstdarg>

#include "log.hpp"

/* Log handle */
Log *gpLog = NULL;


void Log::SetActive(void)
{
    /* Active log */
    gpLog = this;
}

void Log::StdOut(bool value)
{
    streambuf *stream = NULL;

    /* Select stream */
    //stream = (value) ? cout.rdbuf() : &stream;
    if (value) {
        stream = cout.rdbuf();
    }

    /* Set stream */
    rdbuf(stream);
}

void Log::Print(const char *format, ...)
{
    char   message[256];
    Uint32 size;

    va_list args;

    /* Get arguments */
    va_start(args, format);

    /* Generate message */
    size = vsprintf(message, format, args);

    /* Add message */
    mBuffer.sputn(message, size);

    /* Free arguments */
    va_end(args);
}

void Log::PrintOut(void)
{
    string str = mBuffer.str();

    /* Print log */
    cout << str;
}

