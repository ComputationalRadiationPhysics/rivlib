/*
 * rivlib.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "the/types.h"
#include "rivlib/rivlib.h"


#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)
#pragma comment( lib, "jvm.lib" )
#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */

// Examples from the microsoft wizard:

//
//// Header:
////#####################################
//
//// This class is exported from the rivlib.dll
//class RIVLIB_API Crivlib {
//public:
//	Crivlib(void);
//};
//
//extern RIVLIB_API int nrivlib;
//
//RIVLIB_API int RIVLIB_CALL fnrivlib(void);
//
////#####################################
//
//// Source:
////#####################################
//
//// This is an example of an exported variable
//RIVLIB_API int nrivlib=0;
//
//// This is an example of an exported function.
//RIVLIB_API int RIVLIB_CALL fnrivlib(void) {
//    return 42;
//}
//
//// This is the constructor of a class that has been exported.
//// see rivlib.h for the class definition
//Crivlib::Crivlib() {
//    return;
//}
//
////#####################################
//
