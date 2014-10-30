/*
 * rivlib
 * encoder/image_encoder_rgb_zip.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "encoder/image_encoder_rgb_zip.h"
#include "data/buffer_type.h"
#include "data/image_buffer_metadata.h"
#include "the/text/string_builder.h"
#include <algorithm>
#include "zlib.h"

using namespace eu_vicci::rivlib;


/*
 * encoder::image_encoder_rgb_zip::image_encoder_rgb_zip
 */
encoder::image_encoder_rgb_zip::image_encoder_rgb_zip(void) : image_encoder_base() {
    // intentionally empty
}


/*
 * encoder::image_encoder_rgb_zip::~image_encoder_rgb_zip
 */
encoder::image_encoder_rgb_zip::~image_encoder_rgb_zip(void) {
    // intentionally empty
}


/*
 * encoder::image_encoder_rgb_zip::decode
 */
data::buffer::shared_ptr encoder::image_encoder_rgb_zip::decode(data::buffer::shared_ptr data) {
    if (data->type() != data::buffer_type::zip_rgb_bytes) throw the::exception(__FILE__, __LINE__);
    data::buffer::shared_ptr o = data::buffer::create();

    o->set_time_code(data->time_code());
    o->set_type(data::buffer_type::raw_rgb_bytes);
    o->metadata() = data->metadata();

    unsigned int w = o->metadata().as<data::image_buffer_metadata>()->width;
    unsigned int h = o->metadata().as<data::image_buffer_metadata>()->height;

    // decompress using zlib's inflate
    int ret;
    z_stream strm;

    // allocate inflate state
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK) throw the::exception(__FILE__, __LINE__);

    strm.avail_in = data->data().size();
    strm.next_in = data->data().as<unsigned char>();

    o->data().assert_size(w * h * 3, true); // just to be sure

    size_t pos = 0;
    do {
        strm.avail_out = o->data().size() - pos;
        strm.next_out = o->data().as_at<unsigned char>(pos);
            
        ret = inflate(&strm, Z_NO_FLUSH);
        if ((ret == Z_STREAM_ERROR)
            || (ret == Z_NEED_DICT)
            || (ret == Z_DATA_ERROR)
            || (ret == Z_MEM_ERROR)) throw the::exception(the::text::astring_builder::format("zlib inflate error: %d", ret).c_str(), __FILE__, __LINE__);

        pos += (o->data().size() - pos) - strm.avail_out;
    } while (ret != Z_STREAM_END);

    // clean up and return
    inflateEnd(&strm);

    return o;
}


/*
 * encoder::image_encoder_rgb_zip::encode
 */
data::buffer::shared_ptr encoder::image_encoder_rgb_zip::encode(data::buffer::shared_ptr data) {
    if (data == nullptr) return nullptr;
    data::buffer::shared_ptr o = data::buffer::create();

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

    o->set_time_code(data->time_code());
    o->set_type(data::buffer_type::zip_rgb_bytes);
    o->metadata() = data->metadata();

    // compress data using zlib's deflate
    const int zlib_comp_level = Z_DEFAULT_COMPRESSION; // acceptable
    int ret;
    z_stream strm;

    // allocate deflate state
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, zlib_comp_level);
    if (ret != Z_OK) throw the::exception("failed zlib::deflateInit", __FILE__, __LINE__);

    strm.avail_in = data->data().size();
    strm.next_in = data->data().as<unsigned char>();

    o->data().assert_size(data->data().size());
    o->data().assert_size(256 * 1024);
    size_t pos = 0;

    // run deflate() on input until output buffer not full, finish
    //  compression if all of source has been read in
    do {
        if (pos >= o->data().size()) {
            o->data().assert_size(o->data().size() * 2, true);
        }

        strm.avail_out = o->data().size() - pos;
        strm.next_out = o->data().as_at<unsigned char>(pos);

        ret = deflate(&strm, Z_FINISH);     // no bad return value
        THE_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered
        pos += (o->data().size() - pos) - strm.avail_out;

    } while (ret != Z_STREAM_END);
    THE_ASSERT(strm.avail_in == 0);         // all input was be used

    deflateEnd(&strm);

    // TODO: This makes all the effort before obsolete.
    //  We will change this on store an explicit "valid_size" for the data block as a whole when refactoring the buffer class.
    o->data().enforce_size(pos, true);

    return o;
}
