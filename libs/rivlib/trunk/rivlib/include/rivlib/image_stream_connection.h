/*
 * rivlib API
 * image_stream_connection.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_IMAGE_STREAM_CONNECTION_H_INCLUDED
#define VICCI_RIVLIB_IMAGE_STREAM_CONNECTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "rivlib/common.h"
#include "rivlib/api_ptr.h"
#include "rivlib/connection_base.h"
#include "rivlib/ip_utilities.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {

    /** forward declaration */
    class RIVLIB_API image_stream_connection;
    
    /*
     * force generation of template class for dll interface
     * http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958
     */
    RIVLIB_APIEXT template class RIVLIB_API api_ptr<image_stream_connection>;


    /**
     * The control connection channel to a rivlib server
     */
    class RIVLIB_API image_stream_connection : public connection_base {
    public:

        /** The pointer type to be used by the applications */
        typedef api_ptr<image_stream_connection> ptr;

        /**
         * Base class for listener objects
         */
        class RIVLIB_API listener : public connection_base::listener_typed_base<ptr> {
        public:

            /** ctor */
            listener(void);

            /** dtor */
            virtual ~listener(void);

            /**
             * Called upon new incoming image data (uncompressed rgb, no padding!)
             *
             * @param comm The calling object
             * @param width The width of the image in pixel
             * @param height The height of the image in pixel
             * @param rgbpix The uncompressed rgb pixel data
             */
            virtual void on_image_data(ptr comm, uint32_t width, uint32_t height, const void* rgbpix) throw() = 0;

        };

        /**
         * Creates a new control_connection object
         *
         * @return A pointer to the new control_connection object
         */
        static image_stream_connection::ptr create(void);

        /**
         * Registers a listener at this object. The memory ownership of the
         * listener object is not taken by this control_connection object,
         * i.e. the caller must ensure that the provided pointer to the
         * listener remains valid as long as it is registered with this
         * control_connection object.
         *
         * @param l The listener to be registered.
         */
        virtual void add_listener(listener* l) = 0;

        /**
         * Removes a listener from this object.
         *
         * @param l The listener to be removed
         */
        virtual void remove_listener(listener* l) = 0;

        /**
         * Removes all listeners from this object
         */
        virtual void remove_all_listeners(void) = 0;

        /**
         * Answer whether a 'data_channel_type::image_stream' data channel of
         * the specified subtype is supported
         *
         * @param subtype The subtype to test
         *
         * @return True if supported
         */
        virtual bool is_supported(data_channel_image_stream_subtype subtype) = 0;

        /** Dtor */
        virtual ~image_stream_connection(void);

    protected:

        /** Ctor */
        image_stream_connection(void);

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
#endif /* VICCI_RIVLIB_IMAGE_STREAM_CONNECTION_H_INCLUDED */
