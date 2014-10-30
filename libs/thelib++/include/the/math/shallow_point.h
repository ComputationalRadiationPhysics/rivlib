/*
 * the/math/shallow_point.h
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
 * ShallowPoint.h
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_SHALLOW_POINT_H_INCLUDED
#define THE_MATH_SHALLOW_POINT_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/config.h"
#include "the/math/abstract_point.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * TODO: Documentation
     */
    template<class T, unsigned int D> 
    class shallow_point : public abstract_point<T, D, T *> {

    public:

        /**
         * Create a new Point initialised using 'coordinates' as data. The
         * Point will operate on these data. The caller is responsible that
         * the memory designated by 'components' lives as long as the object
         * and all its aliases exist.
         *
         * @param coordinates The initial Point memory. This must not be a NULL
         *                    pointer.
         */
        explicit inline shallow_point(T *coordinates) {
            THE_STACK_TRACE;
            assert(coordinates != NULL);
            this->coordinates = coordinates;
        }

        /**
         * Clone 'rhs'. This operation will create an alias of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline shallow_point(const shallow_point& rhs) {
            THE_STACK_TRACE;
            this->coordinates = rhs.coordinates;
        }

        /** Dtor. */
        ~shallow_point(void);

        /**
         * Replace the coordinate pointer with a new memory location.
         * The original memory is left untouched.
         * 
         * @param coordinates The new Point memory. This must not be a NULL
         *                    pointer.
         */
        inline void set_pointer(T *coordinates) {
            THE_STACK_TRACE;
            assert(coordinates != NULL);
            this->coordinates = coordinates;
        }

        /**
         * Assignment.
         *
         * This operation does <b>not</b> create aliases. 
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        inline shallow_point& operator =(const shallow_point& rhs) {
            THE_STACK_TRACE;
            abstract_point<T, D, T *>::operator =(rhs);
            return *this;
        }

        /**
         * Assignment for arbitrary Points. A valid static_cast between T and Tp
         * is a precondition for instantiating this template.
         *
         * This operation does <b>not</b> create aliases. 
         *
         * If the two operands have different dimensions, the behaviour is as 
         * follows: If the left hand side operand has lower dimension, the 
         * highest (Dp - D) dimensions are discarded. If the left hand side
         * operand has higher dimension, the missing dimensions are filled with 
         * zero components.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline shallow_point& operator =(
                const abstract_point<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            abstract_point<T, D, T *>::operator =(rhs);
            return *this;
        }

    private:

        /** 
         * Forbidden ctor. A default ctor would be inherently unsafe for
         * shallow Points.
         */
        inline shallow_point(void) {}
    };


    /*
     * shallow_point<T, D>::~shallow_point
     */
    template<class T, unsigned int D>
    shallow_point<T, D>::~shallow_point(void) {
        THE_STACK_TRACE;
    }

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_SHALLOW_POINT_H_INCLUDED */
