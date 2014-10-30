/*
 * rivlib API
 * vicci_middleware_broker.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_VICCI_MIDDLEWARE_BROKER_H_INCLUDED
#define VICCI_RIVLIB_VICCI_MIDDLEWARE_BROKER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "rivlib/common.h"

#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)

#include "rivlib/broker.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {


    /**
     * The broker connections to the vicci middleware using JNI and the
     * 'vicci semiwa lib'
     *
     * enable/disable this broker by adding/removing it to/from a core object
     *
     * Do not forget to set the path variables to find the java vm libraries
     * at runtime:
     *
     * e.g.
     *     win32 (on win64)
     * C:\Program Files (x86)\Java\jre7\bin\client
     *     win64
     * C:\Program Files\Java\jre7\bin\server
     *
     */
    class RIVLIB_API vicci_middleware_broker : public broker {
    public:

        /**
         * The vicci project default host address
         *
         * value: 192.168.1.100
         */
        static const char *DEFAULT_VICCI_HOST;

        /**
         * The vicci project default host port
         *
         * value: 5222
         */
        static const unsigned short DEFAULT_VICCI_HOST_PORT;

        /**
         * Creates a new simple console broker
         *
         * @return A new simple console broker
         */
        static broker::ptr create(void);

        /** dtor */
        virtual ~vicci_middleware_broker(void);

        /**
         * Clears all entries from the java class path setting. The broker
         * needs to be restarted (disable & enable) in order for the setting
         * to take effect.
         **/
        virtual void clear_java_class_paths(void) = 0;

        /**
         * Adds a new path to the java class path setting. The path can
         * specify a file system glob pattern, e.g. to add several jar files
         * at once. However, the call will never recursively enter or add
         * subdirectories. The broker needs to be restarted (disabled &
         * enabled) in order for the setting to take effect.
         *
         * @param path The new path to be added to the java class path setting
         */
        virtual void add_java_class_path(const char *path) = 0;

        /**
         * Adds a new path to the java class path setting. The path can
         * specify a file system glob pattern, e.g. to add several jar files
         * at once. However, the call will never recursively enter or add
         * subdirectories. The broker needs to be restarted (disabled &
         * enabled) in order for the setting to take effect.
         *
         * @param path The new path to be added to the java class path setting
         */
        virtual void add_java_class_path(const wchar_t *path) = 0;

        /**
         * Sets the vicci semiwa host
         *
         * @param host The name or ip address of the semiwa host machine
         * @param port The port of the semiwa server
         */
        virtual void set_vicci_semiwa_host(const char *host = DEFAULT_VICCI_HOST,
            unsigned short port = DEFAULT_VICCI_HOST_PORT) = 0;

    protected:

        /** ctor */
        vicci_middleware_broker(void);

    private:

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#else /* __cplusplus */

#error C API not implemented yet

#endif /* __cplusplus */

#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_VICCI_MIDDLEWARE_BROKER_H_INCLUDED */
