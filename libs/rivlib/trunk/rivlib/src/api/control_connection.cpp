/*
 * control_connection.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "rivlib/control_connection.h"
#include "api_impl/control_connection_impl.h"

using namespace eu_vicci::rivlib;


/*
 * control_connection::listener::listener
 */
control_connection::listener::listener(void) {
    // intentionally empty
}


/*
 * control_connection::listener::~listener
 */
control_connection::listener::~listener(void) {
    // intentionally empty
}


/*
 * control_connection::create
 */
control_connection::ptr control_connection::create(void) {
    return new control_connection_impl();
}


/*
 * control_connection::~control_connection
 */
control_connection::~control_connection(void) {
    // intentionally empty
}


/*
 * control_connection::control_connection
 */
control_connection::control_connection(void) {
    // intentionally empty
}
