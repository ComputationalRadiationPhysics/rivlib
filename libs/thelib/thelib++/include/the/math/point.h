/*
 * the/math/point.h
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
 * Point.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller. Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_POINT_H_INCLUDED
#define THE_MATH_POINT_H_INCLUDED
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
     * This is the implementation of an abstract_point that uses its own memory 
     * in a statically allocated array of dimension D. Usually, you want to use
     * this point class or derived classes.
     *
     * See documentation of abstract_point for further information about the 
     * vector classes.
     */
    template<class T, unsigned int D> 
    class point : public abstract_point<T, D, T[D]> {

    public:

        /**
         * Create a point in the coordinate origin.
         */
        point(void);

        /**
         * Create a new point initialised with 'coordinates'. 'coordinates' must
         * not be a NULL pointer. 
         *
         * @param coordinates The initial coordinates of the point.
         */
        explicit inline point(const T *coordinates) : super() {
            THE_STACK_TRACE;
            ASSERT(coordinates != NULL);
            ::memcpy(this->coordinates, coordinates, D * sizeof(T));
        }

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline point(const point& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->coordinates, rhs.coordinates, D * sizeof(T));
        }

        /**
         * Create a copy of 'rhs'. This ctor allows for arbitrary point to
         * point conversions.
         *
         * @param rhs The vector to be cloned.
         */
        template<class Tp, unsigned int Dp, class Sp>
        point(const abstract_point<Tp, Dp, Sp>& rhs);

        /** Dtor. */
        ~point(void);

        /**
         * Assignment.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        inline point& operator =(const point& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * Assigment for arbitrary points. A valid static_cast between T and Tp
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
         * Subclasses must ensure that sufficient memory for the 'coordinates'
         * member has been allocated before calling this operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline point& operator =(const abstract_point<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    private:

        /** super class typedef. */
        typedef abstract_point<T, D, T[D]> super;
    };


    /*
     * the::math::point<T, D>::point
     */
    template<class T, unsigned int D>
    point<T, D>::point(void) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->coordinates[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::point<T, D>::point
     */
    template<class T, unsigned int D>
    template<class Tp, unsigned int Dp, class Sp>
    point<T, D>::point(const abstract_point<Tp, Dp, Sp>& rhs) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->coordinates[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->coordinates[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::point<T, D>::~point
     */
    template<class T, unsigned int D>
    point<T, D>::~point(void) {
        THE_STACK_TRACE;
    }


    /**
     * Partial template specialisation for two-dimensional point. This class
     * provides a constructor with separate components.
     */
    template<class T> 
    class point<T, 2> : public abstract_point<T, 2, T[2]> {

    public:

        /** Behaves like primary class template. */
        point(void);

        /** Behaves like primary class template. */
        explicit inline point(const T *coordinates) : super() {
            THE_STACK_TRACE;
            ASSERT(coordinates != NULL);
            ::memcpy(this->coordinates, coordinates, D * sizeof(T));
        }

        /**
         * Create a new point.
         *
         * @param x The x-coordinate. 
         * @param y The y-coordinate.
         */
        inline point(const T& x, const T& y) : super() {
            THE_STACK_TRACE;
            this->coordinates[0] = x;
            this->coordinates[1] = y;
        }

        /** Behaves like primary class template. */
        inline point(const point& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->coordinates, rhs.coordinates, D * sizeof(T));
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        point(const abstract_point<Tp, Dp, Sp>& rhs);

        /** Behaves like primary class template. */
        ~point(void);

        /** Behaves like primary class template. */
        inline point& operator =(const point& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline point& operator =(const abstract_point<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    private:

        /** The dimensionality of the point. */
        static const unsigned int D;

        /** super class typedef. */
        typedef abstract_point<T, 2, T[2]> super;

        
    };


    /*
     * the::math::point<T, 2>::point
     */
    template<class T> point<T, 2>::point(void) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->coordinates[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::point<T, 2>::point
     */
    template<class T>
    template<class Tp, unsigned int Dp, class Sp>
    point<T, 2>::point(const abstract_point<Tp, Dp, Sp>& rhs) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->coordinates[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->coordinates[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::point<T, 2>::~point
     */
    template<class T> point<T, 2>::~point(void) {
        THE_STACK_TRACE;
    }


    /*
     * the::math::point<T, 2>::D
     */
    template<class T> const unsigned int point<T, 2>::D = 2;


    /**
     * Partial template specialisation for three-dimensional point. This class
     * provides a constructor with separate components.
     */
    template<class T> 
    class point<T, 3> : public abstract_point<T, 3, T[3]> {

    public:

        /** Behaves like primary class template. */
        point(void);

        /** Behaves like primary class template. */
        explicit inline point(const T *coordinates) : super() {
            THE_STACK_TRACE;
            ASSERT(coordinates != NULL);
            ::memcpy(this->coordinates, coordinates, D * sizeof(T));
        }

        /**
         * Create a new point.
         *
         * @param x The x-coordinate. 
         * @param y The y-coordinate.
         * @param z The z-coordinate.
         */
        inline point(const T& x, const T& y, const T& z) : super() {
            THE_STACK_TRACE;
            this->coordinates[0] = x;
            this->coordinates[1] = y;
            this->coordinates[2] = z;
        }

        /** Behaves like primary class template. */
        inline point(const point& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->coordinates, rhs.coordinates, D * sizeof(T));
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        point(const abstract_point<Tp, Dp, Sp>& rhs);

        /** Behaves like primary class template. */
        ~point(void);

        /** Behaves like primary class template. */
        inline point& operator =(const point& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline point& operator =(const abstract_point<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    private:

        /** The dimensionality of the point. */
        static const unsigned int D;

        /** super class typedef. */
        typedef abstract_point<T, 3, T[3]> super;

        
    };


    /*
     * the::math::point<T, 3>::point
     */
    template<class T> point<T, 3>::point(void) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->coordinates[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::point<T, 3>::point
     */
    template<class T>
    template<class Tp, unsigned int Dp, class Sp>
    point<T, 3>::point(const abstract_point<Tp, Dp, Sp>& rhs) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->coordinates[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->coordinates[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::point<T, 3>::~point
     */
    template<class T> point<T, 3>::~point(void) {
        THE_STACK_TRACE;
    }


    /*
     * the::math::point<T, 3>::D
     */
    template<class T> const unsigned int point<T, 3>::D = 3;


    /**
     * Partial template specialisation for four-dimensional point. This class
     * provides a constructor with separate components.
     */
    template<class T> 
    class point<T, 4> : public abstract_point<T, 4, T[4]> {

    public:

        /** Behaves like primary class template. */
        point(void);

        /** Behaves like primary class template. */
        explicit inline point(const T *coordinates) : super() {
            THE_STACK_TRACE;
            ASSERT(coordinates != NULL);
            ::memcpy(this->coordinates, coordinates, D * sizeof(T));
        }

        /**
         * Create a new point.
         *
         * @param x The x-coordinate. 
         * @param y The y-coordinate.
         * @param z The z-coordinate.
         * @param w The w-coordinate.
         */
        inline point(const T& x, const T& y, const T& z, const T& w)
                : super() {
            THE_STACK_TRACE;
            this->coordinates[0] = x;
            this->coordinates[1] = y;
            this->coordinates[2] = z;
            this->coordinates[3] = w;
        }

        /** Behaves like primary class template. */
        inline point(const point& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->coordinates, rhs.coordinates, D * sizeof(T));
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        point(const abstract_point<Tp, Dp, Sp>& rhs);

        /** Behaves like primary class template. */
        ~point(void);

        /** Behaves like primary class template. */
        inline point& operator =(const point& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline point& operator =(const abstract_point<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    private:

        /** The dimensionality of the point. */
        static const unsigned int D;

        /** super class typedef. */
        typedef abstract_point<T, 4, T[4]> super;

        
    };


    /*
     * the::math::point<T, 4>::point
     */
    template<class T> point<T, 4>::point(void) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->coordinates[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::point<T, 4>::point
     */
    template<class T>
    template<class Tp, unsigned int Dp, class Sp>
    point<T, 4>::point(const abstract_point<Tp, Dp, Sp>& rhs) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->coordinates[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->coordinates[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::point<T, 4>::~point
     */
    template<class T> point<T, 4>::~point(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


    /*
     * the::math::point<T, 4>::D
     */
    template<class T> const unsigned int point<T, 4>::D = 4;



} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_POINT_H_INCLUDED */
