/*
 * the/math/rectangle.h
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
 * Rectangle.h  27.09.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_RECTANGLE_H_INCLUDED
#define THE_MATH_RECTANGLE_H_INCLUDED
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
     * This class represents a rectangle.
     */
    template<class T> class rectangle : public abstract_rectangle<T, T[4]> {

    public:

        /** An empty rectangle in the origin. */
        static const rectangle empty;

        /**
         * Create an empty rectangle in the origin.
         */ 
        inline rectangle(void) {
            THE_STACK_TRACE;
            this->bounds[0] = this->bounds[1] = this->bounds[2] 
                = this->bounds[3] = static_cast<T>(0);
        }

        /**
         * Create a new rectangle.
         *
         * @param left   The left border of the rectangle.
         * @param bottom The bottom border of the rectangle.
         * @param right  The right border of the rectangle.
         * @param top    The top border of the rectangle.
         */
        inline rectangle(const T& left, const T& bottom, const T& right, 
                const T& top) {
            THE_STACK_TRACE;
            this->bounds[super::IDx_LEFT] = left;
            this->bounds[super::IDx_BOTTOM] = bottom;
            this->bounds[super::IDx_RIGHT] = right;
            this->bounds[super::IDx_TOP] = top;
        }

        /**
         * Construct a rectangle from an array holding its bounds. The array
         * 'bounds' holds in this order to following borders of the rectangle:
         * left, bottom, right, top.
         *
         * @return The left, bottom, right and top border of the rectangle in
         *         a consecutive order.
         */
        explicit inline rectangle(const T *bounds) {
            THE_STACK_TRACE;
            ::memcpy(this->bounds, bounds, 4 * sizeof(T));
        }

        /**
         * Construct a rectangle with a specific origin and dimension.
         *
         * @param origin The origin (left/bottom point) of the rectangle.
         * @param size   The dimensions of the rectangle.
         */
        template<class Tp1, class Sp1, class Tp2, class Sp2>
        inline rectangle(const abstract_point<Tp1, 2, Sp1>& origin, 
                const abstract_size<Tp2, 2, Sp2>& size) {
            THE_STACK_TRACE;
            this->bounds[super::IDx_LEFT] = this->bounds[super::IDx_RIGHT] 
                = origin.x();
            this->bounds[super::IDx_BOTTOM] = this->bounds[super::IDx_TOP] 
                = origin.y();
            this->bounds[super::IDx_RIGHT] += size.width();
            this->bounds[super::IDx_TOP] += size.height();
        }

        /**
         * Copy ctor.
         *
         * @param rhs The object to clone.
         */
        inline rectangle(const rectangle& rhs) {
            THE_STACK_TRACE;
            ::memcpy(this->bounds, rhs.bounds, 4 * sizeof(T));
        }

        /**
         * Allow arbitrary rectangle to rectangle conversions.
         *
         * @param rhs The object to clone.
         */
        template<class Tp, class Sp>
        inline rectangle(const abstract_rectangle<Tp, Sp>& rhs) {
            THE_STACK_TRACE;
            this->bounds[super::IDx_BOTTOM] = static_cast<T>(rhs.bottom());
            this->bounds[super::IDx_LEFT] = static_cast<T>(rhs.left());
            this->bounds[super::IDx_RIGHT] = static_cast<T>(rhs.right());
            this->bounds[super::IDx_TOP] = static_cast<T>(rhs.top());
        }

        /** Dtor. */
        ~rectangle(void);

        /**
         * Assigment operator. This operator never creates an alias, even for
         * shallow rectangles!
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline rectangle& operator =(const rectangle& rhs) {
            THE_STACK_TRACE;
            abstract_rectangle<T, T[4]>::operator =(rhs);
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
        inline rectangle& operator =(const abstract_rectangle<Tp, Sp>& rhs) {
            THE_STACK_TRACE;
            abstract_rectangle<T, T[4]>::operator =(rhs);
            return *this;
        }

    private:

        /** super class typedef. */
        typedef abstract_rectangle<T, T[4]> super;
    };


    /*
     * the::math::rectangle<T>::rectangle<T>::empty
     */
    template<class T> const rectangle<T> rectangle<T>::empty(0, 0, 0, 0);

    /*
     * the::math::rectangle<T>::~rectangle
     */
    template<class T> rectangle<T>::~rectangle(void) {
        THE_STACK_TRACE;
    }

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_RECTANGLE_H_INCLUDED */
