/*
 * rivlib API
 * common.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_COMMON_H_INCLUDED
#define VICCI_RIVLIB_COMMON_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "rivlib/version.h"

#define __STDC_LIMIT_MACROS
#include "rivlib/config.h"
#include <stdint.h>


/*
 * Defines controlling library symbol import/export
 */
#ifdef _WIN32
#   ifdef RIVLIB_EXPORTS
#       define RIVLIB_API __declspec(dllexport)
#       define RIVLIB_APIEXT
#   else
#       define RIVLIB_API __declspec(dllimport)
#       define RIVLIB_APIEXT extern
#   endif
#   define RIVLIB_CALL __stdcall
#else
#   define RIVLIB_API
#   ifdef RIVLIB_EXPORTS
#   	define RIVLIB_APIEXT
#   else
#   	define RIVLIB_APIEXT extern
#   endif
#   define RIVLIB_CALL
#endif


/**
 * Offset for network user messages
 *
 * Always use this offset to send user messages. Messages with smaller values
 * are not guaranteed to function properly, except for messages defined by
 * library classes.
 *
 * @example:
 *    control_connection::ptr cc;
 *    cc->send(RIVLIB_USERMSG + 42, 10, "Some data");
 */
#define RIVLIB_USERMSG 1000


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_COMMON_H_INCLUDED */
