/*
 * rivlib API
 * provider_impl.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_PROVIDER_IMPL_H_INCLUDED
#define VICCI_RIVLIB_PROVIDER_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */


#include "rivlib/provider.h"
#include "element_node.h"
#include "the/string.h"
#include "the/multicast_delegate.h"
#include "data_channel_info.h"
#include <vector>


namespace eu_vicci {
namespace rivlib {

    /**
     * Implementation of the core class
     */
    class provider_impl : public provider, public element_node {
    public:

        /**
         * ctor
         *
         * @param name The name
         */
        provider_impl(const the::wstring& name);

        /** dtor */
        virtual ~provider_impl(void);

        /**
         * Gets the name of the provider
         *
         * @return The name of the provider
         */
        virtual const char* get_name_astr(void) const;

        /**
         * Gets the name of the provider
         *
         * @return The name of the provider
         */
        virtual const wchar_t* get_name_wstr(void) const;

        /**
         * Adds a data_binding to the core
         *
         * @param binding The data_binding to add
         */
        virtual void add_data_binding(data_binding::ptr binding);

        /**
         * Removes a data_binding from the core
         *
         * @param binding The data_binding to remove
         */
        virtual void remove_data_binding(data_binding::ptr binding);

        /**
         * Removes all data bindings from the core
         */
        virtual void remove_all_data_bindings(void);

        /**
         * Enumerates all data bindings of the core
         *
         * @param enumerator The enumerator function called for each data_binding
         * @param ctxt The context pointer used when calling the enumerator function
         */
        virtual void enumerate_data_bindings(data_binding_enumerator enumerator, void *ctxt = nullptr);

        /**
         * Shuts the provider and all connected object down
         */
        virtual void shutdown(void);

        /**
         * Adds a callback to be called when a user message is received
         *
         * @param func The callback function to be added
         * @param ctxt The callback function context
         */
        virtual void add_user_message_callback(user_message_callback_delegate func, void *ctxt = nullptr);

        /**
         * Removes a callback to be called when a user message is received
         *
         * @param func The callback function to be added
         * @param ctxt The callback function context
         */
        virtual void remove_user_message_callback(user_message_callback_delegate func, void *ctxt = nullptr);

        /**
         * Broadcasts a message to all connected control clients
         *
         * @param id The id of the message (should normally be a user message using RIVLIB_USERMSG)
         * @param size The size of the message data in byte
         * @param data The message data
         */
        virtual void broadcast_message(unsigned int id, unsigned int size, const char *data);

        /**
         * Answer all data channels available at this provider
         *
         * @return All data channels available
         */
        std::vector<data_channel_info> query_channels(void);

        /**
         * Indicates that a user message has been received
         *
         * @param id The id of the message (should normally be a user message using RIVLIB_USERMSG)
         * @param size The size of the message data in byte
         * @param data The message data
         */
        void on_user_message_received(unsigned int id, unsigned int size, const char *data);

    protected:

    private:

        /** the name of the provider */
        the::astring name_astr;

        /** the name of the provider */
        the::wstring name_wstr;

        /** Delegate fired when a user message is received */
        the::multicast_delegate<the::delegate<void, unsigned int, unsigned int, const char *> > on_user_msg;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_PROVIDER_IMPL_H_INCLUDED */
