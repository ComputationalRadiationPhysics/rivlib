/*
 * rivlib API
 * version.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_VERSION_H_INCLUDED
#define VICCI_RIVLIB_VERSION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


//
// #include "the/stringize_macro.h"
//
// Redefinition of the macro, because we don't want the API to require TheLib
//
#ifndef THE_STRINGIZE_DEFINED
#define THE_STRINGIZE_DEFINED 1

#define THE_STRINGIZE_DO(arg) #arg
#define THE_STRINGIZE(arg) THE_STRINGIZE_DO(arg)

#endif /* THE_STRINGIZE_DEFINED */
//
// Redefinition end
//


#define RIVLIB_VERSION_MAJOR 1
#define RIVLIB_VERSION_MINOR 0
#define RIVLIB_VERSION_BUILD 0
#define RIVLIB_VERSION_REV   2

#define RIVLIB_VERSION RIVLIB_VERSION_MAJOR.RIVLIB_VERSION_MINOR.RIVLIB_VERSION_BUILD.RIVLIB_VERSION_REV
#define RIVLIB_VERSION_STRING THE_STRINGIZE(RIVLIB_VERSION)
#define RIVLIB_VERSION_COMMA_LIST RIVLIB_VERSION_MAJOR, RIVLIB_VERSION_MINOR, RIVLIB_VERSION_BUILD, RIVLIB_VERSION_REV


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_VERSION_H_INCLUDED */
