/*
 * rivlib
 * element_node.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_ELEMENT_NODE_H_INCLUDED
#define VICCI_RIVLIB_ELEMENT_NODE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "node.h"
#include <memory.h>
#include "rivlib/core.h"
#include "the/multicast_delegate.h"


namespace eu_vicci {
namespace rivlib {

    /**
     * Base class for rivlib internal node objects
     */
    class element_node : public node {
    public:

        /** Dtor */
        virtual ~element_node(void);

        /**
         * Locks this node
         */
        virtual void lock(void);

        /**
         * Unlocks this node
         */
        virtual void unlock(void);

        /**
         * Access to the 'core_discovered' event, fired when the core has been discovered
         *
         * @return The 'core_discovered' event delegate
         */
        inline the::multicast_delegate<the::delegate<> >& core_discovered(void) {
            return this->core_discovered_event;
        }

        /**
         * Access to the 'core_lost' event, fired when the core has been lost
         *
         * @return The 'core_lost' event delegate
         */
        inline the::multicast_delegate<the::delegate<> >& core_lost(void) {
            return this->core_lost_event;
        }


    protected:

        /** Ctor */
        element_node(void);

        /**
         * Answer the core
         *
         * @return The core
         */
        ::eu_vicci::rivlib::core::ptr get_core(void);
        
        /**
         * Called to fire the 'connected' event
         *
         * @param peer The new peer
         */
        virtual void on_connected(api_ptr_base peer);
        
        /**
         * Called to fire the 'disconnected' event
         *
         * @param peer The peer
         */
        virtual void on_disconnected(api_ptr_base peer);

        /**
         * Called to fire the 'core_discovered' event
         */
        virtual void on_core_discovered(void);

        /**
         * Called to fire the 'core_lost' event
         */
        virtual void on_core_lost(void);

        /**
         * Answer the error log
         *
         * @return The error log
         */
        virtual error_log& log(void);

    private:

        /**
         * searches for the core
         */
        void search_core(void);

        /** The core */
        std::weak_ptr<node_base> core;

        /** The core used for locking */
        std::shared_ptr<node_base> lock_obj;

        /** The simple search lock */
        bool search_lock;

        /** Event fired when the core has been discovered */
        the::multicast_delegate<the::delegate<> > core_discovered_event;

        /** Event fired when the core has been lost */
        the::multicast_delegate<the::delegate<> > core_lost_event;

    };

    
} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_ELEMENT_NODE_H_INCLUDED */
