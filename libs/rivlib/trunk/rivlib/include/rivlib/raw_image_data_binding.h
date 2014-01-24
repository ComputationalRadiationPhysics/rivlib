/*
 * rivlib API
 * raw_image_data_binding.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_RAW_IMAGE_DATA_BINDING_H_INCLUDED
#define VICCI_RIVLIB_RAW_IMAGE_DATA_BINDING_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "rivlib/common.h"
#include "rivlib/image_data_binding.h"
#include "rivlib/image_data_types.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {


    /**
     * The class for data connections to raw image data
     */
    class RIVLIB_API raw_image_data_binding : public image_data_binding {
    public:

        /**
         * Creates a data binding to raw image data
         *
         * @param data The data to the raw image data
         * @param width The width of the image data in pixel
         * @param height The height of the image data in pixel
         * @param col_type The image type
         * @param dat_type The image data type
         * @param img_ori The image orientation
         * @param scan_width The size of one complete scan lines in bytes.
         *            The minimum value is "width x number of colour channels"
         *            and all specified values smaller than this are
         *            internally clamped to this minimum value.
         */
        static data_binding::ptr create(const void *data, unsigned int width,
            unsigned int height, image_colour_type col_type,
            image_data_type dat_type,
            image_orientation img_ori = image_orientation::top_down,
            unsigned int scan_width = 0);

        /** dtor */
        virtual ~raw_image_data_binding(void);

    protected:

        /**
         * ctor
         *
         * @param width The width of the image data in pixel
         * @param height The height of the image data in pixel
         * @param col_type The image type
         * @param dat_type The image data type
         * @param img_ori The image orientation
         * @param scan_width The size of one complete scan lines in bytes.
         *            The minimum value is "width x number of colour channels"
         *            and all specified values smaller than this are
         *            internally clamped to this minimum value.
         */
        raw_image_data_binding(unsigned int width,
            unsigned int height, image_colour_type col_type,
            image_data_type dat_type, image_orientation img_ori,
            unsigned int scan_width);

    private:

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#else /* __cplusplus */

#error C API not implemented yet

#endif /* __cplusplus */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_RAW_IMAGE_DATA_BINDING_H_INCLUDED */
