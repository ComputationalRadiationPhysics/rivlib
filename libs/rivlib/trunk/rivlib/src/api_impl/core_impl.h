/*
 * rivlib API
 * core_impl.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_CORE_IMPL_H_INCLUDED
#define VICCI_RIVLIB_CORE_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */


#include "rivlib/core.h"
#include "node.h"
#include "the/system/threading/critical_section.h"


namespace eu_vicci {
namespace rivlib {

    /**
     * Implementation of the core class
     */
    class core_impl : public core, public node {
    public:

        /** ctor */
        core_impl(void);

        /** dtor */
        virtual ~core_impl(void);

        /**
         * Adds a broker to the core
         *
         * @param broker The broker to add
         */
        virtual void add_broker(broker::ptr broker);

        /**
         * Removes a broker from the core
         *
         * @param broker The broker to remove
         */
        virtual void remove_broker(broker::ptr broker);

        /**
         * Removes all brokers from the core
         */
        virtual void remove_all_brokers(void);

        /**
         * Enumerates all brokers of the core
         *
         * @param enumerator The enumerator function called for each broker
         * @param ctxt The context pointer used when calling the enumerator function
         */
        virtual void enumerate_brokers(broker_enumerator enumerator, void *ctxt = nullptr);

        /**
         * Adds a communicator to the core
         *
         * @param communicator The communicator to add
         */
        virtual void add_communicator(communicator::ptr communicator);

        /**
         * Removes a communicator from the core
         *
         * @param communicator The communicator to remove
         */
        virtual void remove_communicator(communicator::ptr communicator);

        /**
         * Removes all communicators from the core
         */
        virtual void remove_all_communicators(void);

        /**
         * Enumerates all communicators of the core
         *
         * @param enumerator The enumerator function called for each communicator
         * @param ctxt The context pointer used when calling the enumerator function
         */
        virtual void enumerate_communicators(communicator_enumerator enumerator, void *ctxt = nullptr);

        /**
         * Adds a provider to the core
         *
         * @param provider The provider to add
         */
        virtual void add_provider(provider::ptr provider);

        /**
         * Removes a provider from the core
         *
         * @param provider The provider to remove
         */
        virtual void remove_provider(provider::ptr provider);

        /**
         * Removes all providers from the core
         */
        virtual void remove_all_providers(void);

        /**
         * Enumerates all providers of the core
         *
         * @param enumerator The enumerator function called for each provider
         * @param ctxt The context pointer used when calling the enumerator function
         */
        virtual void enumerate_providers(provider_enumerator enumerator, void *ctxt = nullptr);

        /**
         * Shuts the core and all connected object down
         */
        virtual void shutdown(void);

        /**
         * Locks the core
         */
        virtual void lock(void);

        /**
         * Unlocks the core
         */
        virtual void unlock(void);

        /**
         * Answer the error log
         *
         * @return The error log
         */
        virtual error_log& log(void);

    protected:

    private:

        /** The thread lock object */
        the::system::threading::critical_section lock_obj;

        /** The error log */
        error_log log_obj;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_CORE_IMPL_H_INCLUDED */
