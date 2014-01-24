/*
 * stdafx.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVPROVTEST_STDAFX_H_INCLUDED
#define VICCI_RIVPROVTEST_STDAFX_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#ifdef _WIN32

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <tchar.h>
#include <memory>

#else

#include <stdio.h>
#include <memory>
#include <stdint.h>

#endif

#ifndef byte
typedef uint8_t byte;
#endif

// reference additional headers your program requires here

#endif /* VICCI_RIVPROVTEST_STDAFX_H_INCLUDED */
