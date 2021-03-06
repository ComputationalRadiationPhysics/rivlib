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
