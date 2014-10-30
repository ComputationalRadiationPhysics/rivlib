/*
 * rivlib
 * data/buffer.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "data/buffer.h"

using namespace eu_vicci;
using namespace eu_vicci::rivlib;


/*
 * data::buffer::create
 */
data::buffer::shared_ptr data::buffer::create(void) {
    // this could be changed to use a pool
    return shared_ptr(new data::buffer());
}


/*
 * data::buffer::~buffer
 */
data::buffer::~buffer(void) {
    // intentionally empty
}


/*
 * data::buffer::buffer
 */
data::buffer::buffer(void) : data_blob(), metadata_blob(), time_code_value(0), type_id_value(buffer_type::invalid) {
    // intentionally empty
}
