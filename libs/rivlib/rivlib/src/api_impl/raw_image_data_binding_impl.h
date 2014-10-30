/*
 * rivlib API
 * raw_image_data_binding_impl.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_RAW_IMAGE_DATA_BINDING_IMPL_H_INCLUDED
#define VICCI_RIVLIB_RAW_IMAGE_DATA_BINDING_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */


#include "rivlib/raw_image_data_binding.h"
#include "element_node.h"
#include "data/buffer.h"


namespace eu_vicci {
namespace rivlib {

    /**
     * Implementation of the core class
     */
    class raw_image_data_binding_impl : public raw_image_data_binding, public element_node {
    public:

        /**
         * ctor
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
        raw_image_data_binding_impl(const void *data, unsigned int width,
            unsigned int height, image_colour_type col_type,
            image_data_type dat_type, image_orientation img_ori,
            unsigned int scan_width);

        /** dtor */
        virtual ~raw_image_data_binding_impl(void);

        /**
         * Informs the data_binding that new data is now available.
         * The provider object will start reading this data asynchronously,
         * if possible.
         */
        virtual void async_data_available(void);

        /**
         * Answer whether or not asynchronous reading operations are performed
         * on the current image data.
         */
        virtual bool is_async_operation_running(void);

        /**
         * Blocks the calling thread until the asynchronous read operation on
         * the current data has been completed (if any is currently
         * performed).
         */
        virtual void wait_async_data_completed(void);

        /**
         * Aborts any asynchronous reading operations on the data. This
         * methode blocks until all such operations are aborted.
         */
        virtual void wait_async_data_abort(void);

        /**
         * Answer the data buffer with a specified offset (in bytes) and in a
         * specified pointer type
         *
         * @param Tp The type to be returned
         * @param offset The offset in byte
         *
         * @return The typed and offsetted pointer
         */
        template<class Tp>
        inline const Tp *as_at(size_t offset) {
            return reinterpret_cast<const Tp*>(
                static_cast<const char*>(this->data_ptr) + offset);
        }

    protected:

    private:

        /** The input data buffer pointer */
        const void *data_ptr;

        /** The raw image data buffer */
        data::buffer::shared_ptr raw_buffer;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_RAW_IMAGE_DATA_BINDING_IMPL_H_INCLUDED */
