/*
 * the/array_allocator.h
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
 * ArrayAllocator.h
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_ARRAY_ALLOCATOR_H_INCLUDED
#define THE_ARRAY_ALLOCATOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/memory.h"
#include "the/not_instantiable.h"
#include "the/types.h"


namespace the {


    /**
     * Allocates arrays of dynamic memory. 
     *
     * It uses the C++ array new and delete operators and therefore guarantees
     * that the default ctors and the dtors of the objects allocated are called
     * when necessary.
     */
    template<class T> class array_allocator : public not_instantiable {

    public:

        /** The type of the object handled by the allocator.*/
        typedef T target_type;

        /** The pointer type that is handled by the allocator. */
        typedef T *target_ptr_type;

        /**
         * Allocates 'cnt' contiguous objects of type T.
         *
         * @param cnt The number of elements to allocate.
         *
         * @return The pointer to the memory.
         *
         * @throws std::bad_alloc If the memory could not be allocated.
         */
        static inline target_ptr_type allocate(const size_t cnt) {
            return new T[cnt];
        }

        /**
         * Deallocate the memory designated by 'inOutPtr'.
         *
         * @param inOutPtr The pointer to the memory. The pointer will be set 
         *                 NULL when the method returns.
         */
        static inline void deallocate(target_ptr_type& inOutPtr) {
            delete[] inOutPtr;
            inOutPtr = NULL;
        }

        ///**
        // * Reallocate 'ptr' to be an array having 'cntNew' elements. The 
        // * overlapping part of the old array will remain in the new array, too.
        // *
        // * @param ptr    The pointer to be reallocated.
        // * @param cntOld The current size of the array designated by 'ptr'.
        // * @param cntNew The new size of the array.
        // *
        // * @return A pointer to the reallocated memory.
        // *
        // * @throws std::bad_alloc If the new memory could not be allocated.
        // */
        //static inline T *Reallocate(T *ptr, const size_t cntOld, 
        //		const size_t cntNew) {
        //	T *retval = new T[cnt];
        //	::memcpy(retval, ptr, (cntOld < cntNew) ? cntOld : cntNew);
        //	delete[] ptr;
        //	return retval;
        //}

    };
    
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_ARRAY_ALLOCATOR_H_INCLUDED */
