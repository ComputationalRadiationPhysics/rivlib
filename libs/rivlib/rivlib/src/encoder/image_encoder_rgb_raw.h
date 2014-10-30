/*
 * rivlib
 * encoder/image_encoder_rgb_raw.h
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
     * The rgb_raw image encoder
     */
    class image_encoder_rgb_raw : public image_encoder_base {
    public:

        /** ctor */
        image_encoder_rgb_raw(void);

        /** dtor */
        virtual ~image_encoder_rgb_raw(void);

    protected:

        /**
         * Performs the actual encoding
         *
         * @param data The raw input data
         *
         * @return The encoded input data
         */
        virtual data::buffer::shared_ptr encode(data::buffer::shared_ptr data);

    };


} /* end namespace encoder */
} /* end namespace rivlib */
} /* end namespace eu_vicci */
