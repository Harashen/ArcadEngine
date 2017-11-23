#ifndef _AI_HPP_
#define _AI_HPP_

#include "entity.hpp"
#include "point.hpp"


/*!
    Ai is the class that controls the artifical
    intelligence of the computer-controlled entities.
    \author Javier Bermejo García
*/
class Ai {
    /*!
        Make the entity to run in the appropiate direction.
        \param Entity the entity to process
    */
    void Run(Entity *entity);

public:
    /*!
        Set the active Ai object.
    */
    void SetActive(void);

    /*!
        Update all the computer-controlled entities.
    */
    void Update(void);
};

//! Pointer to the current active Ai object.
extern Ai *pAi;

#endif
