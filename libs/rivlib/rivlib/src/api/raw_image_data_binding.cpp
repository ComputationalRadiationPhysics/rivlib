/*
 * raw_image_data_binding.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "rivlib/raw_image_data_binding.h"
#include "api_impl/raw_image_data_binding_impl.h"
#include "the/not_supported_exception.h"

using namespace eu_vicci::rivlib;


/*
 * raw_image_data_binding::create
 */
data_binding::ptr raw_image_data_binding::create(const void *data,
        unsigned int width, unsigned int height, image_colour_type col_type,
        image_data_type dat_type, image_orientation img_ori,
        unsigned int scan_width) {
    return new raw_image_data_binding_impl(
        data, width, height, col_type, dat_type, img_ori, scan_width);
}


/*
 * raw_image_data_binding::~raw_image_data_binding
 */
raw_image_data_binding::~raw_image_data_binding(void) {
    // intentionally empty
}


/*
 * raw_image_data_binding::raw_image_data_binding
 */
raw_image_data_binding::raw_image_data_binding(
        unsigned int width, unsigned int height, image_colour_type col_type,
        image_data_type dat_type, image_orientation img_ori,
        unsigned int scan_width) : image_data_binding(width, height,
            col_type, dat_type, img_ori, scan_width) {
    // intentionally empty
}
