/*
 * glutInclude.visglut.h
 *
 * Copyright (C) 2007 - 2009 by VISUS (Universitaet Stuttgart).
 * Alle Rechte vorbehalten.
 *
 * Include this file for the visglut library.
 */

#ifndef VISLIBTEST_GLUTINCLUDE_VISGLUT_H_INCLUDED
#define VISLIBTEST_GLUTINCLUDE_VISGLUT_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "/home/schnei49/libs/visglut/include/visglut.h"

#ifdef _WIN32
#ifdef _MSC_VER
#ifdef _WIN64
#if defined _DEBUG || defined DEBUG 
#pragma comment(lib, "/home/schnei49/libs/visglut/lib/visglut64d.lib")
#else /* DEBUG */
#pragma comment(lib, "/home/schnei49/libs/visglut/lib/visglut64.lib")
#endif /* DEBUG */
#else /* _WIN64 */
#if defined _DEBUG || defined DEBUG 
#pragma comment(lib, "/home/schnei49/libs/visglut/lib/visglut32d.lib")
#else /* DEBUG */
#pragma comment(lib, "/home/schnei49/libs/visglut/lib/visglut32.lib")
#endif /* DEBUG */
#endif /* _WIN64 */
#pragma comment(linker, "/NODEFAULTLIB:freeglut_static.lib")
#endif /* _MSC_VER */
#endif /* _WIN32 */

#endif /* VISLIBTEST_GLUTINCLUDE_VISGLUT_H_INCLUDED */
