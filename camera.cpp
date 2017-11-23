#include <cmath>

#include <GL/gl.h>

#include "camera.hpp"
#include "level.hpp"
#include "video.hpp"

/* Camera handler */
Camera *gpCamera = NULL;


Camera::Camera(void)
{
    /* Initialize variables */
    mpTarget = NULL;

    /* Default position */
    mPosX = 0.0;
    mPosY = 0.0;
}

Camera::~Camera(void)
{
    if (mpTarget) delete (mpTarget);
}

void Camera::SetActive(void)
{
    /* Set active camera */
    gpCamera = this;
}

void Camera::SetTarget(Entity *entity)
{
    /* Set camera target */
    mpTarget = entity;
}

void Camera::Look(void)
{
    Sint16 width  = gpLevel->GetWidth();
    Sint16 height = gpLevel->GetHeight();

    /* Follow target entity */
    if (mpTarget) {
        Rect rect = mpTarget->GetRect();
        float dX;
        float dY;

        /* Calculate position */
        dX = (rect.GetWidth()  - gpVideo->GetWidth())  * 0.5;
        dY = (rect.GetHeight() - gpVideo->GetHeight()) * 0.5;

        /* Set position */
        mPosX = rect.GetX() + dX;
        mPosY = rect.GetY() + dY;
    }

    /* Low limit */
    if (mPosX < 0) mPosX = 0;
    if (mPosY < 0) mPosY = 0;    

    /* High limit */
    if ((mPosX + gpVideo->GetWidth()) > width) {
        mPosX = width - gpVideo->GetWidth();
    }

    if ((mPosY + gpVideo->GetHeight()) > height) {
        mPosY = height - gpVideo->GetHeight();
    }

    /* Load identity */
    glLoadIdentity();

    /* Translate to camera position */
    glTranslatef(-mPosX, -mPosY, 0.0);
}
