/*!
	\mainpage
	\version 1.0
	\author Javier Bermejo García

	ArcadEngine is a simple, powerful and multiplatform game engine
	designed to be able to create arcade games easily using only a collection
	of graphics, sounds and scripts.
*/

#include "engine.h"

int main(int argc, char **argv)
{
	CEngine *Engine = new CEngine;
	bool     exit;

	/* Initialize engine */
	exit = Engine->Init();
	if (!exit) {
		delete Engine;
		
		return -1;
	}

	/* Configure engine */
	exit = Engine->Configure();
	if (!exit) {
		delete Engine;
		
		return -2;
	}

	/* Run engine */
	Engine->Run();
	
	delete Engine;
	
	return 0;
}
