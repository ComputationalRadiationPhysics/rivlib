/*
 * raw_image_data_binding_impl.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "api_impl/raw_image_data_binding_impl.h"
#include "encoder/image_encoder_base.h"
#include "the/assert.h"

using namespace eu_vicci::rivlib;


/*
 * raw_image_data_binding_impl::raw_image_data_binding_impl
 */
raw_image_data_binding_impl::raw_image_data_binding_impl(const void *data,
        unsigned int width, unsigned int height, image_colour_type col_type,
        image_data_type dat_type, image_orientation img_ori,
        unsigned int scan_width)
        : raw_image_data_binding(width, height, col_type, dat_type, img_ori, scan_width),
        element_node(), data_ptr(data), raw_buffer() {
    THE_ASSERT(this->data_ptr != nullptr);
    // intentionally empty
}


/*
 * raw_image_data_binding_impl::~raw_image_data_binding_impl
 */
raw_image_data_binding_impl::~raw_image_data_binding_impl(void) {
    this->disconnect_all();
    this->raw_buffer.reset();
    this->data_ptr = nullptr; // DO NOT DELETE
}


/*
 * raw_image_data_binding_impl::async_data_available
 */
void raw_image_data_binding_impl::async_data_available(void) {
    std::vector<api_ptr_base> consumers = this->select<encoder::image_encoder_base>();
    for (size_t i = 0, cnt = consumers.size(); i < cnt; ++i) {
        encoder::image_encoder_base* encoder = dynamic_cast<encoder::image_encoder_base*>(consumers[i].get());
        encoder->start_new_input_encoding();
    }
}


/*
 * raw_image_data_binding_impl::is_async_operation_running
 */
bool raw_image_data_binding_impl::is_async_operation_running(void) {
    std::vector<api_ptr_base> consumers = this->select<encoder::image_encoder_base>();
    for (size_t i = 0, cnt = consumers.size(); i < cnt; ++i) {
        encoder::image_encoder_base* encoder = dynamic_cast<encoder::image_encoder_base*>(consumers[i].get());
        if (encoder->is_input_encoding_running()) return true;
    }
    return false;
}


/*
 * raw_image_data_binding_impl::wait_async_data_completed
 */
void raw_image_data_binding_impl::wait_async_data_completed(void) {
    std::vector<api_ptr_base> consumers = this->select<encoder::image_encoder_base>();
    for (size_t i = 0, cnt = consumers.size(); i < cnt; ++i) {
        encoder::image_encoder_base* encoder = dynamic_cast<encoder::image_encoder_base*>(consumers[i].get());
        encoder->wait_input_encoding(false);
    }
}


/*
 * raw_image_data_binding_impl::wait_async_data_abort
 */
void raw_image_data_binding_impl::wait_async_data_abort(void) {
    std::vector<api_ptr_base> consumers = this->select<encoder::image_encoder_base>();
    for (size_t i = 0, cnt = consumers.size(); i < cnt; ++i) {
        encoder::image_encoder_base* encoder = dynamic_cast<encoder::image_encoder_base*>(consumers[i].get());
        encoder->wait_input_encoding(true);
    }
}
