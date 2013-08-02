#include <cmath>
#include <GL/gl.h>
#include "camera.h"
#include "level.h"
#include "video.h"

/* Camera handler */
CCamera *pCamera = NULL;


CCamera::CCamera(void)
{
	/* Initialize variables */
	Target = NULL;

	/* Default position */
	PosX = 0.0;
	PosY = 0.0;
}

CCamera::~CCamera(void)
{
    if (Target)
        delete (Target);
}

void CCamera::SetActive(void)
{
	/* Set active camera */
	pCamera = this;
}

void CCamera::SetTarget(CEntity *Entity)
{
	/* Set camera target */
	Target = Entity;
}

void CCamera::Look(void)
{
	Sint16 width  = pLevel->GetWidth();
	Sint16 height = pLevel->GetHeight();

	/* Follow target entity */
	if (Target) {
		CRect tRect = Target->GetRect();
		float  dX, dY;

		/* Calculate position */
		dX   = (tRect.GetWidth()  - pVideo->GetWidth())  * 0.5;
		dY   = (tRect.GetHeight() - pVideo->GetHeight()) * 0.5;

		/* Set position */
		PosX = tRect.GetX() + dX;
		PosY = tRect.GetY() + dY;
	}

	/* Low limit */
	if (PosX < 0)
		PosX = 0;
	if (PosY < 0)
		PosY = 0;

	/* High limit */
	if ((PosX + pVideo->GetWidth()) > width)
		PosX = width - pVideo->GetWidth();
	if ((PosY + pVideo->GetHeight()) > height)
		PosY = height - pVideo->GetHeight();

	/* Load identity */
	glLoadIdentity();

	/* Translate to camera position */
	glTranslatef(-PosX, -PosY, 0.0);
}
