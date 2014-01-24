/*
 * the/math/abstract_point.h
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
 * AbstractPoint.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_POINT_H_INCLUDED
#define THE_MATH_ABSTRACT_POINT_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/config.h"
#include "the/math/abstract_point_impl.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * TODO: documentation
     */
    template<class T, unsigned int D, class S> class abstract_point 
            : public abstract_point_impl<T, D, S, abstract_point> {

    public:

        /** Dtor. */
        ~abstract_point(void);

        /**
         * Assignment.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        inline abstract_point& operator =(const abstract_point& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * Assigment for arbitrary vectors. A valid static_cast between T and Tp
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
        inline abstract_point& operator =(
                const abstract_point<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** Typedef for our super class. */
        typedef abstract_point_impl<T, D, S, the::math::abstract_point> super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_point(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_point_impl;
    };


    /*
     * the::math::abstract_point<T, D, S>::~abstract_point
     */
    template<class T, unsigned int D, class S>
    abstract_point<T, D, S>::~abstract_point(void) {
        THE_STACK_TRACE;
    }


    /**
     * Partial template specialisation for two-dimensional points. This 
     * implementation provides convenience access methods to the two 
     * coordinates.
     */
    template<class T, class S> class abstract_point<T, 2, S> 
            : public abstract_point_impl<T, 2, S, abstract_point> {

    public:

        /** Behaves like primary class template. */
        ~abstract_point(void);

        /**
         * Answer the x-coordinate of the point.
         *
         * @return The x-coordinate of the point.
         */
        inline const T& get_x(void) const {
            THE_STACK_TRACE;
            return this->coordinates[0];
        }

        /**
         * Answer the y-coordinate of the point.
         *
         * @return The y-coordinate of the point.
         */
        inline const T& get_y(void) const {
            THE_STACK_TRACE;
            return this->coordinates[1];
        }

        /**
         * set the coordinates ot the point.
         *
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         */
        inline void set(const T& x, const T& y) {
            THE_STACK_TRACE;
            this->coordinates[0] = x;
            this->coordinates[1] = y;
        }

        /**
         * set the x-coordinate of the point.
         *
         * @param x The new x-coordinate.
         */
        inline void set_x(const T& x) {
            THE_STACK_TRACE;
            this->coordinates[0] = x;
        }

        /**
         * set the y-coordinate of the point.
         *
         * @param y The new y-coordinate.
         */
        inline void set_y(const T& y) {
            THE_STACK_TRACE;
            this->coordinates[1] = y;
        }

        /**
         * Answer the x-coordinate of the point.
         *
         * @return The x-coordinate of the point.
         */
        inline const T& x(void) const {
            THE_STACK_TRACE;
            return this->coordinates[0];
        }

        /**
         * Answer the y-component of the point.
         *
         * @return The y-component of the point.
         */
        inline const T& y(void) const {
            THE_STACK_TRACE;
            return this->coordinates[1];
        }

        /** Behaves like primary class template. */
        inline abstract_point& operator =(const abstract_point& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_point& operator =(
                const abstract_point<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }


    protected:

        /** Typedef for our super class. */
        typedef abstract_point_impl<T, 2, S, the::math::abstract_point> super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_point(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_point_impl;
    };


    /*
     * the::math::abstract_point<T, 2, S>::~abstract_point
     */
    template<class T, class S> abstract_point<T, 2, S>::~abstract_point(void) {
        THE_STACK_TRACE;
    }


    /**
     * Partial template specialisation for three-dimensional points. This 
     * implementation provides convenience access methods to the three 
     * coordinates.
     */
    template<class T, class S> class abstract_point<T, 3, S> 
            : public abstract_point_impl<T, 3, S, abstract_point> {

    public:

        /** Behaves like primary class template. */
        ~abstract_point(void);

        /**
         * Answer the x-coordinate of the point.
         *
         * @return The x-coordinate of the point.
         */
        inline const T& get_x(void) const {
            THE_STACK_TRACE;
            return this->coordinates[0];
        }

        /**
         * Answer the y-coordinate of the point.
         *
         * @return The y-coordinate of the point.
         */
        inline const T& get_y(void) const {
            THE_STACK_TRACE;
            return this->coordinates[1];
        }

        /**
         * Answer the z-coordinate of the point.
         *
         * @return The z-coordinate of the point.
         */
        inline const T& get_z(void) const {
            THE_STACK_TRACE;
            return this->coordinates[2];
        }

        /**
         * set the coordinates ot the point.
         *
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         * @param z The z-coordinate of the point.
         */
        inline void set(const T& x, const T& y, const T& z) {
            THE_STACK_TRACE;
            this->coordinates[0] = x;
            this->coordinates[1] = y;
            this->coordinates[2] = z;
        }

        /**
         * set the x-coordinate of the point.
         *
         * @param x The new x-coordinate.
         */
        inline void set_x(const T& x) {
            THE_STACK_TRACE;
            this->coordinates[0] = x;
        }

        /**
         * set the y-coordinate of the point.
         *
         * @param y The new y-coordinate.
         */
        inline void set_y(const T& y) {
            THE_STACK_TRACE;
            this->coordinates[1] = y;
        }

        /**
         * set the z-coordinate of the point.
         *
         * @param z The new z-coordinate.
         */
        inline void set_z(const T& z) {
            THE_STACK_TRACE;
            this->coordinates[2] = z;
        }

        /**
         * Answer the x-coordinate of the point.
         *
         * @return The x-coordinate of the point.
         */
        inline const T& x(void) const {
            THE_STACK_TRACE;
            return this->coordinates[0];
        }

        /**
         * Answer the y-coordinate of the point.
         *
         * @return The y-coordinate of the point.
         */
        inline const T& y(void) const {
            THE_STACK_TRACE;
            return this->coordinates[1];
        }

        /**
         * Answer the z-coordinate of the point.
         *
         * @return The z-coordinate of the point.
         */
        inline const T& z(void) const {
            THE_STACK_TRACE;
            return this->coordinates[2];
        }

        /** Behaves like primary class template. */
        inline abstract_point& operator =(const abstract_point& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_point& operator =(
                const abstract_point<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** Typedef for our super class. */
        typedef abstract_point_impl<T, 3, S, the::math::abstract_point> super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_point(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_point_impl;
    };


    /*
     * the::math::abstract_point<T, 3, S>::~abstract_point
     */
    template<class T, class S> abstract_point<T, 3, S>::~abstract_point(void) {
        THE_STACK_TRACE;
    }


    /**
     * Partial template specialisation for four-dimensional points. This 
     * implementation provides convenience access methods to the four 
     * coordinates.
     */
    template<class T, class S> class abstract_point<T, 4, S> 
            : public abstract_point_impl<T, 4, S, abstract_point> {

    public:

        /** Behaves like primary class template. */
        ~abstract_point(void);

        /**
         * Answer the x-coordinate of the point.
         *
         * @return The x-coordinate of the point.
         */
        inline const T& get_x(void) const {
            THE_STACK_TRACE;
            return this->coordinates[0];
        }

        /**
         * Answer the y-coordinate of the point.
         *
         * @return The y-coordinate of the point.
         */
        inline const T& get_y(void) const {
            THE_STACK_TRACE;
            return this->coordinates[1];
        }

        /**
         * Answer the z-coordinate of the point.
         *
         * @return The z-coordinate of the point.
         */
        inline const T& get_z(void) const {
            THE_STACK_TRACE;
            return this->coordinates[2];
        }

        /**
         * Answer the w-coordinate of the point.
         *
         * @return The w-coordinate of the point.
         */
        inline const T& get_w(void) const {
            THE_STACK_TRACE;
            return this->coordinates[3];
        }

        /**
         * set the coordinates ot the point.
         *
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         * @param z The z-coordinate of the point.
         * @param w The w-coordinate of the point.
         */
        inline void set(const T& x, const T& y, const T& z, const T& w) {
            THE_STACK_TRACE;
            this->coordinates[0] = x;
            this->coordinates[1] = y;
            this->coordinates[2] = z;
            this->coordinates[3] = w;
        }

        /**
         * set the x-coordinate of the point.
         *
         * @param x The new x-coordinate.
         */
        inline void set_x(const T& x) {
            THE_STACK_TRACE;
            this->coordinates[0] = x;
        }

        /**
         * set the y-coordinate of the point.
         *
         * @param y The new y-coordinate.
         */
        inline void set_y(const T& y) {
            THE_STACK_TRACE;
            this->coordinates[1] = y;
        }

        /**
         * set the z-coordinate of the point.
         *
         * @param z The new z-coordinate.
         */
        inline void set_z(const T& z) {
            THE_STACK_TRACE;
            this->coordinates[2] = z;
        }

        /**
         * set the w-coordinate of the point.
         *
         * @param w The new w-coordinate.
         */
        inline void set_w(const T& w) {
            THE_STACK_TRACE;
            this->coordinates[3] = w;
        }

        /**
         * Answer the x-coordinate of the point.
         *
         * @return The x-coordinate of the point.
         */
        inline const T& x(void) const {
            THE_STACK_TRACE;
            return this->coordinates[0];
        }

        /**
         * Answer the y-coordinate of the point.
         *
         * @return The y-coordinate of the point.
         */
        inline const T& y(void) const {
            THE_STACK_TRACE;
            return this->coordinates[1];
        }

        /**
         * Answer the z-coordinate of the point.
         *
         * @return The z-coordinate of the point.
         */
        inline const T& z(void) const {
            THE_STACK_TRACE;
            return this->coordinates[2];
        }

        /**
         * Answer the w-coordinate of the point.
         *
         * @return The w-coordinate of the point.
         */
        inline const T& w(void) const {
            THE_STACK_TRACE;
            return this->coordinates[3];
        }

        /** Behaves like primary class template. */
        inline abstract_point& operator =(const abstract_point& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_point& operator =(
                const abstract_point<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** Typedef for our super class. */
        typedef abstract_point_impl<T, 4, S, the::math::abstract_point> super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_point(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_point_impl;
    };


    /*
     * the::math::abstract_point<T, 4, S>::~abstract_point
     */
    template<class T, class S> abstract_point<T, 4, S>::~abstract_point(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_POINT_H_INCLUDED */
