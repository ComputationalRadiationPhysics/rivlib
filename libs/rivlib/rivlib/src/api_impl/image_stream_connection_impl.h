/*
 * image_stream_connection_impl.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_IMAGE_STREAM_CONNECTION_IMPL_H_INCLUDED
#define VICCI_RIVLIB_IMAGE_STREAM_CONNECTION_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */


#include "rivlib/image_stream_connection.h"
#include "node.h"
#include "connection_base_impl.h"
#include "the/string.h"
#include "the/types.h"


namespace eu_vicci {
namespace rivlib {

    /**
     * Implementation of the image stream connection (client to server)
     */
    class image_stream_connection_impl : public image_stream_connection,
        public node {
    public:

        /** ctor */
        image_stream_connection_impl(void);

        /** dtor */
        virtual ~image_stream_connection_impl(void);

        /**
         * Registers a listener at this object. The memory ownership of the
         * listener object is not taken by this control_connection object,
         * i.e. the caller must ensure that the provided pointer to the
         * listener remains valid as long as it is registered with this
         * control_connection object.
         *
         * @param l The listener to be registered.
         */
        virtual void add_listener(listener* l);

        /**
         * Removes a listener from this object.
         *
         * @param l The listener to be removed
         */
        virtual void remove_listener(listener* l);

        /**
         * Removes all listeners from this object
         */
        virtual void remove_all_listeners(void);

        /**
         * Connects to a riv provider service
         *
         * @param uri The uri to connect to (using riv-schema)
         */
        virtual void connect(const char *uri);

        /**
         * Connects to a riv provider service
         *
         * @param uri The uri to connect to (using riv-schema)
         */
        virtual void connect(const wchar_t *uri);

        /**
         * Disconnects from the currently connected riv provider service
         *
         * @param wait If true, the method will return only after the
         *            connection has been closed
         */
        virtual void disconnect(bool wait = false);

        /**
         * Answer the connection status of this channel
         *
         * @return The connection status
         */
        virtual status get_status(void) const;

        /**
         * Answer the uri of the connected riv provider service
         *
         * @return The uri of the connected riv provider service
         */
        virtual const char *get_uri_astr(void) const;

        /**
         * Answer the uri of the connected riv provider service
         *
         * @return The uri of the connected riv provider service
         */
        virtual const wchar_t *get_uri_wstr(void) const;

        /**
         * Locks this node
         */
        virtual void lock(void);

        /**
         * Unlocks this node
         */
        virtual void unlock(void);

        /**
         * Answer the error log
         *
         * @return The error log
         */
        virtual error_log& log(void);

        /**
         * Answer whether a 'data_channel_type::image_stream' data channel of
         * the specified subtype is supported
         *
         * @param subtype The subtype to test
         *
         * @return True if supported
         */
        virtual bool is_supported(data_channel_image_stream_subtype subtype);

    private:

        /**
         * Implementation of logic
         */
        class self_impl : public connection_base_impl<image_stream_connection_impl> {
        public:

            /** ctor */
            self_impl(void);

            /** dtor */
            virtual ~self_impl(void);

            /**
             * Checks if the uri conforms to the specific channel type
             *
             * @param scheme The scheme
             * @param username The user name
             * @param host The host name or address
             * @param is_host_v6 True if the host address is IPv6
             * @param host_port The port
             * @param path The request path
             * @param query The request query
             * @param fragment The request fragment
             *
             * @throw the::exception if uri does not conform to the specific channel type
             */
            virtual void check_uri(std::string &scheme,
                std::string& username, std::string& host,
                bool &is_host_v6, unsigned short& host_port,
                std::string& path, std::string& query,
                std::string& fragment);

            /**
             * Evaluates the answer code and throws an exception if the connection
             * is not accepted.
             *
             * @param code The answer code sent from the server
             *
             * @throw the::exception if the connection is not accepted.
             */
            virtual void evaluate_answer(unsigned short code);

            /**
             * The implementation of the communication core functionality
             */
            virtual void communication_core(void);

        };

        /**
         * The implementation object
         * (composition pattern works better than multiple inheritance)
         */
        self_impl impl;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_IMAGE_STREAM_CONNECTION_IMPL_H_INCLUDED */
