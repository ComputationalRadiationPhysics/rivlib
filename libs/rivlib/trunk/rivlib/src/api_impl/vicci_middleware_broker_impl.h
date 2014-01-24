/*
 * vicci_middleware_broker_impl.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_VICCI_MIDDLEWARE_BROKER_IMPL_H_INCLUDED
#define VICCI_RIVLIB_VICCI_MIDDLEWARE_BROKER_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */


#include "rivlib/common.h"

#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)

#include "rivlib/vicci_middleware_broker.h"
#include "abstract_queued_broker.h"
#include <jni.h>
#include "jni/java_vm_config.h"
#include "jni/java_vm.h"
#include <string>
#include <map>
#include <tuple>


namespace eu_vicci {
namespace rivlib {


    /**
     * Implementation of vicci_middleware_broker
     */
    class vicci_middleware_broker_impl : public vicci_middleware_broker,
        public abstract_queued_broker {
    public:

        /** ctor */
        vicci_middleware_broker_impl(void);

        /** dtor */
        virtual ~vicci_middleware_broker_impl(void);

        /**
         * Clears all entries from the java class path setting. The broker
         * needs to be restarted (disable & enable) in order for the setting
         * to take effect.
         **/
        virtual void clear_java_class_paths(void);

        /**
         * Adds a new path to the java class path setting. The path can
         * specify a file system glob pattern, e.g. to add several jar files
         * at once. However, the call will never recursively enter or add
         * subdirectories. The broker needs to be restarted (disabled &
         * enabled) in order for the setting to take effect.
         *
         * @param path The new path to be added to the java class path setting
         */
        virtual void add_java_class_path(const char *path);

        /**
         * Adds a new path to the java class path setting. The path can
         * specify a file system glob pattern, e.g. to add several jar files
         * at once. However, the call will never recursively enter or add
         * subdirectories. The broker needs to be restarted (disabled &
         * enabled) in order for the setting to take effect.
         *
         * @param path The new path to be added to the java class path setting
         */
        virtual void add_java_class_path(const wchar_t *path);

        /**
         * Sets the vicci semiwa host
         *
         * @param host The name or ip address of the semiwa host machine
         * @param port The port of the semiwa server
         */
        virtual void set_vicci_semiwa_host(const char *host = DEFAULT_VICCI_HOST,
            unsigned short port = DEFAULT_VICCI_HOST_PORT);

        /** initializes the broker */
        virtual void init(void);

        /** deinitializes the broker */
        virtual void deinit(void);

        /**
         * Activates a provider
         *
         * @param comm The communcator
         * @param prov The provider
         */
        virtual void activate(communicator::ptr comm, provider::ptr prov);

        /**
         * Deactivates a provider
         *
         * @param comm The communcator
         * @param prov The provider
         */
        virtual void deactivate(communicator::ptr comm, provider::ptr prov);

    private:

        /** The java vm config */
        java_vm_config cfg;

        /** The java vm */
        java_vm *vm;

        /** The host to connect to */
        std::string host;

        /** The port to connect to */
        unsigned short port;

        /** java class for SeMiWaPublisherConnectionConfiguration */
        jclass conncfg_type;

        /** java ctor for SeMiWaPublisherConnectionConfiguration */
        jmethodID conncfg_ctor;

        /** java class for SeMiWaPublisherConnection */
        jclass conn_type;

        /** java ctor for SeMiWaPublisherConnection */
        jmethodID conn_ctor;

        /** java method "SeMiWaPublisherConnection.sendSensorRegistered" */
        jmethodID conn_sendSensorRegistered;

        /** java method "SeMiWaPublisherConnection.sendSensorUnregistered" */
        jmethodID conn_sendSensorUnregistered;

        /** java method "SeMiWaPublisherConnection.sendSensorData" */
        jmethodID conn_sendSensorData;

        /** Type of connection pairs */
        typedef std::tuple<communicator::ptr, provider::ptr> connection_pair_type;

        /** Type of the connection map */
        typedef std::map<connection_pair_type, jobject> connection_map_type;

        /** the connections activated through this broker */
        connection_map_type connections;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */
#endif /* VICCI_RIVLIB_VICCI_MIDDLEWARE_BROKER_H_INCLUDED */
