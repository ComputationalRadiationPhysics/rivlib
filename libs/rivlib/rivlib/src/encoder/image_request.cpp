/*
 * rivlib
 * encoder/image_request.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "image_request.h"

using namespace eu_vicci::rivlib;


/*
 * encoder::image_request::image_request
 */
encoder::image_request::image_request(output_callback func, void *ctxt, unsigned int last_time_id) 
        : func(func), ctxt(ctxt), last_time_id(last_time_id) {
    // intentionally empty
}


/*
 * encoder::image_request::~image_request
 */
encoder::image_request::~image_request(void) {
    this->reset();
}


/*
 * encoder::image_request::call
 */
void encoder::image_request::call(const data::buffer::shared_ptr data) {
    if (this->func != nullptr) {
        this->func(data, this->ctxt);
    }
}


/*
 * encoder::image_request::reset
 */
void encoder::image_request::reset(void) {
    this->func = nullptr;
    this->ctxt = nullptr;
    this->last_time_id = 0;
}
