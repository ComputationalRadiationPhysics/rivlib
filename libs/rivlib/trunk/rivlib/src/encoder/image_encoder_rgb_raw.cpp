/*
 * rivlib
 * encoder/image_encoder_rgb_raw.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "encoder/image_encoder_rgb_raw.h"
#include "data/buffer_type.h"
#include "data/image_buffer_metadata.h"
#include <algorithm>

using namespace eu_vicci::rivlib;


/*
 * encoder::image_encoder_rgb_raw::image_encoder_rgb_raw
 */
encoder::image_encoder_rgb_raw::image_encoder_rgb_raw(void) : image_encoder_base() {
    // intentionally empty
}


/*
 * encoder::image_encoder_rgb_raw::~image_encoder_rgb_raw
 */
encoder::image_encoder_rgb_raw::~image_encoder_rgb_raw(void) {
    // intentionally empty
}


/*
 * encoder::image_encoder_rgb_raw::encode
 */
data::buffer::shared_ptr encoder::image_encoder_rgb_raw::encode(data::buffer::shared_ptr data) {
    if (data == nullptr) return nullptr;

    // lol rofl mao ...
    if (data->type() == data::buffer_type::raw_bgr_bytes) {
        data::image_buffer_metadata *ibm = data->metadata().as<data::image_buffer_metadata>();
        unsigned int cnt = ibm->width * ibm->height;
        unsigned char *d = data->data().as<unsigned char>();
        for (unsigned int i = 0; i < cnt; i++, d += 3) {
            std::swap(d[0], d[2]);
            // we should check this, but this is fast enough
            //if (this->encoder_should_terminate()) return nullptr;
        }
        data->set_type(data::buffer_type::raw_rgb_bytes);
    }

    return data;
}
