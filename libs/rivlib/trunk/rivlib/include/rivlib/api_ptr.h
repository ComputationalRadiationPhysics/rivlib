/*
 * rivlib API
 * api_ptr.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_API_PTR_H_INCLUDED
#define VICCI_RIVLIB_API_PTR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifdef __cplusplus

#include "rivlib/api_ptr_base.h"
#include "rivlib/node_base.h"

namespace eu_vicci {
namespace rivlib {
    

    /**
     * The smart api pointer
     */
    template<class T>
    class api_ptr : public api_ptr_base {
    public:

        /** ctor */
        api_ptr(void);

        /**
         * ctor
         *
         * @param o The object to point to
         */
        api_ptr(T* o);

        /**
         * Copy ctor
         *
         * @param src The pointer to clone
         */
        api_ptr(const api_ptr& src);

        /** dtor */
        virtual ~api_ptr(void);

        /**
         * Returns a temporary native pointer
         *
         * @remarks Use this pointer for temporary access only! Never reassign
         *          this pointer to another shared pointer.
         *
         * @return A temporary native pointer
         */
        template<class Tp>
        inline Tp* get_as(void) {
            return dynamic_cast<Tp*>(this->get());
        }

        /**
         * Returns a temporary native pointer
         *
         * @remarks Use this pointer for temporary access only! Never reassign
         *          this pointer to another shared pointer.
         *
         * @return A temporary native pointer
         */
        template<class Tp>
        inline const Tp* get_as(void) const {
            return dynamic_cast<Tp*>(this->get());
        }

        /**
         * Returns a temporary native pointer
         *
         * @remarks Use this pointer for temporary access only! Never reassign
         *          this pointer to another shared pointer.
         *
         * @return A temporary native pointer
         */
        T *get(void);

        /**
         * Returns a temporary native pointer
         *
         * @remarks Use this pointer for temporary access only! Never reassign
         *          this pointer to another shared pointer.
         *
         * @return A temporary native pointer
         */
        const T *get(void) const;

        /** make 'reset' base implementation available*/
        using api_ptr_base::reset;

        /**
         * Resets the api pointer to point to a new object
         *
         * @param o The new object to point to
         */
        void reset(T *o);

        /**
         * Assignment operator
         *
         * @param rhs the right hand side operand
         *
         * @return A reference to this object
         */
        api_ptr& operator=(const api_ptr& rhs);

        /**
         * Test for equality
         *
         * @param rhs the right hand side operand
         *
         * @return True if this and rhs point to the same object
         */
        bool operator==(const api_ptr& rhs) const;

        /**
         * Test for inequality
         *
         * @param rhs the right hand side operand
         *
         * @return True if this and rhs point to different object
         */
        bool operator!=(const api_ptr& rhs) const;

        /**
         * Tests if an object is set
         *
         * @return True if the pointer points to an object
         */
        virtual operator bool(void) const;

        /**
         * Access members of the target object
         *
         * @return The target object
         */
        T* operator ->(void);

        /**
         * Access members of the target object
         *
         * @return The target object
         */
        const T* operator ->(void) const;

    };


    /*
     * api_ptr::api_ptr
     */
    template<class T>
    api_ptr<T>::api_ptr(void) : api_ptr_base() {
        // intentionally empty
    }
    

    /*
     * api_ptr::api_ptr
     */
    template<class T>
    api_ptr<T>::api_ptr(T* o) : api_ptr_base(dynamic_cast<node_base*>(o)) {
        // intentionally empty
    }
    

    /*
     * api_ptr::api_ptr
     */
    template<class T>
    api_ptr<T>::api_ptr(const api_ptr& src) : api_ptr_base(src) {
        // intentionally empty
    }
    

    /*
     * api_ptr::~api_ptr
     */
    template<class T>
    api_ptr<T>::~api_ptr(void) {
        // intentionally empty
    }
    

    /*
     * api_ptr::get
     */
    template<class T>
    T *api_ptr<T>::get(void) {
        return dynamic_cast<T*>(this->obj.get());
    }
    

    /*
     * api_ptr::get
     */
    template<class T>
    const T *api_ptr<T>::get(void) const {
        return dynamic_cast<T*>(this->obj.get());
    }
    

    /*
     * api_ptr::reset
     */
    template<class T>
    void api_ptr<T>::reset(T *o) {
        this->obj.reset(dynamic_cast<node_base*>(o));
    }
    

    /*
     * api_ptr::operator =
     */
    template<class T>
    api_ptr<T>& api_ptr<T>::operator=(const api_ptr& rhs) {
        api_ptr_base::operator=(rhs);
        return *this;
    }
    

    /*
     * api_ptr::operator==
     */
    template<class T>
    bool api_ptr<T>::operator==(const api_ptr& rhs) const {
        return api_ptr_base::operator==(rhs);
    }
    

    /*
     * api_ptr::operator!=
     */
    template<class T>
    bool api_ptr<T>::operator!=(const api_ptr& rhs) const {
        return !api_ptr_base::operator==(rhs);
    }
    

    /*
     * api_ptr::operator bool
     */
    template<class T>
    api_ptr<T>::operator bool(void) const {
        return dynamic_cast<T*>(this->obj.get()) != nullptr;
    }
    

    /*
     * api_ptr::operator ->
     */
    template<class T>
    T* api_ptr<T>::operator ->(void) {
        return dynamic_cast<T*>(this->obj.get());
    }
    

    /*
     * api_ptr::operator ->
     */
    template<class T>
    const T* api_ptr<T>::operator ->(void) const {
        return dynamic_cast<const T*>(this->obj.get());
    }


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#else /* __cplusplus */

#error C API not implemented yet

#endif /* __cplusplus */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_API_PTR_H_INCLUDED */
