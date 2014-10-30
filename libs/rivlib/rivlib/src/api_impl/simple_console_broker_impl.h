/*
 * simple_console_broker_impl.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_SIMPLE_CONSOLE_BROKER_IMPL_H_INCLUDED
#define VICCI_RIVLIB_SIMPLE_CONSOLE_BROKER_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */


#include "rivlib/simple_console_broker.h"
#include "abstract_queued_broker.h"


namespace eu_vicci {
namespace rivlib {

    /**
     * Implementation of the core class
     */
    class simple_console_broker_impl : public simple_console_broker,
        public abstract_queued_broker {
    public:

        /** ctor */
        simple_console_broker_impl(void);

        /** dtor */
        virtual ~simple_console_broker_impl(void);

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

    protected:

    private:

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_SIMPLE_CONSOLE_BROKER_IMPL_H_INCLUDED */
