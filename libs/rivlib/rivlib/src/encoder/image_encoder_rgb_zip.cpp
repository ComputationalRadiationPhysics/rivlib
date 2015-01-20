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

    unsigned int frameWidth = data->metadata().as<data::image_buffer_metadata>()->width;
    unsigned int frameHeight = data->metadata().as<data::image_buffer_metadata>()->height;

    if(frameWidth!=_frameWidth || frameHeight!=_frameHeight)
    {
      _frameWidth = (int)frameWidth;
      _frameHeight = (int)frameHeight;

      avcodec_register_all();
      av_init_packet(&pkt);

      codec = avcodec_find_decoder(AV_CODEC_ID_H264);
      if(!codec) {
	  printf("Error: cannot find the h264 codec\n");
	  return false;
      }

      codec_context = avcodec_alloc_context3(codec);
      if(avcodec_open2(codec_context, codec, NULL) < 0) {
	  printf("Error: could not open codec.\n");
	  return false;
      }

      parser = av_parser_init(AV_CODEC_ID_H264);
      if(!parser) {
	printf("Erorr: cannot create H264 parser.\n");
	return false;
      }

      frame = avcodec_alloc_frame();
      std::copy((data->data().as<unsigned char>()),(data->data().as<unsigned char>()) + data->data().size() , std::back_inserter(buffer));
      dec_convertCtx = sws_getContext(frameWidth, frameHeight, AV_PIX_FMT_YUV420P, frameWidth, frameHeight, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
    }

    std::copy((data->data().as<unsigned char>()),(data->data().as<unsigned char>()) + data->data().size() , std::back_inserter(buffer));

    int len = 0;
    int dec_bytes = 0;
    int got_frame = 0;

    pkt.data = NULL;
    pkt.size = 0;

    len = av_parser_parse2(parser, codec_context, &pkt.data, &pkt.size, &buffer[0], buffer.size(), 0, 0, AV_NOPTS_VALUE);
    if(len && pkt.size )
	dec_bytes = avcodec_decode_video2(codec_context, frame, &got_frame, &pkt);

    if(got_frame&&dec_bytes){
	AVPicture picture;
	avpicture_alloc(&picture, AV_PIX_FMT_RGB24, frame->width, frame->height);

	int scale = sws_scale(dec_convertCtx, frame->data, frame->linesize, 0, frame->height, picture.data, picture.linesize);
	o->data().assert_size(frame->height*frame->width*3);
	memcpy((o->data().as<unsigned char>()),picture.data[0], frame->height*frame->width*3);
	buffer.erase(buffer.begin(), buffer.begin() + len);
	avpicture_free(&picture);
    }

    /*
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
    */

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
        }
        data->set_type(data::buffer_type::raw_rgb_bytes);
    }

    o->set_time_code(data->time_code());
    o->set_type(data::buffer_type::zip_rgb_bytes);
    o->metadata() = data->metadata();

    unsigned int frameWidth  = data->metadata().as<data::image_buffer_metadata>()->width;
    unsigned int frameHeight = data->metadata().as<data::image_buffer_metadata>()->height;

    // init encoder on first frame or when size changed
    if(frameWidth!=_frameWidth || frameHeight!=_frameHeight)
    {
      _frameWidth = (int)frameWidth;
      _frameHeight = (int)frameHeight;

      x264_param_default_preset(&param, "veryfast", "zerolatency");
      x264_param_apply_profile(&param, "baseline");

      param.i_threads = 1;
      param.i_width = frameWidth;
      param.i_height = frameHeight;
      param.i_keyint_max = 15;
      param.b_intra_refresh = 1;
      param.rc.i_rc_method = X264_RC_CRF;
      param.rc.f_rf_constant = 25;
      param.rc.f_rf_constant_max = 35;

      encoder = x264_encoder_open(&param);

      picIn = new x264_picture_t;
      picOut = new x264_picture_t;

      x264_picture_alloc(picIn, X264_CSP_I420, frameWidth, frameHeight);
      x264_encoder_parameters(encoder, &param);
      enc_convertCtx = sws_getContext(frameWidth, frameHeight, AV_PIX_FMT_RGB24, frameWidth, frameHeight, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);

      iNals = 0;
      nals = NULL;
    }

    uint8_t *inData[1] = {data->data().as<unsigned char>() };
    int inLinesize[1] = { 3*(int)frameWidth };

    sws_scale(enc_convertCtx, inData, inLinesize, 0, frameHeight, picIn->img.plane, picIn->img.i_stride);

    int enc_bytes = x264_encoder_encode(encoder, &nals, &iNals, picIn, picOut);

    if(enc_bytes){
	o->data().assert_size(enc_bytes);
	memcpy((o->data().as<unsigned char>()),nals[0].p_payload, enc_bytes);
    }

    /*
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
    */

    return o;
}
