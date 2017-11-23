/*!
    \mainpage
    \version 1.0
    \author Javier Bermejo García

    ArcadEngine is a simple, powerful and multiplatform game engine
    designed to be able to create arcade games easily using only a collection
    of graphics, sounds and scripts.
*/

#include "engine.hpp"

int main(int argc, char **argv)
{
    Engine *mpEngine = new Engine;
    bool    exit;

    /* Initialize engine */
    exit = mpEngine->Init();
    if (!exit) {
        delete mpEngine;
        
        return -1;
    }

    /* Configure engine */
    exit = mpEngine->Configure();
    if (!exit) {
        delete mpEngine;
        
        return -2;
    }

    /* Run engine */
    mpEngine->Run();
    
    delete mpEngine;
    
    return 0;
}
