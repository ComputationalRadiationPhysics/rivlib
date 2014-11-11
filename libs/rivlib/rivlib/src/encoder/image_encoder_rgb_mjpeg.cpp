/*
 * rivlib
 * encoder/image_encoder_rgb_mjpeg.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "encoder/image_encoder_rgb_mjpeg.h"
#include "data/buffer_type.h"
#include "data/image_buffer_metadata.h"
#include "the/text/string_builder.h"
#include <algorithm>
#include "vislib/types.h"
#define XMD_H
#include "jpeglib.h"
#include "vislib/String.h"

using namespace eu_vicci::rivlib;


namespace eu_vicci {
namespace rivlib {
namespace encoder {
namespace _internal {

    /**
     * Convert "exit" to "throw"
     */
    static void jpeg_error_exit_throw(j_common_ptr cinfo) {
        char buffer[JMSG_LENGTH_MAX];
        (*cinfo->err->format_message)(cinfo, buffer);
        vislib::StringA msg(buffer);
        ::jpeg_destroy(cinfo);
        throw vislib::Exception(msg, __FILE__, __LINE__);
    }

    /**
     * block any warning, trace, error of libJpeg
     */
    static void jpeg_output_message_no(j_common_ptr cinfo) {
        // be silent!
    }

} /* end namespace _internal */
} /* end namespace encoder */
} /* end namespace rivlib */
} /* end namespace eu_vicci */


/*
 * encoder::image_encoder_rgb_mjpeg::image_encoder_rgb_mjpeg
 */
encoder::image_encoder_rgb_mjpeg::image_encoder_rgb_mjpeg(void)
        : image_encoder_base(), quality(99) {
    // intentionally empty
}


/*
 * encoder::image_encoder_rgb_mjpeg::~image_encoder_rgb_mjpeg
 */
encoder::image_encoder_rgb_mjpeg::~image_encoder_rgb_mjpeg(void) {
    // intentionally empty
}


/*
 * encoder::image_encoder_rgb_mjpeg::decode
 */
data::buffer::shared_ptr encoder::image_encoder_rgb_mjpeg::decode(data::buffer::shared_ptr data) {
    if (data->type() != data::buffer_type::mjpeg_rgb_bytes) throw the::exception(__FILE__, __LINE__);
    data::buffer::shared_ptr o = data::buffer::create();

    o->set_time_code(data->time_code());
    o->set_type(data::buffer_type::raw_rgb_bytes);
    o->metadata() = data->metadata();

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = ::jpeg_std_error(&jerr);
    cinfo.err->error_exit = _internal::jpeg_error_exit_throw;
    cinfo.err->output_message = _internal::jpeg_output_message_no;
    ::jpeg_create_decompress(&cinfo);
    ::jpeg_mem_src(&cinfo, data->data().as<unsigned char>(),
        static_cast<unsigned long>(data->data().size()));


    if (::jpeg_read_header(&cinfo, TRUE) != 1) {
        ::jpeg_destroy_decompress(&cinfo);
	throw vislib::Exception("jpeg decode header error", __FILE__, __LINE__);
    }

    /* set parameters for decompression */
    /* We don't need to change any of the defaults set by
     * jpeg_read_header(), so we do nothing here.
     */

    if (!::jpeg_start_decompress(&cinfo)) {
        ::jpeg_destroy_decompress(&cinfo);
	throw vislib::Exception("jpeg decode error", __FILE__, __LINE__);
    }

    if (sizeof(JSAMPLE) != 1) { // only support 8-bit jpegs ATM
        ::jpeg_destroy_decompress(&cinfo);
	throw vislib::Exception("jpeg decode error", __FILE__, __LINE__);
    }
    if (cinfo.output_components < 1) {
        ::jpeg_destroy_decompress(&cinfo);
	throw vislib::Exception("jpeg decode error", __FILE__, __LINE__);
    }
    
    if (cinfo.out_color_space != JCS_RGB) {
        ::jpeg_destroy_decompress(&cinfo);
	throw vislib::Exception("jpeg decode out_color_space error", __FILE__, __LINE__);
    }
    
    unsigned int row_stride = cinfo.output_width * cinfo.output_components;
    o->data().enforce_size(cinfo.output_height * row_stride);
    while (cinfo.output_scanline < cinfo.output_height) {
        JSAMPLE *ptr = o->data().as_at<JSAMPLE>(row_stride * cinfo.output_scanline);
        ::jpeg_read_scanlines(&cinfo, &ptr, 1);
    }
    
    ::jpeg_finish_decompress(&cinfo);
    ::jpeg_destroy_decompress(&cinfo);

    return o;
}


/*
 * encoder::image_encoder_rgb_mjpeg::encode
 */
data::buffer::shared_ptr encoder::image_encoder_rgb_mjpeg::encode(data::buffer::shared_ptr data) {
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
    ASSERT(data->type() == data::buffer_type::raw_rgb_bytes);

    o->set_time_code(data->time_code());
    o->set_type(data::buffer_type::mjpeg_rgb_bytes);
    o->metadata() = data->metadata();

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    cinfo.err->error_exit = _internal::jpeg_error_exit_throw;
    cinfo.err->output_message = _internal::jpeg_output_message_no;
    ::jpeg_create_compress(&cinfo);

    unsigned char *buf = NULL;
    unsigned long bufSize = 0;
    ::jpeg_mem_dest(&cinfo, &buf, &bufSize);

    data::image_buffer_metadata *ibm = data->metadata().as<data::image_buffer_metadata>();

    cinfo.image_width = ibm->width;
    cinfo.image_height = ibm->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    ::jpeg_set_defaults(&cinfo);
    ::jpeg_set_quality(&cinfo, this->quality, TRUE);

    ::jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW rowPointer[1];
    int rowStride = ibm->width * 3;
    rowPointer[0] = const_cast<JSAMPLE*>(data->data().as<JSAMPLE>());

    for (unsigned int y = 0; y < ibm->height; y++, rowPointer[0] += rowStride) {
        ::jpeg_write_scanlines(&cinfo, rowPointer, 1);
    }

    ::jpeg_finish_compress(&cinfo);
    ::jpeg_destroy_compress(&cinfo);

    o->data().enforce_size(bufSize);
    ::memcpy(o->data(), buf, bufSize);

    ::free(buf);

    return o;
}
