#ifndef _TIMER_HPP_
#define _TIMER_HPP_

#include <SDL/SDL.h>

//! Timer constants.
const Uint64 MSEC_PER_SEC = 1000ULL;
const Uint64 USEC_PER_SEC = 1000000ULL;
const Uint64 NSEC_PER_SEC = 1000000000ULL;


/*!
    Timer provides a set of tools to work with timers
    using different time units (milliseconds, microseconds...).
    There is two timers, depending if the system
    has POSIX (real time clock) or not, in case of not having
    it makes use of the SDL options.
    \author Javier Bermejo García
*/
class Timer {
public:
    /*!
        Get the current time.
        \return the current time in nanoseconds.
    */
    static Uint64 GetTime(void);

    /*!
        Second sleep.
        \param Time the sleep value in seconds.
    */
    static void Sleep(Uint64 Time);

    /*!
        Millisecond sleep.
        \param Time the sleep value in milliseconds.
    */
    static void MSleep(Uint64 Time);

    /*!
        Microsecond sleep.
        \param Time the sleep value in microseconds.
    */
    static void USleep(Uint64 Time);

    /*!
        Nanosecond sleep.
        \param Time the sleep value in nanoseconds.
    */
    static void NSleep(Uint64 Time);
};

#endif
