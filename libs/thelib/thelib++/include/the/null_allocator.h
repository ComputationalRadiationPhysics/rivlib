/*
 * the/null_allocator.h
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
 * NullAllocator.h
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_NULL_ALLOCATOR_H_INCLUDED
#define THE_NULL_ALLOCATOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/not_instantiable.h"
#include "the/types.h"


namespace the {


    /**
     * This is an allocator class to be used in collections or SmartPtr classes
     * which does not allocate or deallocate any memory. Instead the allocation
     * methods always return 'NULL' and the deallocation methods only set the
     * incoming pointers to 'NULL'. This class is fully compatible with the
     * 'single_allocator' and the 'array_allocator'.
     *
     * One use of this class is a 'SmartPtr' pointing to an object placed on
     * the global application stack.
     */
    template<class T> class null_allocator : public not_instantiable {

    public:

        /** The type of the object handled by the allocator.*/
        typedef T target_type;

        /** The pointer type that is handled by the allocator. */
        typedef T *target_ptr_type;

        /**
         * Allways returns 'NULL'.
         *
         * @param cnt Ignored.
         *
         * @return 'NULL'
         */
        static inline target_ptr_type allocate(const size_t cnt = 0) {
            return NULL;
        }

        /**
         * Sets the pointer 'inOutPtr' to 'NULL'.
         *
         * @param inOutPtr The pointer will be set NULL.
         */
        static inline void deallocate(target_ptr_type& inOutPtr) {
            inOutPtr = NULL;
        }

    };

} /* end namespace the */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_NULL_ALLOCATOR_H_INCLUDED */
