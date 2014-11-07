/*
 * rivlib
 * encoder/image_encoder_rgb_mjpeg.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#pragma once
#include "encoder/image_encoder_base.h"
#include "data/buffer.h"


namespace eu_vicci {
namespace rivlib {
namespace encoder {


    /**
     * The rgb_mjpeg image encoder
     */
    class image_encoder_rgb_mjpeg : public image_encoder_base {
    public:

        /** ctor */
        image_encoder_rgb_mjpeg(void);

        /** dtor */
        virtual ~image_encoder_rgb_mjpeg(void);

        /**
         * Performs data decoding to rgb_raw
         *
         * @param data The encoded input data
         *
         * @return The raw_rgb output data
         */
        data::buffer::shared_ptr decode(data::buffer::shared_ptr data);

        /**
         * Answer the compression quality setting, that will be used when
         * encoding the image.
         *
         * @return The compression quality setting [0..100]
         */
        inline unsigned int CompressionQuality(void) const {
            return this->quality;
        }

        /**
         * Sets the compression quality. Values will be clamped to [0..100].
         * Larger values result in higher quality and large file sizes.
         *
         * @param q The new value for the compression quality
         */
        inline void SetCompressionQuality(unsigned int q) {
            this->quality = (q < 100) ? q : 100;
        }

    protected:

        /**
         * Performs the actual encoding
         *
         * @param data The raw input data
         *
         * @return The encoded data
         */
        virtual data::buffer::shared_ptr encode(data::buffer::shared_ptr data);

    private:

        /** The compression quality setting [0..100] */
        unsigned int quality;

    };


} /* end namespace encoder */
} /* end namespace rivlib */
} /* end namespace eu_vicci */
