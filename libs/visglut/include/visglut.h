/*
 * visglut.h
 *
 * Copyright (C) 2008 - 2010 by VISUS (Universitaet Stuttgart)
 * Alle Rechte vorbehalten.
 */

#ifndef VISGLUT_H_INCLUDED
#define VISGLUT_H_INCLUDED
#if (_MSC_VER > 1000)
#pragma once
#endif /* (_MSC_VER > 1000) */

#define FREEGLUT_STATIC 1
#define VISGLUT_EXTENSIONS 2

#include "GL/freeglut_std.h"
#include "GL/freeglut_ext.h"

#include "visglutversion.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * VIS-Stuff by S.Grottel
 */

/*
 * Some additional glutGet constants for maximized windows
 */
#ifdef _WIN32
#define  GLUT_WINDOW_RESTORE_X              0x0264
#define  GLUT_WINDOW_RESTORE_Y              0x0265
#define  GLUT_WINDOW_RESTORE_WIDTH          0x0266
#define  GLUT_WINDOW_RESTORE_HEIGHT         0x0267
#else /* _WIN32 */
#define  GLUT_WINDOW_RESTORE_X              GLUT_WINDOW_X
#define  GLUT_WINDOW_RESTORE_Y              GLUT_WINDOW_Y
#define  GLUT_WINDOW_RESTORE_WIDTH          GLUT_WINDOW_WIDTH
#define  GLUT_WINDOW_RESTORE_HEIGHT         GLUT_WINDOW_HEIGHT
#endif /* _WIN32 */

/**
 * Sets position of the window based on its client area.
 *
 * @param x The new x coordinate of the client area
 * @param y The new y coordinate of the client area
 */
FGAPI void FGAPIENTRY glutClientPositionWindow(int x, int y);

/**
 * Sets the icon of the current window.
 *
 * @param icon The string ressource identifier of the icon.
 */
FGAPI void FGAPIENTRY glutSetWindowIconA(char *icon);

/**
 * Sets the icon of the current window.
 *
 * @param icon The string ressource identifier of the icon.
 */
FGAPI void FGAPIENTRY glutSetWindowIconW(wchar_t *icon);

/**
 * Sets the icon of the current window.
 *
 * @param icon The integer ressource identifier of the icon.
 */
FGAPI void FGAPIENTRY glutSetWindowIconI(int icon);

/**
 * Adds a separating menu entry into the current menu.
 * Note this menu entry may create a callback event if clicked with item id 0.
 */
FGAPI void FGAPIENTRY glutAddMenuSeparator(void);

/**
 * Activates or deactivates the window decorations for the current window.
 *
 * @param show Zero to hide, non-zero to show window decorations
 */
FGAPI void FGAPIENTRY glutShowWindowDecorations(int show);

/**
 * Flags the window to stay on top or not.
 *
 * @param stay Non-zero window stay on top, zero window normally
 *             ordered in z-depth.
 */
FGAPI void FGAPIENTRY glutWindowStayOnTop(int stay);

#ifdef __cplusplus
}
#endif

#endif /* VISGLUT_H_INCLUDED */
