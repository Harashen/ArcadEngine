#ifndef _AI_H_
#define _AI_H_

#include "entity.h"
#include "point.h"


/*!
	CAi is the class that controls the artifical
	intelligence of the computer-controlled entities.
	\author Javier Bermejo García
*/
class CAi {
	/*!
		Make the entity to run in the appropiate direction.
		\param Entity the entity to process
	*/
	void Run(CEntity *Entity);

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
extern CAi *pAi;

#endif
