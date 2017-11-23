#include "timer.hpp"


void Timer::Sleep(Uint64 time)
{
    /* Sleep */
    MSleep(time * MSEC_PER_SEC);
}

void Timer::MSleep(Uint64 time)
{
    /* Sleep */
    USleep(time * MSEC_PER_SEC);
}

void Timer::USleep(Uint64 time)
{
    /* Sleep */
    NSleep(time * MSEC_PER_SEC);
}

#ifdef _POSIX_TIMERS

#include <sys/time.h>


Uint64 Timer::GetTime(void)
{
    struct timespec ts;

    /* Get time */
    clock_gettime(CLOCK_MONOTONIC, &ts);

    /* Return time */
    return (ts.tv_sec * NSEC_PER_SEC) + ts.tv_nsec;
}

void Timer::NSleep(Uint64 time)
{
    struct timespec ts;

    /* Set sleep time */
    ts.tv_sec  = time / NSEC_PER_SEC;
    ts.tv_nsec = time % NSEC_PER_SEC;

    /* Sleep */
    nanosleep(&ts, NULL);
}

#else

#include <SDL/SDL.h>


Uint64 Timer::GetTime(void)
{
    /* Return time */
    return (SDL_GetTicks() * USEC_PER_SEC);
}

void Timer::NSleep(Uint64 time)
{
    /* Sleep */
    SDL_Delay(time / USEC_PER_SEC);
}

#endif

