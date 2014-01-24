/*
 * the/collections/array_element_ptr_ctor.h
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
 * ArrayElementPtrCtor.h
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_COLLECTIONS_ARRAY_ELEMENT_PTR_CTOR_H_INCLUDED
#define THE_COLLECTIONS_ARRAY_ELEMENT_PTR_CTOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/force_inline.h"
#include "the/single_allocator.h"


namespace the {
namespace collections {


    /**
     * The ArrayElementPtrCtor class is a functor that allows the the::Array
     * class initialising arrays of pointers to T and deallocating these objects
     * using the specified allocator A.
     */
    template<class T, class A = single_allocator<T> >
    class array_element_ptr_ctor {

    public:

        /**
         * Call the default constructor of T on 'inOutAddress'.
         *
         * @param inOutAddress The address of the object to be constructed.
         */
        THE_FORCE_INLINE static void ctor(T **inOutAddress) {
            *inOutAddress = NULL;
        }

        /**
         * Deallocate the object that the pointer at address 'inOutAddress' 
         * designates using the allocator A and reset the pointer to NULL.
         *
         * @param inOutAddress The address of the object to be destructed.
         */
        THE_FORCE_INLINE static void dtor(T **inOutAddress) {
            A::deallocate(*inOutAddress);
            *inOutAddress = NULL;
        }

        /** Dtor. */
        ~array_element_ptr_ctor(void);

    private:

        /**
         * Disallow instances.
         */
        array_element_ptr_ctor(void);
    };


    /*
     * array_element_ptr_ctor<T, A>::~array_element_ptr_ctor
     */
    template<class T, class A>
    array_element_ptr_ctor<T, A>::~array_element_ptr_ctor(void) {
        // Nothing to do.
    }


    /*
     * array_element_ptr_ctor<T, A>::array_element_ptr_ctor
     */
    template<class T, class A>
    array_element_ptr_ctor<T, A>::array_element_ptr_ctor(void) {
        // Nothing to do.
    }

} /* end namespace collections */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_COLLECTIONS_ARRAYELEMENTPTRCTOR_H_INCLUDED */
