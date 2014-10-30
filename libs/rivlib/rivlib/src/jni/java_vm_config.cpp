/*
 * java_vm_config.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "jni/java_vm_config.h"

#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)

#include "the/assert.h"
#include "the/exception.h"

using namespace eu_vicci::rivlib;


/*
 * java_vm_config::java_vm_config
 */
java_vm_config::java_vm_config(void) : class_paths() {
    // intentionally empty
}


/*
 * java_vm_config::~java_vm_config
 */
java_vm_config::~java_vm_config(void) {
    // intentionally empty
}

#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */
