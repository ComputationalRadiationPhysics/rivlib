/*
 * the/single_allocator.h
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
 * SingleAllocator.h
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_SINGLEALLOCATOR_H_INCLUDED
#define THE_SINGLEALLOCATOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/memory.h"
#include "the/not_instantiable.h"


namespace the {


    /**
     * This class creates typed memory for a single object of the template type.
     * It therefore cannot be used for allocating continuous arrays.
     *
     * The allocator uses the C++ allocation and deallocation mechanisms and 
     * therefore guarantees that the default ctor is called on the newly
     * allocated object and that the dtor is called before deallocating an
     * object.
     */
    template<class T> class single_allocator : public not_instantiable {

    public:

        /** The type of the object handled by the allocator.*/
        typedef T target_type;

        /** The pointer type that is handled by the allocator. */
        typedef T *target_ptr_type;

        /**
         * Allocate an object of type T.
         *
         * @return A pointer to the newly allocated object.
         *
         * @throws std::bad_alloc If there was not enough memory to allocate the
         *                        object.
         */
        static inline target_ptr_type allocate(void) {
            return new T;
        }

        /**
         * Deallocate 'ptr' and set it NULL.
         *
         * @param inOutPtr The pointer to be deallocated. The pointer will be 
         *                 set NULL before the method returns.
         */
        static inline void deallocate(target_ptr_type& inOutPtr) {
            delete inOutPtr;
            inOutPtr = NULL;
        }

    };
    
} /* end namespace the */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SINGLEALLOCATOR_H_INCLUDED */
