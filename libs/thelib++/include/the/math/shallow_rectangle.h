/*
 * the/math/shallow_rectangle.h
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
 * ShallowRectangle.h  27.09.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_SHALLOW_RECTANGLE_H_INCLUDED
#define THE_MATH_SHALLOW_RECTANGLE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"
#include "the/math/abstract_rectangle.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * This class represents a shallow rectangle, that uses memory provided
     * by the caller.
     */
    template<class T> class shallow_rectangle 
            : public abstract_rectangle<T, T *> {

    public:

        /**
         * Construct a rectangle from an array holding its bounds. The array
         * 'bounds' holds in this order to following borders of the rectangle:
         * left, bottom, right, top.
         *
         * @param bounds The left, bottom, right and top border of the 
         *               rectangle in a consecutive order.
         */
        explicit inline shallow_rectangle(T *bounds) {
            THE_STACK_TRACE;
            this->bounds = bounds;
        }

        /**
         * Copy ctor. This ctor creates an alias!
         *
         * @param rhs The object to clone.
         */
        inline shallow_rectangle(const shallow_rectangle& rhs) {
            THE_STACK_TRACE;
            this->bounds = rhs.bounds;
        }

        /** Dtor. */
        ~shallow_rectangle(void);

        /**
         * Assigment operator. This operator never creates an alias, even for
         * shallow rectangles!
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline shallow_rectangle& operator =(const shallow_rectangle& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * Assigment operator. This operator never creates an alias, even for
         * shallow rectangles!
         *
         * This assignment allows for arbitrary rectangle to rectangle
         * conversions.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Tp, class Sp>
        inline shallow_rectangle& operator =(
                const abstract_rectangle<Tp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    public:

        /** Typedef for the super class. */
        typedef abstract_rectangle<T, T *> super;
    };


    /*
     * the::math::shallow_rectangle<T>::~shallow_rectangle
     */
    template<class T> shallow_rectangle<T>::~shallow_rectangle(void) {
        THE_STACK_TRACE;
    }


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_SHALLOW_RECTANGLE_H_INCLUDED */
