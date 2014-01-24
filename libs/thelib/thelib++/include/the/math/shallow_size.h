/*
 * the/math/shallow_size.h
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
 * Shallowext.h
 *
 * Copyright (C) 2008 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2008 by Christoph M�ller. Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_SHALLOW_SIZE_H_INCLUDED
#define THE_MATH_SHALLOW_SIZE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/math/abstract_size.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * This class represents a shallow size, that uses memory provided
     * by the caller.
     */
    template<class T, unsigned int D> 
    class shallow_size : public abstract_size<T, D, T *> {

    public:

        /**
         * Construct a ext from an array holding its values. The array
         * 'ext' holds the size in the following order: width, height,
         * higher exts ...
         *
         * @param ext The ext values.
         */
        explicit inline shallow_size(T *ext) {
            THE_STACK_TRACE;
            ASSERT(ext != NULL);
            this->ext = ext;
        }

        /**
         * Copy ctor. This ctor creates an alias!
         *
         * @param rhs The object to clone.
         */
        inline shallow_size(const shallow_size& rhs) {
            THE_STACK_TRACE;
            this->ext = rhs.ext;
        }

        /** Dtor. */
        ~shallow_size(void);

        /**
         * Assigment operator. This operator never creates an alias, even for
         * shallow exts!
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline shallow_size& operator =(const shallow_size& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * Assigment operator. This operator never creates an alias, even for
         * shallow exts!
         *
         * This assignment allows for arbitrary ext to ext
         * conversions.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline shallow_size& operator =(
                const abstract_size<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    public:

        /** Typedef for the super class. */
        typedef abstract_size<T, D, T *> super;
    };


    /*
     * the::math::shallow_size<T, D>::~shallow_size
     */
    template<class T, unsigned int D> 
    shallow_size<T, D>::~shallow_size(void) {
        THE_STACK_TRACE;
    }


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_SHALLOW_SIZE_H_INCLUDED */
