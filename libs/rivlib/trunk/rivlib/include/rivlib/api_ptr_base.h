/*
 * rivlib API
 * api_ptr_base.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_API_PTR_BASE_H_INCLUDED
#define VICCI_RIVLIB_API_PTR_BASE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "rivlib/common.h"
#include "rivlib/node_base.h"

#ifdef __cplusplus

#include <memory>

    
/*
 * force generation of template class for dll interface
 * http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958
 */
RIVLIB_APIEXT template class RIVLIB_API std::shared_ptr<eu_vicci::rivlib::node_base>;


namespace eu_vicci {
namespace rivlib {
    

    /**
     * The base class for smart api pointers
     */
    class RIVLIB_API api_ptr_base {
    public:

        /** Ctor */
        api_ptr_base(void);

        /**
         * Ctor
         *
         * @param o The object to point to
         */
        api_ptr_base(node_base *o);

        /**
         * Copy ctor
         *
         * @param src The pointer to clone
         */
        api_ptr_base(const api_ptr_base& src);

        /** Dtor */
        virtual ~api_ptr_base(void);

        /**
         * Returns a temporary native pointer
         *
         * @remarks Use this pointer for temporary access only! Never reassign
         *          this pointer to another shared pointer.
         *
         * @return A temporary native pointer
         */
        node_base* get(void);

        /**
         * Returns a temporary native pointer
         *
         * @remarks Use this pointer for temporary access only! Never reassign
         *          this pointer to another shared pointer.
         *
         * @return A temporary native pointer
         */
        const node_base* get(void) const;

        /**
         * Resets the shared pointer
         */
        void reset(void);
       
        /**
         * Assignment operator
         *
         * @param rhs the right hand side operand
         *
         * @return A reference to this object
         */
        api_ptr_base& operator =(const api_ptr_base& rhs);

        /**
         * Test for equality
         *
         * @param rhs the right hand side operand
         *
         * @return True if this and rhs point to the same object
         */
        bool operator==(const api_ptr_base& rhs) const;

        /**
         * Test for inequality
         *
         * @param rhs the right hand side operand
         *
         * @return True if this and rhs point to different object
         */
        bool operator!=(const api_ptr_base& rhs) const;

        /**
         * Tests if an object is set
         *
         * @return True if the pointer points to an object
         */
        virtual operator bool(void) const;

    protected:

        /** The shared pointer */
        std::shared_ptr<node_base> obj;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#else /* __cplusplus */

#error C API not implemented yet

#endif /* __cplusplus */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_API_PTR_BASE_H_INCLUDED */
