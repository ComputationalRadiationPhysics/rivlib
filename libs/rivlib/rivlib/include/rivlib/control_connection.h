/*
 * rivlib API
 * control_connection.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_CONTROL_CONNECTION_H_INCLUDED
#define VICCI_RIVLIB_CONTROL_CONNECTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "rivlib/common.h"
#include "rivlib/api_ptr.h"
#include "rivlib/connection_base.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {

    /** forward declaration */
    class RIVLIB_API control_connection;
    
    /*
     * force generation of template class for dll interface
     * http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958
     */
    RIVLIB_APIEXT template class RIVLIB_API api_ptr<control_connection>;


    /**
     * The control connection channel to a rivlib server
     */
    class RIVLIB_API control_connection : public connection_base {
    public:

        /** The pointer type to be used by the applications */
        typedef api_ptr<control_connection> ptr;

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
             * Called when a (non-internal) message has been received from the
             * provider
             *
             * @param comm The calling control_connection
             * @param id The message id
             * @param size The message data size (in bytes)
             * @param data The message data
             */
            virtual void on_msg(control_connection::ptr comm, 
                unsigned int id, unsigned int size, const void *data) throw() = 0;

        };

        /**
         * Creates a new control_connection object
         *
         * @return A pointer to the new control_connection object
         */
        static control_connection::ptr create(void);

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
         * Sends a message to the connected riv provider service
         *
         * @param id The message id
         * @param size The message data size (in bytes)
         * @param data The message data
         */
        virtual void send(unsigned int id, unsigned int size, const void *data) = 0;

        /**
         * Constructs the uri to a data channel of the connected provider
         *
         * @param name The name of the data channel
         * @param type The type of the data channel
         * @param subtype The subtype of the data channel
         * @param uri Points to the memory to receive the constructed uri
         * @param uri_size The size of 'uri' in bytes
         *
         * @return If 'uri' is nullptr, returns the size required to store the
         *         constructred uri, excluding the terminating zero. If 'uri'
         *         is not nullptr, returns the number of bytes written.
         */
        virtual size_t make_data_channel_uri(const char *name, uint16_t type, uint16_t subtype, char *uri, size_t uri_size) = 0;

        /** Dtor */
        virtual ~control_connection(void);

    protected:

        /** Ctor */
        control_connection(void);

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
#endif /* VICCI_RIVLIB_CONTROL_CONNECTION_H_INCLUDED */
