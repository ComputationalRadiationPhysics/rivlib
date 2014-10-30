/*
 * abstract_queued_broker.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_ABSTRACT_QUEUED_BROKER_IMPL_H_INCLUDED
#define VICCI_RIVLIB_ABSTRACT_QUEUED_BROKER_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "element_node.h"
#include "rivlib/core.h"
#include "rivlib/communicator.h"
#include "rivlib/provider.h"
#include "the/collections/array.h"
#include "the/system/threading/runnable.h"
#include "the/system/threading/thread.h"
#include "the/system/threading/critical_section.h"


namespace eu_vicci {
namespace rivlib {

    /**
     * Abstract base class for queued brokers
     */
    class abstract_queued_broker : public element_node, public the::system::threading::runnable {
    public:

        /** ctor */
        abstract_queued_broker(void);

        /** dtor */
        virtual ~abstract_queued_broker(void);

        /** initializes the broker */
        virtual void init(void) = 0;

        /** deinitializes the broker */
        virtual void deinit(void) = 0;

        /**
         * Activates a provider
         *
         * @param comm The communcator
         * @param prov The provider
         */
        virtual void activate(communicator::ptr comm, provider::ptr prov) = 0;

        /**
         * Deactivates a provider
         *
         * @param comm The communcator
         * @param prov The provider
         */
        virtual void deactivate(communicator::ptr comm, provider::ptr prov) = 0;

        /**
         * Terminates the worker thread
         *
         * @return gracefully
         */
        virtual the::system::threading::thread::termination_behaviour on_thread_terminating(void) throw();

        /**
         * Perform the work of a thread.
         *
         * @return 0
         */
        virtual int run(void);

    protected:

        /**
         * Called to fire the 'core_discovered' event
         */
        virtual void on_core_discovered(void);

        /**
         * Called to fire the 'core_lost' event
         */
        virtual void on_core_lost(void);

        /**
         * reacts on connections and disconnections at the core
         *
         * @param peer Not used
         */
        void on_core_connections_updated(api_ptr_base peer);

        /**
         * Queues a comm-prov pair for activation
         *
         * @param comm The communicator
         * @param prov The provider
         */
        void queue_activation(communicator::ptr comm, provider::ptr prov);

        /**
         * Queues a comm-prov pair for deactivation
         *
         * @param comm The communicator
         * @param prov The provider
         */
        void queue_deactivation(communicator::ptr comm, provider::ptr prov);

    private:

        /** Possible action codes */
        enum class action_type {
            none,
            activate,
            deactivate,
            shutdown
        };

        /** The action list entries */
        class action_entry {
        public:

            /** ctor */
            action_entry(void) : act(action_type::none), comm(nullptr), prov(nullptr) {
                // intentionally empty
            }

            /**
            * Copy ctor
            *
            * @param src The object to clone from
            */
            action_entry(const action_entry& src) : act(src.act), comm(src.comm), prov(src.prov) {
                // intentionally empty
            }

            /** ctor */
            ~action_entry(void) {
                this->comm.reset();
                this->prov.reset();
            }

            /**
            * Assignment operator
            *
            * @param rhs The right hand side operand
            *
            * @return A reference to this
            */
            action_entry& operator=(const action_entry& rhs) {
                this->act = rhs.act;
                this->comm = rhs.comm;
                this->prov = rhs.prov;
                return *this;
            }

            /**
            * Test for equality
            *
            * @param rhs The right hand side operand
            *
            * @return True when rhs equals this
            */
            bool operator==(const action_entry& rhs) {
                return (this->act == rhs.act)
                    && (this->comm == rhs.comm)
                    && (this->prov == rhs.prov);
            }

            /** The action type */
            action_type act;

            /** The communicator object */
            communicator::ptr comm;

            /** The provider object */
            provider::ptr prov;

        };

        /** updates the broker information */
        void update_info(void);

        /** remember the core */
        core::ptr safe_core;

        /** the list of registered communicators */
        the::collections::array<communicator::ptr> comms;

        /** The list of registered providers */
        the::collections::array<provider::ptr> provs;

        /** The thread lock object */
        the::system::threading::critical_section lock_obj;

        /** The action queue */
        the::collections::array<action_entry> queue;

        /** The worker thread */
        the::system::threading::thread *worker_thread;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#endif /* VICCI_RIVLIB_ABSTRACT_QUEUED_BROKER_IMPL_H_INCLUDED */
