/*
 * the/math/shallow_quaternion.h
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
 * ShallowQuaternion.h  28.09.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_SHALLOW_QUATERNION_H_INCLUDED
#define THE_MATH_SHALLOW_QUATERNION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/math/abstract_quaternion.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * A quaternion.
     */
    template<class T> class shallow_quaternion : public abstract_quaternion<T, T*> {

    public:


        /**
         * Create a new quaternion using the memory designated by 'components'
         * as data. 'components' must hold the x, y, z and w component of the
         * quaternion in consecutive order. The caller remains owner of the 
         * memory, but must ensure that it lives at least as long as the
         * object.
         *
         * @param components The components of the quaternion.
         */
        inline explicit shallow_quaternion(T *components) {
            THE_STACK_TRACE;
            this->components = components;
        }


        /**
         * Clone 'rhs'. This operation creates an alias of 'rhs'
         *
         * @param rhs The object to be copied.
         */
        inline shallow_quaternion(const shallow_quaternion& rhs) {
            THE_STACK_TRACE;
            this->components = rhs.components;
        }

        /** Dtor. */
        ~shallow_quaternion(void);

        /**
         * Assignment.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline shallow_quaternion& operator =(const shallow_quaternion& rhs) {
            THE_STACK_TRACE;
            abstract_quaternion<T, T[4]>::operator =(rhs);
            return *this;
        }

        /**
         * Assignment. This operator allows arbitrary quaternion to
         * quaternion conversions.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Tp, class Sp>
        inline shallow_quaternion& operator =(
                const abstract_quaternion<Tp, Sp>& rhs) {
            THE_STACK_TRACE;
            abstract_quaternion<T, T[4]>::operator =(rhs);
            return *this;
        }
    };


    /*
     * the::math::shallow_quaternion<T>::~shallow_quaternion
     */
    template<class T>
    shallow_quaternion<T>::~shallow_quaternion(void) {
        THE_STACK_TRACE;
    }


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_SHALLOW_QUATERNION_H_INCLUDED */
