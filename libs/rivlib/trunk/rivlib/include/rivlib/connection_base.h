/*
 * rivlib API
 * connection_base.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_CONNECTION_BASE_H_INCLUDED
#define VICCI_RIVLIB_CONNECTION_BASE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "rivlib/common.h"
#include "rivlib/api_ptr.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {


    /**
     * Base class for connections from a riv client to server
     *
     * @param T The final listener class
     */
    class RIVLIB_API connection_base {
    public:

        /** possible connection status values */
        enum class status {
            not_connected,
            connecting,
            connected,
            disconnecting
        };

        /**
         * Connects to a riv provider service
         *
         * @param uri The uri to connect to (using riv-schema)
         */
        virtual void connect(const char *uri) = 0;

        /**
         * Connects to a riv provider service
         *
         * @param uri The uri to connect to (using riv-schema)
         */
        virtual void connect(const wchar_t *uri) = 0;

        /**
         * Disconnects from the currently connected riv provider service
         *
         * @param wait If true, the method will return only after the
         *            connection has been closed
         */
        virtual void disconnect(bool wait = false) = 0;

        /**
         * Answer the connection status of this channel
         *
         * @return The connection status
         */
        virtual status get_status(void) const = 0;

        /**
         * Answer the uri of the connected riv provider service
         *
         * @return The uri of the connected riv provider service
         */
        virtual const char *get_uri_astr(void) const = 0;

        /**
         * Answer the uri of the connected riv provider service
         *
         * @return The uri of the connected riv provider service
         */
        virtual const wchar_t *get_uri_wstr(void) const = 0;

        /** Dtor */
        virtual ~connection_base(void);

    protected:

        /**
         * Base class for listener objects
         */
        class RIVLIB_API listener_base {
        protected:

            /** ctor */
            listener_base(void);

            /** dtor */
            virtual ~listener_base(void);

        };

        /**
         * Base class for typed listener objects
         *
         * @param T The class of the api_ptr of the owning connection
         */
        template<class T>
        class RIVLIB_API listener_typed_base : public listener_base {
        public:

            /**
             * Called when the connection has been connected to a
             * provider
             *
             * @param comm The calling connection
             */
            virtual void on_connected(T comm) throw() = 0;

            /**
             * Called when the connection has been disconnected from a
             * provider
             *
             * @param comm The calling connection
             */
            virtual void on_disconnected(T comm) throw() = 0;

            /**
             * Called when an error occured. If the error occured while
             * establishing a connection, the connection attempts are
             * canceled.
             *
             * @param comm The calling connection
             * @param msg The error message
             */
            virtual void on_error(T comm, const char *msg) throw() = 0;

        protected:

            /** ctor */
            listener_typed_base(void) : listener_base() {
                // intentionally empty
            }

            /** dtor */
            virtual ~listener_typed_base(void) {
                // intentionally empty
            }

        };

        /** Ctor */
        connection_base(void);

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
#endif /* VICCI_RIVLIB_CONNECTION_BASE_H_INCLUDED */
