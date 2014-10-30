/*
 * rivlib API
 * provider.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_PROVIDER_H_INCLUDED
#define VICCI_RIVLIB_PROVIDER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "rivlib/common.h"
#include "rivlib/api_ptr.h"
#include "rivlib/data_binding.h"
//#include "the/deprecated.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {

    
    /** forward declaration */
    class RIVLIB_API provider;
    
    /*
     * force generation of template class for dll interface
     * http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958
     */
    RIVLIB_APIEXT template class RIVLIB_API api_ptr<provider>;

    /**
     * Implements a provider for remote, interative visualizations
     */
    class RIVLIB_API provider {
    public:

        /** The pointer type to be used by the applications */
        typedef api_ptr<provider> ptr;

        /**
         * Type for enumerator functions for enumerating data bindings
         *
         * @param binding The current data_binding
         * @param ctxt The context pointer provided when invoking the enumeration
         *
         * @return True if the enumeration should continue, false if the enumeration should abort now
         */
        typedef bool (*data_binding_enumerator)(data_binding::ptr binding, void* ctxt);

        /**
         * Function pointer type for callback functions when the provider received a user message
         *
         * @param id The user message id
         * @param size The size of the message data
         * @param data The message data
         * @param ctxt The callback context
         */
        typedef void (*user_message_callback_delegate)(unsigned int id, unsigned int size, const char *data, void *ctxt);

        /**
         * Creates a new rivlib provider object
         *
         * @param name A name of the provider
         *
         * @return The newly created rivlib provider object
         */
        static provider::ptr create(const char * name);

        /**
         * Creates a new rivlib provider object
         *
         * @param name A name of the provider
         *
         * @return The newly created rivlib provider object
         */
        static provider::ptr create(const wchar_t * name);

        /**
         * Creates a new rivlib provider object
         *
         * @param name A name of the provider
         *
         * @return The newly created rivlib provider object
         */
//        THE_DEPRECATED
        static inline provider::ptr create_provider(const char * name) {
            return create(name);
        }

        /**
         * Creates a new rivlib provider object
         *
         * @param name A name of the provider
         *
         * @return The newly created rivlib provider object
         */
//        THE_DEPRECATED
        static inline provider::ptr create_provider(const wchar_t * name) {
            return create(name);
        }

        /**
         * Gets the name of the provider
         *
         * @return The name of the provider
         */
        virtual const char* get_name_astr(void) const = 0;

        /**
         * Gets the name of the provider
         *
         * @return The name of the provider
         */
        virtual const wchar_t* get_name_wstr(void) const = 0;

        /**
         * Adds a data_binding to the core
         *
         * @param binding The data_binding to add
         */
        virtual void add_data_binding(data_binding::ptr binding) = 0;

        /**
         * Removes a data_binding from the core
         *
         * @param binding The data_binding to remove
         */
        virtual void remove_data_binding(data_binding::ptr binding) = 0;

        /**
         * Removes all data bindings from the core
         */
        virtual void remove_all_data_bindings(void) = 0;

        /**
         * Enumerates all data bindings of the core
         *
         * @param enumerator The enumerator function called for each data_binding
         * @param ctxt The context pointer used when calling the enumerator function
         */
        virtual void enumerate_data_bindings(data_binding_enumerator enumerator, void *ctxt = nullptr) = 0;

        /**
         * Shuts the provider and all connected object down
         */
        virtual void shutdown(void) = 0;

        /**
         * Adds a callback to be called when a user message is received
         *
         * @param func The callback function to be added
         * @param ctxt The callback function context
         */
        virtual void add_user_message_callback(user_message_callback_delegate func, void *ctxt = nullptr) = 0;

        /**
         * Removes a callback to be called when a user message is received
         *
         * @param func The callback function to be added
         * @param ctxt The callback function context
         */
        virtual void remove_user_message_callback(user_message_callback_delegate func, void *ctxt = nullptr) = 0;

        /**
         * Broadcasts a message to all connected control clients
         *
         * @param id The id of the message (should normally be a user message using RIVLIB_USERMSG)
         * @param size The size of the message data in byte
         * @param data The message data
         */
        virtual void broadcast_message(unsigned int id, unsigned int size, const char *data) = 0;

        /** dtor */
        virtual ~provider(void);

    protected:

        /** ctor */
        provider(void);

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#else /* __cplusplus */

#error C API not implemented yet

#endif /* __cplusplus */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_PROVIDER_H_INCLUDED */
