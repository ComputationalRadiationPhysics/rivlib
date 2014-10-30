/*
 * the/collections/ptr_array.h
 *
 * Copyright (c) 2012, TheLib Team (http://www.thelib.org/license)
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
/*
 * PtrArray.h
 *
 * Copyright (C) 2007 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2007 by Christoph Mueller. Alle Rechte vorbehalten.
 */

#ifndef THE_COLLECTIONS_PTR_ARRAY_H_INCLUDED
#define THE_COLLECTIONS_PTR_ARRAY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/collections/array.h"
#include "the/collections/array_element_ptr_ctor.h"


namespace the {
namespace collections {

    /**
     * This is a special array that holds pointers to objects of type T and 
     * takes ownership, i.e. if an array element is deleted the array will 
     * deallocate the object designated by this array element. The object will
     * be deleted using the allocator A. THE USER IS RESPONSIBLE FOR ONLY
     * STORING OBJECTS ON THE HEAP THAT CAN BE DEALLOCATED BY THE GIVEN
     * ALLOCATOR!
     *
     * For synchronisation of the PtrArray using the L template parameter,
     * read the documentation of the::Array.
     *
     * If you want to store pointers to single objects in the array, use 
     * single_allocator for A and allocate the objects using new. This is the 
     * default.
     *
     * If you want to store pointers to arrays of objects in the array, use
     * array_allocator for A and allocate the array using new[].
     */
    template<class T, class L = system::threading::no_lock,
        class A = single_allocator<T> >
    class ptr_array : public array<T *, L, array_element_ptr_ctor<T, A> > {

    protected:

        /** Immediate superclass. */
        typedef array<T *, L, array_element_ptr_ctor<T, A> > super;

    public:

        /** 
         * Create an array with the specified initial capacity.
         *
         * @param capacity The initial capacity of the array.
         */
        ptr_array(const size_t capacity = super::default_capacity);

        /**
         * Create a new array with the specified initial capacity and
         * use 'element' as default value for all elements.
         *
         * @param capacity The initial capacity of the array.
         * @param element  The default value to set.
         */
        inline ptr_array(const size_t capacity, const T& element)
                : super(capacity, element) {}

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        ptr_array(const ptr_array& rhs) : super(rhs) {}

        /** Dtor. */
        virtual ~ptr_array(void);
    };


    /*
     * the::ptr_array<T, L, A>::ptr_array
     */
    template<class T, class L , class A>
    ptr_array<T, L, A>::ptr_array(const size_t capacity) : super(capacity) {
        // Nothing to do. The constructor/destructor functor also works in
        // ctor and dtor as it is not dependent on the virtual table.
    }


    /*
     * the::ptr_array<T, L, A>::~ptr_array
     */
    template<class T, class L , class A> ptr_array<T, L, A>::~ptr_array(void) {
        // Nothing to do. The constructor/destructor functor also works in
        // ctor and dtor as it is not dependent on the virtual table.
    }

} /* end namespace collections */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_COLLECTIONS_PTR_ARRAY_H_INCLUDED */
