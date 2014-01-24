/*
 * core.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "rivlib/core.h"
#include "api_impl/core_impl.h"

using namespace eu_vicci::rivlib;


/*
 * core::create_core
 */
core::ptr core::create(void) {
    return new core_impl();
}


/*
 * core::core
 */
core::core(void) {
    // intentionally empty
}


/*
 * core::~core
 */
core::~core(void) {
    // intentionally empty
}
