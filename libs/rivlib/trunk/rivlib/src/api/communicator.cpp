/*
 * communicator.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "rivlib/communicator.h"

using namespace eu_vicci::rivlib;


/*
 * communicator::communicator
 */
communicator::communicator(void) /*: core(nullptr)*/ {
    // intentionally empty
}


/*
 * communicator::~communicator
 */
communicator::~communicator(void) {
    //this->core = nullptr; // do not delete
}
