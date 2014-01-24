/*
 * rivlib API
 * image_data_binding.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_IMAGE_DATA_BINDING_H_INCLUDED
#define VICCI_RIVLIB_IMAGE_DATA_BINDING_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "rivlib/common.h"
#include "rivlib/data_binding.h"
#include "rivlib/image_data_types.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {


    /**
     * The class for data connections to raw image data
     */
    class RIVLIB_API image_data_binding : public data_binding {
    public:

        /**
         * Gets the width in pixel
         *
         * @return The width in pixel
         */
        inline unsigned int get_width(void) const {
            return this->width;
        }

        /**
         * Gets the height in pixel
         *
         * @return The height in pixel
         */
        inline unsigned int get_height(void) const {
            return this->height;
        }

        /**
         * Gets the colour type
         *
         * @return The colour type
         */
        inline image_colour_type get_colour_type(void) const {
            return this->col_type;
        }

        /**
         * Gets the data type
         *
         * @return The data type
         */
        inline image_data_type get_data_type(void) const {
            return this->dat_type;
        }

        /**
         * Gets the image orientation (y axis)
         *
         * @return The image orientation (y axis)
         */
        inline image_orientation get_orientation(void) const {
            return this->img_ori;
        }

        /**
         * Gets the byte size of one scan line
         *
         * @return The byte size of one scan line
         */
        inline unsigned int get_scan_width(void) const {
            return this->scan_width;
        }

        /** dtor */
        virtual ~image_data_binding(void);

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
        image_data_binding(unsigned int width,
            unsigned int height, image_colour_type col_type,
            image_data_type dat_type,
            image_orientation img_ori = image_orientation::top_down,
            unsigned int scan_width = 1);

        /** forbidden default ctor */
        image_data_binding(void);

        /**
         * The image data has been resized.
         *
         * @remarks It is not safe to call this method while asynchronous
         *          operations are running!
         *
         * @param width The width of the image data in pixel
         * @param height The height of the image data in pixel
         */
        inline void set_size(unsigned int width, unsigned int height) {
            this->width = width;
            this->height = height;
        }

    private:

        /** The width in pixel */
        unsigned int width;

        /** The height in pixel */
        unsigned int height;

        /** The colour type */
        image_colour_type col_type;

        /** The data type */
        image_data_type dat_type;

        /** The image orientation (y axis) */
        image_orientation img_ori;

        /** The byte size of one scan line */
        unsigned int scan_width;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#else /* __cplusplus */

#error C API not implemented yet

#endif /* __cplusplus */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_IMAGE_DATA_BINDING_H_INCLUDED */
