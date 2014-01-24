/*
 * rivlib
 * node.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_NODE_H_INCLUDED
#define VICCI_RIVLIB_NODE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "rivlib/node_base.h"
#include "rivlib/api_ptr_base.h"
#include "rivlib/api_ptr.h"
#include "the/system/threading/auto_lock.h"
#include "the/multicast_delegate.h"
#include "error_log.h"
#include <vector>
#include <memory>


namespace eu_vicci {
namespace rivlib {


    /**
     * Base class for rivlib internal node objects
     */
    class node : public node_base {
    public:

        /** Dtor */
        virtual ~node(void);

        /**
         * Access to the 'connecting' event, fired whenever a new peer is about to be connected to this object
         *
         * @return The 'connecting' event delegate
         */
        inline the::multicast_delegate<the::delegate<void, api_ptr_base, bool&> >& connecting(void) {
            return this->connecting_event;
        }

        /**
         * Access to the 'connected' event, fired after a new peer has been connected to this object
         *
         * @return The 'connected' event delegate
         */
        inline the::multicast_delegate<the::delegate<void, api_ptr_base> >& connected(void) {
            return this->connected_event;
        }

        /**
         * Access to the 'disconnecting' event, fired whenever a peer is about to be disconnected from this object
         *
         * @return The 'disconnecting' event delegate
         */
        inline the::multicast_delegate<the::delegate<void, api_ptr_base, bool&> >& disconnecting(void) {
            return this->disconnecting_event;
        }

        /**
         * Access to the 'disconnected' event, fired after a peer has been disconnected from this object
         *
         * @return The 'disconnected' event delegate
         */
        inline the::multicast_delegate<the::delegate<void, api_ptr_base> >& disconnected(void) {
            return this->disconnected_event;
        }

        /**
         * Locks this node
         */
        virtual void lock(void) = 0;

        /**
         * Unlocks this node
         */
        virtual void unlock(void) = 0;

        /**
         * Answer the error log
         *
         * @return The error log
         */
        virtual error_log& log(void) = 0;

        /**
         * Answer a 'shared_ptr' positively comparable to previously created api_ptr objects
         *
         * @remarks: This is needed by the api_ptr mechanism. Do not call directly!
         *
         * @return A 'shared_ptr' of this object
         */
        std::shared_ptr<node_base> get_api_ptr(void);

        /**
         * Creates a list of all connected peers which can be cast to the
         * template parameter type
         *
         * @param T The type to cast the peers to
         *
         * @return A list with all peers of the specified type
         */
        template<class T>
        std::vector<api_ptr_base> select(void);

        /**
         * Connects 'peer' to this object
         *
         * @param peer The peer object to connect
         */
        void connect(api_ptr_base peer);

        /**
         * Disconnects a 'peer' from this object
         *
         * @param peer The peer object to disconnect
         */
        void disconnect(api_ptr_base peer);

        /**
         * Disconnects all peers from this object
         */
        void disconnect_all(void);

        /**
         * Disconnects all peers from this object recursively
         */
        void disconnect_all_recursively(void);

        /**
         * Answer whether 'peer' is connected to this object
         *
         * @param peer The object to test
         *
         * @return True if 'peer' is connected to this object
         */
        bool is_connected(api_ptr_base peer);

        /**
         * Called to fire the 'connecting' event
         *
         * @param peer The new peer
         * @param abort Set to true to abort the connection process
         */
        virtual void on_connecting(api_ptr_base peer, bool& abort);
        
        /**
         * Called to fire the 'connected' event
         *
         * @param peer The new peer
         */
        virtual void on_connected(api_ptr_base peer);

        /**
         * Called to fire the 'disconnecting' event
         *
         * @param peer The peer
         * @param abort Set to true to abort the disconnection process
         */
        virtual void on_disconnecting(api_ptr_base peer, bool& abort);
        
        /**
         * Called to fire the 'disconnected' event
         *
         * @param peer The peer
         */
        virtual void on_disconnected(api_ptr_base peer);

    protected:

        /** Ctor */
        node(void);

    private:

        /** This weak pointer will be used to construct all api_ptr objects from */
        std::weak_ptr<node_base> self_api_ptr;

        /** The list of all connected peer objects */
        std::vector<api_ptr_base> peer_nodes;

        /** The event called before a new connection is established */
        the::multicast_delegate<the::delegate<void, api_ptr_base, bool&> > connecting_event;

        /** The event called after a new connection has been established */
        the::multicast_delegate<the::delegate<void, api_ptr_base> > connected_event;

        /** The event called before a connection is broken up */
        the::multicast_delegate<the::delegate<void, api_ptr_base, bool&> > disconnecting_event;

        /** The event called after a connection has been broken up */
        the::multicast_delegate<the::delegate<void, api_ptr_base> > disconnected_event;

    };


    /*
     * node::select<T>
     */
    template<class T>
    std::vector<api_ptr_base> node::select(void) {
        the::system::threading::auto_lock<node>(*this);
        std::vector<api_ptr_base> ret;
        size_t cnt = this->peer_nodes.size();
        for (size_t i = 0; i < cnt; i++) {
            api_ptr_base& n = this->peer_nodes[i];
            if (dynamic_cast<T*>(n.get()) != nullptr) {
                ret.push_back(this->peer_nodes[i]);
            }
        }
        return ret;
    }

    
} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_NODE_H_INCLUDED */
