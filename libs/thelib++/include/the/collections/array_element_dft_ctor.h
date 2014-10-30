/*
 * the/collections/array_element_dft_ctor.h
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
 * ArrayElementDftCtor.h
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_COLLECTIONS_ARRAY_ELEMENT_DFT_CTOR_H_INCLUDED
#define THE_COLLECTIONS_ARRAY_ELEMENT_DFT_CTOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/force_inline.h"


namespace the {
namespace collections {


    /**
     * The ArrayElementDftCtor class is a functor that allows the the::Array
     * class constructing objects of the template type T that have been 
     * allocated typelessly.
     */
    template<class T> class array_element_dft_ctor {

    public:

        /**
         * Call the default constructor of T on 'inOutAddress'.
         *
         * @param inOutAddress The address of the object to be constructed.
         */
        THE_FORCE_INLINE static void ctor(T *inOutAddress) {
            new (inOutAddress) T;
        }

        /**
         * Call the destructor of T on the object at 'inOutAddress'.
         *
         * @param inOutAddress The address of the object to be destructed.
         */
        THE_FORCE_INLINE static void dtor(T *inOutAddress) {
            inOutAddress->~T();
        }

        /** Dtor. */
        ~array_element_dft_ctor(void);

    private:

        /**
         * Disallow instances.
         */
        array_element_dft_ctor(void);
    };


    /*
     * array_element_dft_ctor<T>::~array_element_dft_ctor
     */
    template<class T>
    array_element_dft_ctor<T>::~array_element_dft_ctor(void) {
        // Nothing to do.
    }


    /*
     * array_element_dft_ctor<T>::array_element_dft_ctor
     */
    template<class T>
    array_element_dft_ctor<T>::array_element_dft_ctor(void) {
        // Nothing to do.
    }

} /* end namespace collections */
} /* end namespace vislib */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_COLLECTIONS_ARRAY_ELEMENT_DFT_CTOR_H_INCLUDED */
