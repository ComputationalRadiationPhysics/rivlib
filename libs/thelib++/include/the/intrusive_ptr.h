/*
 * include\the\intrusive_ptr.h
 *
 * Copyright (C) 2013 TheLib Team (http://www.thelib.org/license)
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

#ifndef THE_INTRUSIVE_PTR_H_INCLUDED
#define THE_INTRUSIVE_PTR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/not_implemented_exception.h"
#include "the/stack_trace.h"


namespace the {

    /**
     * The intrusive_ptr class implements something a smart pointer, but
     * relies on the objects that it points to for reference counting. It
     * therefore does not need to allocate a proxy object. However, 
     * intrusive_ptrs are less safe than smart pointers as they are prone to
     * dangling references and manipluation and they require objects pointed to
     * to derive from reference_counted (or equivalent).
     *
     * The intrusive_ptr class takes care of incrementing and decrementing
     * (using add_ref() and release()) the reference count of designated objects
     * as intrusive_ptr objects are created, manipulated and deleted. The
     * designated objects are responsible for deleting themselves as the
     * reference count reaches zero. reference_counted implements this
     * behaviour.
     *
     * Any pointer of a reference_counted object that is flat and no
     * intrusive_ptr can be a "weak reference", i.e. a reference that is not
     * counted. However, flat references are not necessarily weak references,
     * as the user can increment an decrement the reference counter manually.
     *
     * @tparam T The type that the pointer points to.
     */
    template<class T> class intrusive_ptr {

    public:

        /** The type of the allocator that T uses to delete itself. */
        typedef typename T::allocator_type allocator_type;

        /** The constant type of a native pointer. */
        typedef const T *const_pointer_type;

        /** A constant reference to the underlying object. */
        typedef const T& const_reference_type;

        /** The type of a native pointer. */
        typedef T *pointer_type;

        /** A reference to the underlying object */
        typedef T& reference_type;

        /** 
         * Create a reference pointing to 'obj'.
         *
         * @param obj    The object to be referenced.
         * @param addRef Determines whether the ctor will increment the 
         *               reference count (default) or not.
         */
        intrusive_ptr(pointer_type obj = nullptr, const bool addRef = true);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline intrusive_ptr(const intrusive_ptr& rhs) : obj(nullptr) {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /** Dtor. */
        virtual ~intrusive_ptr(void);

        /**
         * Get the pointer designated by the reference and dynamic cast it 
         * to 'Tp'. The reference count of the object is not changed by this
         * operation. Callers must not manipulate the object returned, 
         * especially they must never delete it!
         *
         * @return A pointer of type 'Tp' to the object. If the dynamic_cast
         *         fails, this might be NULL even if the object is not NULL.
         */
        template<class Tp> inline Tp *dynamic_peek(void) {
            THE_STACK_TRACE;
            return dynamic_cast<Tp *>(this->obj);
        }

        /**
         * Get the pointer designated by the reference and dynamic cast it 
         * to 'Tp'. The reference count of the object is not changed by this
         * operation. Callers must not manipulate the object returned, 
         * especially they must never delete it!
         *
         * @return A pointer of type 'Tp' to the object. If the dynamic_cast
         *         fails, this might be NULL even if the object is not NULL.
         */
        template<class Tp> inline const Tp *dynamic_peek(void) const {
            THE_STACK_TRACE;
            return dynamic_cast<const Tp *>(this->obj);
        }

        /**
         * Answer, whether the intrusive_ptr is not nullptr.
         *
         * @return true, if the reference is not nullptr, false otherwise.
         */
        inline bool is_not_null(void) const throw() {
            THE_STACK_TRACE;
            return (this->obj != nullptr);
        }

        /**
         * Answer, whether the intrusive_ptr is nullptr.
         *
         * @return true, if the reference is nullptr, false otherwise.
         */
        inline bool is_null(void) const throw() {
            THE_STACK_TRACE;
            return (this->obj == nullptr);
        }

        /**
         * Release the reference and make the pointer NULL.
         */
        void release(void) throw();

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline intrusive_ptr& operator =(const intrusive_ptr& rhs) {
            THE_STACK_TRACE;
            return (*this = rhs.obj);
        }

        /**
         * Assignment operator.
         *
         * This operation creates a strong reference, i.e. decrements the
         * reference count of the old object and increments the reference count
         * of the new one.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        intrusive_ptr& operator =(pointer_type rhs);

        /**
         * Answer, whether two intrusive_ptrs are equal, i.e. designate the
         * same object.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are equal, false otherwise.
         */
        inline bool operator ==(const intrusive_ptr rhs) const throw() {
            THE_STACK_TRACE;
            return (this->obj == rhs.obj);
        }

        /**
         * Answer, whether two intrusive_ptrs are not equal, i.e. designate 
         * different objects.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are not equal, false otherwise.
         */
        inline bool operator !=(const intrusive_ptr& rhs) const throw() {
            THE_STACK_TRACE;
            return (this->obj != rhs.obj);
        }

        /**
         * Dereferences the intrusive_ptrs. 
         *
         * No measurements against dereferencing NULL pointers are taken.
         *
         * @return A reference to the object designated by the reference.
         */
        inline reference_type operator *(void) {
            THE_STACK_TRACE;
            return *(this->obj);
        }

        /**
         * Dereferences the intrusive_ptrs. 
         *
         * No measurements against dereferencing NULL pointers are taken.
         *
         * @return A reference to the object designated by the reference.
         */
        inline const_reference_type operator *(void) const {
            THE_STACK_TRACE;
            return *(this->obj);
        }

        /**
         * Member access.
         * 
         * The member access operation creates a weak reference to the 
         * referenced object, i.e. the reference count is not modified.
         *
         * @return A pointer to the object designated by the smart reference.
         */
        inline pointer_type operator ->(void) throw() {
            THE_STACK_TRACE;
            return this->obj;
        }

        /**
         * Member access. 
         * 
         * The member access operation creates a weak reference to the 
         * referenced object, i.e. the reference count is not modified.
         *
         * @return A pointer to the object designated by the smart reference.
         */
        inline const_pointer_type operator ->(void) const throw() {
            THE_STACK_TRACE;
            return this->obj;
        }

    private:

        /** The referenced object. */
        pointer_type obj;

    };

    /**
     * Performs a dynamic cast between two intrusive_ptrs.
     *
     * @param ptr The pointer to be casted.
     *
     * @return The pointer 'ptr' dynamically casted to type 'O'.
     *
     * @tparam O The target type.
     * @tparam I The source type.
     */
    template<class O, class I>
    inline intrusive_ptr<O> dynamic_pointer_cast(
            intrusive_ptr<I>& ptr) throw() {
        THE_STACK_TRACE;
        return intrusive_ptr<O>(dynamic_cast<O *>(ptr.operator ->()));
    }

} /* end namespace the */


