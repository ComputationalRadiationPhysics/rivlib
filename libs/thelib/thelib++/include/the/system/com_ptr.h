/*
 * include\the\system\com_ptr.h
 *
 * Copyright (C) 2012 TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THE_SYSTEM_COM_PTR_H_INCLUDED
#define THE_SYSTEM_COM_PTR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/assert.h"
#include "the/null_pointer_exception.h"
#include "the/stack_trace.h"

#include "the/system/com_exception.h"


namespace the {
namespace system {

    /**
     * The com_ptr is a smart pointer for COM objects. It manages the reference
     * count of the object that it designates by calling AddRef() and Release().
     *
     * @tparam The type of the COM class that the pointer points to.
     */
    template<class T> class com_ptr {

    public:

        /**
         * The constant type of COM interface that is designated by the pointer.
         */
        typedef const T *const_pointer_type;

        /** 
         * A constant reference to the defererenced type of the COM interface.
         */
        typedef const T& const_reference_type;

        /** The type of COM interface that is designated by the pointer. */
        typedef T *pointer_type;

        /** A reference to the defererenced type of the COM interface. */
        typedef T& reference_type;

        /**
         * Initialises a new instance.
         *
         * @param ptr    The object to be referenced. This parameter defaults 
         *               to NULL.
         * @param addRef Specifies whether the ctor should increase the 
         *               reference count or not. This parameter defaults to
         *               true.
         */
        com_ptr(pointer_type ptr = NULL, const bool addRef = true) throw();

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline com_ptr(const com_ptr& rhs) throw() : ptr(NULL) {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /** Dtor. */
        virtual ~com_ptr(void) throw();

        /**
         * Answer, whether the pointer is currently not NULL.
         *
         * @return true, if the reference pointer is not NULL, false otherwise.
         */
        inline bool is_not_null(void) const throw() {
            THE_STACK_TRACE;
            return (this->ptr != NULL);
        }

        /**
         * Answer, whether the pointer is currently NULL.
         *
         * @return true, if the reference pointer is NULL, false otherwise.
         */
        inline bool is_null(void) const throw() {
            THE_STACK_TRACE;
            return (this->ptr == NULL);
        }

#ifdef THE_WINDOWS
        /**
         * Query the COM interface with the specified ID from the object
         * and return a new pointer for the result.
         *
         * @param iid The ID of the interface to query.
         *
         * @return A new pointer to the interface of the object.
         *
         * @throws com_exception          If the call to QueryInterface() on
         *                                the object designated by the pointer
         *                                failed.
         * @throws null_pointer_exception If the pointer is currently NULL.
         */
        template<class Tp> com_ptr<Tp> query_interface(const IID& iid);
#endif /* THE_WINDOWS */

#ifdef _MSC_VER
        /**
         * Query the COM interface 'Tp' from the object and return a new pointer
         * for the result.
         *
         * @return A new pointer to the interface of the object.
         *
         * @throws com_exception          If the call to QueryInterface() on
         *                                the object designated by the pointer
         *                                failed.
         * @throws null_pointer_exception If the pointer is currently NULL.
         */
        template<class Tp> inline com_ptr<Tp> query_interface(void) {
            THE_STACK_TRACE;
            return this->query_interface<Tp>(__uuidof(Tp));
        }

        /**
         * Query the COM interface 'Tp' from the object and return a new pointer
         * for the result.
         *
         * The advantage of this method is that the compiler can deduce the 
         * template parameter and therefore the ID of the interface, too.
         *
         * @param out The pointer to receive the interface. This parameter will
         *            also be returned.
         *
         * @return 'out'.
         *
         * @throws com_exception          If the call to QueryInterface() on
         *                                the object designated by the pointer
         *                                failed.
         * @throws null_pointer_exception If the pointer is currently NULL.
         */
        template<class Tp>
        inline com_ptr<Tp>& query_interface(com_ptr<Tp>& out) {
            THE_STACK_TRACE;
            out = this->query_interface<Tp>(__uuidof(Tp));
            return out;
        }
#endif /* _MSC_VER */

        /**
         * Release the reference and make the pointer NULL.
         */
        void release(void) throw();

        /**
         * Assignment operator.
         *
         * This operation creates a strong reference, i. e. decrements the 
         * reference count of the old object and increments the reference count
         * of the new one.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        com_ptr& operator =(const com_ptr& rhs) throw();

        /**
         * Assign values of 'rhs' to this object.
         *
         * This operation creates a strong reference, i. e. decrements the 
         * reference count of the old object and increments the reference count
         * of the new one.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        com_ptr& operator =(pointer_type rhs) throw();

        /**
         * Answer, whether two pointers are equal, i.e. designate the same 
         * object.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are equal, false otherwise.
         */
        inline bool operator ==(const com_ptr& rhs) const throw() {
            THE_STACK_TRACE;
            return (this->ptr == rhs.ptr);
        }

        /**
         * Answer, whether two pointers are equal, i.e. designate the same 
         * object.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are equal, false otherwise.
         */
        inline bool operator ==(const_pointer_type rhs) const throw() {
            THE_STACK_TRACE;
            return (this->ptr == rhs);
        }

        /**
         * Answer, whether two pointers are not equal, i.e. designate different 
         * objects.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are not equal, false otherwise.
         */
        inline bool operator !=(const com_ptr& rhs) const throw() {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Answer, whether two pointers are not equal, i.e. designate different
         * object.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are not equal, false otherwise.
         */
        inline bool operator !=(const_pointer_type rhs) const throw() {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Member access.
         * 
         * The member access operation creates a weak reference to the 
         * referenced object, i.e. the reference count is not modified.
         *
         * @return A pointer to the object designated by the pointer.
         */
        inline pointer_type operator ->(void) throw() {
            THE_STACK_TRACE;
            return this->ptr;
        }

        /**
         * Member access.
         * 
         * The member access operation creates a weak reference to the 
         * referenced object, i.e. the reference count is not modified.
         *
         * @return A pointer to the object designated by the pointer.
         */
        inline const_pointer_type operator ->(void) const throw() {
            THE_STACK_TRACE;
            return this->ptr;
        }

        /**
         * Expose the native pointer.
         *
         * The member access operation creates a weak reference to the 
         * referenced object, i.e. the reference count is not modified.
         *
         * @return A pointer to the object designated by the pointer.
         */
        inline operator pointer_type(void) const throw() {
            THE_STACK_TRACE;
            return this->ptr;
        }

        /**
         * Dereference the pointer.
         *
         * @return A reference to the COM interface.
         *
         * @throws null_pointer_exception If the pointer is currently NULL.
         */
        inline reference_type operator *(void) {
            THE_STACK_TRACE;
            this->check_not_null();
            return *this->ptr;
        }

        /**
         * Dereference the pointer.
         *
         * @return A reference to the COM interface.
         *
         * @throws null_pointer_exception If the pointer is currently NULL.
         */
        inline const_reference_type operator *(void) const {
            THE_STACK_TRACE;
            this->check_not_null();
            return *this->ptr;
        }

        /**
         * Answer the address of the object designated by the pointer_type.
         *
         * The address operator is required in order to make calls to COM
         * functions that return objects transparent. It is safe to use the
         * operator this way, because the reference count of the returned
         * value has been incremented by the method called (that is by 
         * convention). Therefore, it does not matter that this way of using
         * the operator bypasses incrementing the reference counter. However,
         * you must ensure that the com_ptr does not store any valid reference
         * when using it this way, i.e. that it is NULL. The debug version
         * of this method asserts against this, because there is no other 
         * meaningful use case for this operator than the one described above.
         *
         * Implementation note: There must not be a const variant of this
         * operator! If there would be one, the assignment operator would not
         * work any more as it could not get the address of the right hand side
         * operand. This restriction is, however, no problem because there is
         * no meaningful use case for a const address operator.
         *
         * @return The address of the object designated by the pointer.
         */
        inline pointer_type *operator &(void) throw() {
            THE_STACK_TRACE;
            THE_ASSERT(this->is_null());
            return &this->ptr;
        }

    private:

        /**
         * Checks whether the pointer is NULL and raises an exception in
         * this case. Otherwise, nothing happens.
         *
         * @throws null_pointer_exception If the pointer is currently NULL.
         */
        void check_not_null(void);

        /** The referenced object. */
        T *ptr;

    };


    /**
     * If not NULL, call Release() on the object designated by 'inOutPtr' and
     * set 'inOutPtr' NULL.
     *
     * @param inOutPtr A pointer to a COM object.
     */
    template<class T> inline void safe_release(T *& inOutPtr) throw() {
        THE_STACK_TRACE;
        if (inOutPtr != NULL) {
            inOutPtr->Release();
            inOutPtr = NULL;
        }
    }

} /* end namespace system */
} /* end namespace the */


/*
 * the::system::com_ptr<T>::com_ptr
 */
template<class T>
the::system::com_ptr<T>::com_ptr(pointer_type ptr, const bool addRef) throw()
        : ptr(ptr) {
    THE_STACK_TRACE;
    if (addRef && (this->ptr != NULL)) {
        this->ptr->AddRef();
    }
}


/*
 * the::system::com_ptr<T>::~com_ptr
 */
template<class T> the::system::com_ptr<T>::~com_ptr(void) throw() {
    THE_STACK_TRACE;
    this->release();
}


#ifdef THE_WINDOWS
/*
 * the::system::com_ptr<T>::query_interface
 */
template<class T> 
template<class Tp> 
the::system::com_ptr<Tp> the::system::com_ptr<T>::query_interface(
        const IID& iid) {
    THE_STACK_TRACE;
    this->check_not_null();

    Tp *p = NULL;
    HRESULT hr = S_OK;

    hr = this->ptr->QueryInterface(iid, reinterpret_cast<void **>(&p));
    if (FAILED(hr)) {
        throw com_exception(hr, __FILE__, __LINE__);
    }

    return com_ptr<Tp>(p, false);
}
#endif /* THE_WINDOWS */


/*
 * the::system::com_ptr<T>::release
 */
template<class T> void the::system::com_ptr<T>::release(void) throw() {
    THE_STACK_TRACE;
    the::system::safe_release(this->ptr);
}


/*
 * the::system::com_ptr<T>::operator =
 */
template<class T> the::system::com_ptr<T>& the::system::com_ptr<T>::operator =(
        const com_ptr& rhs) throw() {
    THE_STACK_TRACE;
    if (this != &rhs) {
        if (rhs.ptr != NULL) {
            rhs.ptr->AddRef();
        }
        this->release();
        this->ptr = rhs.ptr;
    }
    return *this;
}


/*
 * the::system::com_ptr<T>::operator =
 */
template<class T> the::system::com_ptr<T>& the::system::com_ptr<T>::operator =(
        pointer_type rhs) throw() {
    THE_STACK_TRACE;
    if (this->ptr != rhs) {
        if (rhs != NULL) {
            rhs->AddRef();
        }
        this->release();
        this->ptr = rhs;
    }
    return *this;
}


/*
 * the::system::com_ptr<T>::check_not_null
 */
template<class T> void the::system::com_ptr<T>::check_not_null(void) {
    THE_STACK_TRACE;
    if (this->ptr == NULL) {
        throw null_pointer_exception("ptr", __FILE__, __LINE__);
    }
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_COM_PTR_H_INCLUDED */
