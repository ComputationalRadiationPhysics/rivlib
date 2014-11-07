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
    void jpeg_error_exit_throw(j_common_ptr cinfo) {
        char buffer[JMSG_LENGTH_MAX];
        (*cinfo->err->format_message)(cinfo, buffer);
        vislib::StringA msg(buffer);
        ::jpeg_destroy(cinfo);
        throw vislib::Exception(msg, __FILE__, __LINE__);
    }

    /**
     * block any warning, trace, error of libJpeg
     */
    void jpeg_output_message_no(j_common_ptr cinfo) {
        // be silent!
    }

} /* end namespace _internal */
} /* end namespace encoder */
} /* end namespace rivlib */
} /* end namespace eu_vicci */


/*
 * encoder::image_encoder_rgb_mjpeg::image_encoder_rgb_mjpeg
 */
encoder::image_encoder_rgb_mjpeg::image_encoder_rgb_mjpeg(void) : image_encoder_base() {
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

#if 0
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = ::jpeg_std_error(&jerr);
    cinfo.err->error_exit = jpegutil::jpeg_error_exit_throw;
    cinfo.err->output_message = jpegutil::jpeg_output_message_no;
    ::jpeg_create_decompress(&cinfo);
    ::jpeg_mem_src(&cinfo,
        static_cast<unsigned char*>(const_cast<void*>(mem)),
        static_cast<unsigned long>(size));

    if (::jpeg_read_header(&cinfo, TRUE) != 1) {
        ::jpeg_destroy_decompress(&cinfo);
        return false;
    }

    /* set parameters for decompression */
    /* We don't need to change any of the defaults set by
     * jpeg_read_header(), so we do nothing here.
     */

    if (!::jpeg_start_decompress(&cinfo)) {
        ::jpeg_destroy_decompress(&cinfo);
        return false;
    }

    if (sizeof(JSAMPLE) != 1) { // only support 8-bit jpegs ATM
        ::jpeg_destroy_decompress(&cinfo);
        return false;
    }
    if (cinfo.output_components < 1) {
        ::jpeg_destroy_decompress(&cinfo);
        return false;
    }

    this->image().CreateImage(cinfo.output_width, cinfo.output_height, cinfo.output_components, BitmapImage::CHANNELTYPE_BYTE);
    switch (cinfo.out_color_space) {
        case JCS_UNKNOWN:
            for (int i = 0; i < cinfo.output_components; i++) {
                this->image().SetChannelLabel(static_cast<unsigned int>(i), BitmapImage::CHANNEL_UNDEF);
            }
            break;
        case JCS_GRAYSCALE:
            for (int i = 0; i < cinfo.output_components; i++) {
                this->image().SetChannelLabel(static_cast<unsigned int>(i), BitmapImage::CHANNEL_GRAY);
            }
            break;
        case JCS_RGB:
            if (this->image().GetChannelCount() == 3) {
                this->image().SetChannelLabel(0, BitmapImage::CHANNEL_RED);
                this->image().SetChannelLabel(1, BitmapImage::CHANNEL_GREEN);
                this->image().SetChannelLabel(2, BitmapImage::CHANNEL_BLUE);
            } else {
                for (int i = 0; i < cinfo.output_components; i++) {
                    this->image().SetChannelLabel(static_cast<unsigned int>(i), BitmapImage::CHANNEL_GRAY);
                }
            }
            break;
        case JCS_YCbCr: /* Y/Cb/Cr (also known as YUV) */
            // not supported
            for (int i = 0; i < cinfo.output_components; i++) {
                this->image().SetChannelLabel(static_cast<unsigned int>(i), BitmapImage::CHANNEL_UNDEF);
            }
            break;
        case JCS_CMYK:
            if (this->image().GetChannelCount() == 4) {
                this->image().SetChannelLabel(0, BitmapImage::CHANNEL_CYAN);
                this->image().SetChannelLabel(1, BitmapImage::CHANNEL_MAGENTA);
                this->image().SetChannelLabel(2, BitmapImage::CHANNEL_YELLOW);
                this->image().SetChannelLabel(3, BitmapImage::CHANNEL_BLACK);
            } else {
                for (int i = 0; i < cinfo.output_components; i++) {
                    this->image().SetChannelLabel(static_cast<unsigned int>(i), BitmapImage::CHANNEL_GRAY);
                }
            }
            break;
        case JCS_YCCK: /* Y/Cb/Cr/K */
            // not supported
            for (int i = 0; i < cinfo.output_components; i++) {
                this->image().SetChannelLabel(static_cast<unsigned int>(i), BitmapImage::CHANNEL_UNDEF);
            }
            break;
        default: // not supported for SURE
            for (int i = 0; i < cinfo.output_components; i++) {
                this->image().SetChannelLabel(static_cast<unsigned int>(i), BitmapImage::CHANNEL_UNDEF);
            }
            break;
    }

    unsigned int row_stride = cinfo.output_width * cinfo.output_components;
    while (cinfo.output_scanline < cinfo.output_height) {
        JSAMPLE *ptr = this->image().PeekDataAs<JSAMPLE>() + row_stride * cinfo.output_scanline;
        ::jpeg_read_scanlines(&cinfo, &ptr, 1);
    }

    if (cinfo.out_color_space == JCS_CMYK) {
        this->image().Invert();
    }

    ::jpeg_finish_decompress(&cinfo);
    ::jpeg_destroy_decompress(&cinfo);
#endif

    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;

    /*
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
    */
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

    o->set_time_code(data->time_code());
    o->set_type(data::buffer_type::mjpeg_rgb_bytes);
    o->metadata() = data->metadata();

#if 0

    using vislib::graphics::BitmapImage;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    cinfo.err->error_exit = jpegutil::jpeg_error_exit_throw;
    cinfo.err->output_message = jpegutil::jpeg_output_message_no;
    ::jpeg_create_compress(&cinfo);

    unsigned char *buf = NULL;
    unsigned long bufSize = 0;
    ::jpeg_mem_dest(&cinfo, &buf, &bufSize);

    const BitmapImage *src = &(this->image());
    vislib::SmartPtr<BitmapImage> alt;
    if (!src->EqualChannelLayout(BitmapImage::TemplateByteRGB)) {
        alt = new BitmapImage();
        alt->ConvertFrom(*src, BitmapImage::TemplateByteRGB);
        src = alt.operator->();
    }

    cinfo.image_width = src->Width();
    cinfo.image_height = src->Height();
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    ::jpeg_set_defaults(&cinfo);
    ::jpeg_set_quality(&cinfo, this->quality, TRUE);

    ::jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW rowPointer[1];
    int rowStride = src->Width() * 3;
    rowPointer[0] = const_cast<JSAMPLE*>(src->PeekDataAs<JSAMPLE>());

    for (unsigned int y = 0; y < src->Height(); y++, rowPointer[0] += rowStride) {
        ::jpeg_write_scanlines(&cinfo, rowPointer, 1);
    }

    ::jpeg_finish_compress(&cinfo);
    ::jpeg_destroy_compress(&cinfo);
    mem.EnforceSize(bufSize);
    ::memcpy(mem, buf, bufSize);
    ::free(buf);

    return true;

#endif

    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;

    /*
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
    */
}
