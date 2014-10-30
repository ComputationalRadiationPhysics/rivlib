/*
 * rivlib
 * data/buffer_type.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#pragma once

namespace eu_vicci {
namespace rivlib {
namespace data {


    /**
     * struct storing image buffer metadata
     *
     * @remarks
     *  The data is always bytes!
     *  The image orientation is Top-Down.
     */
    typedef struct _image_buffer_metadata_t {

        /** The width in pixel */
        unsigned int width;

        /** The height in pixel */
        unsigned int height;

    } image_buffer_metadata;


} /* end namespace data */
} /* end namespace rivlib */
} /* end namespace eu_vicci */
