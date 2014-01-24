/*
 * image_data_binding.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "rivlib/image_data_binding.h"
#include "the/not_supported_exception.h"

using namespace eu_vicci::rivlib;


/*
 * image_data_binding::image_data_binding
 */
image_data_binding::image_data_binding(unsigned int width,
        unsigned int height, image_colour_type col_type,
        image_data_type dat_type, image_orientation img_ori,
        unsigned int scan_width) : data_binding(), width(width),
        height(height), col_type(col_type), dat_type(dat_type),
        img_ori(img_ori), scan_width(scan_width) {
    // intentionally empty
}


/*
 * image_data_binding::image_data_binding
 */
image_data_binding::image_data_binding(void) {
    THROW_THE_NOT_SUPPORTED_EXCEPTION;
}


/*
 * image_data_binding::~image_data_binding
 */
image_data_binding::~image_data_binding(void) {
    // intentionally empty
}
