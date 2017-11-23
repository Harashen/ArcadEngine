#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "entity.hpp"

/*!
    Camera is the class that controls the drawing perspective
    and the drawing position of the primitives.
    \author Javier Bermejo García
*/
class Camera {
    Entity *mpTarget;	//!< The entity to be followed.

public:
    float mPosX;		//!< Current X-axis position.
    float mPosY;		//!< Current Y-axis position.

public:
    /*!
        Camera constructor.
    */
    Camera(void);
    
    /*!
        Camera destructor.
    */
    ~Camera (void);

    /*!
        Set the active camera object.
    */
    void SetActive(void);

    /*!
        Set the target to follow.
        \param Entity the target entity.
    */
    void SetTarget(Entity *entity);

    /*!
        Update camera position.
    */
    void Look(void);
};

//! Active camera object
extern Camera *gpCamera;

#endif
