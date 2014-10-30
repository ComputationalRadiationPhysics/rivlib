/*
 * rivlib API
 * core.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_CORE_H_INCLUDED
#define VICCI_RIVLIB_CORE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "rivlib/common.h"
#include "rivlib/api_ptr.h"
#include "rivlib/broker.h"
#include "rivlib/communicator.h"
#include "rivlib/provider.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {

    /** forward declaration */
    class RIVLIB_API core;
    
    /*
     * force generation of template class for dll interface
     * http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958
     */
    RIVLIB_APIEXT template class RIVLIB_API api_ptr<core>;

    /**
     * The core management class of the rivlib
     */
    class RIVLIB_API core {
    public:

        /** The pointer type to be used by the applications */
        typedef api_ptr<core> ptr;

        /**
         * Type for enumerator functions for enumerating brokers
         *
         * @param broker The current broker
         * @param ctxt The context pointer provided when invoking the enumeration
         *
         * @return True if the enumeration should continue, false if the enumeration should abort now
         */
        typedef bool (*broker_enumerator)(broker::ptr broker, void* ctxt);

        /**
         * Type for enumerator functions for enumerating communicators
         *
         * @param communicator The current communicator
         * @param ctxt The context pointer provided when invoking the enumeration
         *
         * @return True if the enumeration should continue, false if the enumeration should abort now
         */
        typedef bool (*communicator_enumerator)(communicator::ptr communicator, void* ctxt);

        /**
         * Type for enumerator functions for enumerating providers
         *
         * @param provider The current provider
         * @param ctxt The context pointer provided when invoking the enumeration
         *
         * @return True if the enumeration should continue, false if the enumeration should abort now
         */
        typedef bool (*provider_enumerator)(provider::ptr provider, void* ctxt);

        /**
         * Creates a new rivlib core object
         *
         * @return The newly created rivlib core object
         */
        static core::ptr create(void);

        /**
         * Adds a broker to the core
         *
         * @param broker The broker to add
         */
        virtual void add_broker(broker::ptr broker) = 0;

        /**
         * Removes a broker from the core
         *
         * @param broker The broker to remove
         */
        virtual void remove_broker(broker::ptr broker) = 0;

        /**
         * Removes all brokers from the core
         */
        virtual void remove_all_brokers(void) = 0;

        /**
         * Enumerates all brokers of the core
         *
         * @param enumerator The enumerator function called for each broker
         * @param ctxt The context pointer used when calling the enumerator function
         */
        virtual void enumerate_brokers(broker_enumerator enumerator, void *ctxt = nullptr) = 0;

        /**
         * Adds a communicator to the core
         *
         * @param communicator The communicator to add
         */
        virtual void add_communicator(communicator::ptr communicator) = 0;

        /**
         * Removes a communicator from the core
         *
         * @param communicator The communicator to remove
         */
        virtual void remove_communicator(communicator::ptr communicator) = 0;

        /**
         * Removes all communicators from the core
         */
        virtual void remove_all_communicators(void) = 0;

        /**
         * Enumerates all communicators of the core
         *
         * @param enumerator The enumerator function called for each communicator
         * @param ctxt The context pointer used when calling the enumerator function
         */
        virtual void enumerate_communicators(communicator_enumerator enumerator, void *ctxt = nullptr) = 0;

        /**
         * Adds a provider to the core
         *
         * @param provider The provider to add
         */
        virtual void add_provider(provider::ptr provider) = 0;

        /**
         * Removes a provider from the core
         *
         * @param provider The provider to remove
         */
        virtual void remove_provider(provider::ptr provider) = 0;

        /**
         * Removes all providers from the core
         */
        virtual void remove_all_providers(void) = 0;

        /**
         * Enumerates all providers of the core
         *
         * @param enumerator The enumerator function called for each provider
         * @param ctxt The context pointer used when calling the enumerator function
         */
        virtual void enumerate_providers(provider_enumerator enumerator, void *ctxt = nullptr) = 0;

        /**
         * Shuts the core and all connected object down
         */
        virtual void shutdown(void) = 0;

        /** dtor */
        virtual ~core(void);

    protected:

        /** ctor */
        core(void);

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
#endif /* VICCI_RIVLIB_CORE_H_INCLUDED */
