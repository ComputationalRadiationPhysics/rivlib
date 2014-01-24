/*
 * image_stream_connection.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "rivlib/image_stream_connection.h"
#include "api_impl/image_stream_connection_impl.h"

using namespace eu_vicci::rivlib;


/*
 * image_stream_connection::listener::listener
 */
image_stream_connection::listener::listener(void) {
    // intentionally empty
}


/*
 * image_stream_connection::listener::~listener
 */
image_stream_connection::listener::~listener(void) {
    // intentionally empty
}


/*
 * image_stream_connection::create
 */
image_stream_connection::ptr image_stream_connection::create(void) {
    return new image_stream_connection_impl();
}


/*
 * image_stream_connection::~image_stream_connection
 */
image_stream_connection::~image_stream_connection(void) {
    // intentionally empty
}


/*
 * image_stream_connection::image_stream_connection
 */
image_stream_connection::image_stream_connection(void) {
    // intentionally empty
}
