/*
 * vicci_middleware_broker.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "rivlib/vicci_middleware_broker.h"

#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)

#include "api_impl/vicci_middleware_broker_impl.h"

using namespace eu_vicci::rivlib;


/*
 * vicci_middleware_broker::DEFAULT_VICCI_HOST
 */
const char *vicci_middleware_broker::DEFAULT_VICCI_HOST = "192.168.1.100";


/*
 * vicci_middleware_broker::DEFAULT_VICCI_HOST_PORT
 */
const unsigned short vicci_middleware_broker::DEFAULT_VICCI_HOST_PORT = 5222;


/*
 * vicci_middleware_broker::create
 */
broker::ptr vicci_middleware_broker::create(void) {
    // intentionally empty
    return new vicci_middleware_broker_impl();
}

/*
 * vicci_middleware_broker::~vicci_middleware_broker
 */
vicci_middleware_broker::~vicci_middleware_broker(void) {
    // intentionally empty
}


/*
 * vicci_middleware_broker::vicci_middleware_broker
 */
vicci_middleware_broker::vicci_middleware_broker(void) : broker() {
    // intentionally empty
}

#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */
