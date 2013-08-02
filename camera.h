#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "entity.h"

/*!
	CCamera is the class that controls the drawing perspective
	and the drawing position of the primitives.
	\author Javier Bermejo García
*/
class CCamera {
	CEntity *Target;	//!< The entity to be followed.

public:
	float PosX;			//!< Current X-axis position.
	float PosY;			//!< Current Y-axis position.

public:
	/*!
		CCamera constructor.
	*/
	CCamera(void);
	
	/*!
		CCamera destructor.
	*/
	~CCamera (void);

	/*!
		Set the active camera object.
	*/
	void SetActive(void);

	/*!
		Set the target to follow.
		\param Entity the target entity.
	*/
	void SetTarget(CEntity *Entity);

	/*!
		Update camera position.
	*/
	void Look(void);
};

//! Active camera object
extern CCamera *pCamera;

#endif
