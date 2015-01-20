/*
 * rivlib
 * encoder/image_encoder_rgb_zip.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#pragma once
#include "encoder/image_encoder_base.h"
#include "data/buffer.h"
#include <string>
#include <vector>

extern "C" {
  #include <x264.h>
  #include <libswscale/swscale.h>
  #include <libavcodec/avcodec.h>
  #include <libavutil/avutil.h>
}


namespace eu_vicci {
namespace rivlib {
namespace encoder {


    /**
     * The rgb_zip image encoder
     */
    class image_encoder_rgb_zip : public image_encoder_base {
    public:

        /** ctor */
        image_encoder_rgb_zip(void);

        /** dtor */
        virtual ~image_encoder_rgb_zip(void);

        /**
         * Performs data decoding to rgb_raw
         *
         * @param data The encoded input data
         *
         * @return The raw_rgb output data
         */
        data::buffer::shared_ptr decode(data::buffer::shared_ptr data);

    private:

        int _frameHeight = -1;
        int _frameWidth = -1;

        // Decoder
        AVCodec* codec;
        AVCodecContext* codec_context;
        AVCodecParserContext* parser;
        AVFrame* frame;
        AVPacket pkt;
        std::vector<uint8_t> buffer;
        SwsContext* dec_convertCtx;

        // Encoder
        x264_t* encoder;
        x264_param_t param;
        x264_picture_t* picIn;
        x264_picture_t* picOut;
        x264_nal_t* headers;
        x264_nal_t* nals;
        int iNals;
        SwsContext* enc_convertCtx;

    protected:

        /**
         * Performs the actual encoding
         *
         * @param data The raw input data
         *
         * @return The encoded data
         */
        virtual data::buffer::shared_ptr encode(data::buffer::shared_ptr data);

    };


} /* end namespace encoder */
} /* end namespace rivlib */
} /* end namespace eu_vicci */
