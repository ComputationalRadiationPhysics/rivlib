/*
 * simple_console_broker.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "rivlib/simple_console_broker.h"
#include "api_impl/simple_console_broker_impl.h"

using namespace eu_vicci::rivlib;


/*
 * simple_console_broker::simple_console_broker
 */
broker::ptr simple_console_broker::create(void) {
    return new simple_console_broker_impl();
}


/*
 * simple_console_broker::simple_console_broker
 */
simple_console_broker::simple_console_broker(void) {
    // intentionally empty
}


/*
 * simple_console_broker::~simple_console_broker
 */
simple_console_broker::~simple_console_broker(void) {
    // intentionally empty
}
