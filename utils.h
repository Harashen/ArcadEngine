#ifndef _UTILS_H_
#define _UTILS_H_

#include <SDL/SDL.h>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;


/* Macros */
#define foreach(x, y) \
	for (y = (x).begin(); y != (x).end(); y++)


/* Functions */
inline Uint32 PowerOfTwo(Uint32 x)
{
	double logbase2 = log(x) / log(2);

	/* Return nearest power of two */
	return (Uint32) pow(2, ceil(logbase2));
}

inline void RemoveSpaces(string &str)
{
	string whitespaces(" \t\f\v\n\r");
	size_t pos;

	/* Find last non-whitespace */
	pos = str.find_last_not_of(whitespaces);

	/* Erase whitespaces */
	if (pos != string::npos)
		str.erase(pos + 1);
	else
		str.clear();

	/* Find first non-whitespace */
	pos = str.find_first_not_of(whitespaces);

	/* Erase whitespaces */
	if (pos != string::npos)
		str.erase(0, pos);
	else
		str.clear();
}

template <class T> 
inline void Flip(T &a, T &b)
{
	T temp = a;

	/* Flip values */
	a = b;
	b = temp;
}

template <class T>
inline T MaxAbs(T a, T b)
{
	/* Absolute values */
	a = abs(a);
	b = abs(b);

	/* Return max */
	return max<T>(a, b);
}

#endif