/*
 * the::intrusive_ptr<T>::intrusive_ptr
 */
template<class T>
the::intrusive_ptr<T>::intrusive_ptr(pointer_type obj, const bool addRef)
        : obj(obj) {
    THE_STACK_TRACE;
    if (addRef && (this->obj != nullptr)) {
        this->obj->add_ref();
    }
}


/*
 * the::intrusive_ptr<T>::~intrusive_ptr
 */
template<class T>
the::intrusive_ptr<T>::~intrusive_ptr(void) {
    THE_STACK_TRACE;
    this->release();
}


/*
 * the::intrusive_ptr<T>::release
 */
template<class T> void the::intrusive_ptr<T>::release(void) throw() {
    THE_STACK_TRACE;
    if (!this->is_null()) {
        this->obj->release();
        this->obj = nullptr;
    }
}


/*
 * the::intrusive_ptr<T>::operator =
 */
template<class T>
the::intrusive_ptr<T>& the::intrusive_ptr<T>::operator =(pointer_type rhs) {
    THE_STACK_TRACE;
    if (this->obj != rhs) {
        if (rhs != nullptr) {
            rhs->add_ref();
        }
        this->release();
        this->obj = rhs;
    }
    return *this;
}


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_INTRUSIVE_PTR_H_INCLUDED */
