/*
 * the/math/abstract_polynom_impl.h
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
 * AbstractpolynomImpl.h
 *
 * Copyright (C) 2006 - 2010 by Visualisierungsinstitut Universitaet Stuttgart. 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_POLYNOM_IMPL_H_INCLUDED
#define THE_MATH_ABSTRACT_POLYNOM_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/assert.h"
#include "the/argument_exception.h"
#include "the/math/functions.h"
#include "the/not_supported_exception.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * Implementation of polynom behaviour. Do not use this class directly. It
     * is used to implement the same inheritance pattern as for vectors,
     * points etc. See the documentation of AbstractVectorImpl for further
     * details.
     *
     * The one-dimensional polynom is defined by its coefficients a_0 ... a_d
     * as:
     *  f(x) := a_d * x^d + a_{d-1} * x^{d-1} + ... + a_1 * x + a_0
     *
     * T scalar type
     * D Degree of the polynom
     * S Coefficient storage
     * C Deriving subclass.
     */
    template<class T, unsigned int D, class S,
            template<class T, unsigned int D, class S> class C>
    class abstract_polynom_impl {
    public:

        /** Dtor. */
        ~abstract_polynom_impl(void);

        /**
         * Tests if the polynom 'src' can be assigned to this object. This is
         * possible if the effective degree of 'src' is equal or smaller than
         * D.
         *
         * @param src The polynom to test
         *
         * @return True if 'src' can be assigned to this object.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline bool can_assign(const C<Tp, Dp, Sp>& src) const {
            THE_STACK_TRACE;
            return (src.effective_degree() <= D);
        }

        /**
         * Sets all coefficients to zero.
         */
        void clear(void);

        /**
         * Calculates the derivative of this polynom
         *
         * @return The derivative of this polynom
         */
        C<T, D - 1, T[D]> derivative(void) const;

        /**
         * Answers the effective degree of the polynom. The effective degree
         * is less than D if a_d is zero.
         *
         * @return The effective degree of the polynom.
         */
        unsigned int effective_degree(void) const;

        /**
         * Answer whether all coefficients are zero.
         *
         * @return True if all coefficients are zero.
         */
        bool is_zero(void) const;

        /**
         * Peeks at the internal representation of the coefficients of the
         * polynom (a_0, a_1, ... a_d).
         *
         * @return The internal representation of the coefficients of the
         *         polynom.
         */
        inline T* peek_coefficients(void) {
            THE_STACK_TRACE;
            return this->coefficients;
        }

        /**
         * Peeks at the internal representation of the coefficients of the
         * polynom (a_0, a_1, ... a_d).
         *
         * @return The internal representation of the coefficients of the
         *         polynom.
         */
        inline const T* peek_coefficients(void) const {
            THE_STACK_TRACE;
            return this->coefficients;
        }

        /**
         * Evaluates the polynom for 'x'
         *
         * @param x The value of the variable of the polynom
         *
         * @return The value of the polynom for 'x'
         */
        T operator()(T x);

        /**
         * Access to the i-th coefficient of the polynom (a_0, a_1, ... a_d).
         *
         * @param i The zero-based index of the coefficient of the polynom
         *          to be returned.
         *
         * @return The i-th coefficient of the polynom
         */
        inline T& operator[](int i) {
            THE_STACK_TRACE;
            return this->coefficients[i];
        }

        /**
         * Answer the i-th coefficient of the polynom (a_0, a_1, ... a_d).
         *
         * @param i The zero-based index of the coefficient of the polynom
         *          to be returned.
         *
         * @return The i-th coefficient of the polynom
         */
        inline T operator[](int i) const {
            THE_STACK_TRACE;
            return this->coefficients[i];
        }

        /**
         * Addition on each coefficient (y-shift)
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         */
        inline abstract_polynom_impl<T, D, S, C>& operator+=(const T& rhs) {
            THE_STACK_TRACE;
            for (unsigned int i = 0; i <= D; i++) {
                this->coefficients[i] += rhs;
            }
            return *this;
        }

        /**
         * Subtraction on each coefficient (y-shift)
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         */
        inline abstract_polynom_impl<T, D, S, C>& operator-=(const T& rhs) {
            THE_STACK_TRACE;
            for (unsigned int i = 0; i <= D; i++) {
                this->coefficients[i] -= rhs;
            }
            return *this;
        }

        /**
         * Scalar multiplication
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         */
        inline abstract_polynom_impl<T, D, S, C>& operator*=(const T& rhs) {
            THE_STACK_TRACE;
            for (unsigned int i = 0; i <= D; i++) {
                this->coefficients[i] *= rhs;
            }
            return *this;
        }

        /**
         * Scalar division
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         */
        inline abstract_polynom_impl<T, D, S, C>& operator/=(const T& rhs) {
            THE_STACK_TRACE;
            for (unsigned int i = 0; i <= D; i++) {
                this->coefficients[i] /= rhs;
            }
            return *this;
        }

        /**
         * Assignment operator
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         *
         * @throw illegal_param_exception if 'rhs' has an effective degree larger
         *        than D.
         */
        template<class Tp, unsigned int Dp, class Sp>
        abstract_polynom_impl<T, D, S, C>& operator=(const C<Tp, Dp, Sp>& rhs);

        /**
         * Test for equality
         *
         * @param rhs The right hand side operand
         *
         * @return true if this and rhs are equal
         */
        template<class Tp, unsigned int Dp, class Sp>
        bool operator==(const C<Tp, Dp, Sp>& rhs) const;

        /**
         * Test for inequality
         *
         * @param rhs The right hand side operand
         *
         * @return false if this and rhs are equal
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline bool operator!=(const C<Tp, Dp, Sp>& rhs)
                const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

    protected:

        /**
         * Finds the roots of the polynom for the linear case
         *
         * @param a0 The const coefficient
         * @param a1 The linear coefficient
         * @param outRoots Array to receive the found roots
         * @param size The size of 'outRoots' in number of elements
         *
         * @return The found roots
         */
        static unsigned int find_roots_deg1(const T& a0, const T& a1,
            T *outRoots, unsigned int size);

        /**
         * Finds the roots of the polynom for the quadratic case
         *
         * @param a0 The const coefficient
         * @param a1 The linear coefficient
         * @param a2 The quadratic coefficient
         * @param outRoots Array to receive the found roots
         * @param size The size of 'outRoots' in number of elements
         *
         * @return The found roots
         */
        static unsigned int find_roots_deg2(const T& a0, const T& a1,
            const T& a2, T *outRoots, unsigned int size);

        /**
         * Finds the roots of the polynom for the cubic case
         *
         * @param a0 The const coefficient
         * @param a1 The linear coefficient
         * @param a2 The quadratic coefficient
         * @param a3 The cubic coefficient
         * @param outRoots Array to receive the found roots
         * @param size The size of 'outRoots' in number of elements
         *
         * @return The found roots
         */
        static unsigned int find_roots_deg3(const T& a0, const T& a1,
            const T& a2, const T& a3, T *outRoots, unsigned int size);

        /**
         * Finds the roots of the polynom for the quartic case
         *
         * @param a0 The const coefficient
         * @param a1 The linear coefficient
         * @param a2 The quadratic coefficient
         * @param a3 The cubic coefficient
         * @param a4 The quartic coefficient
         * @param outRoots Array to receive the found roots
         * @param size The size of 'outRoots' in number of elements
         *
         * @return The found roots
         */
        static unsigned int find_roots_deg4(const T& a0, const T& a1,
            const T& a2, const T& a3, const T& a4, T *outRoots,
            unsigned int size);

        /**
         * Removed double roots from the array 'outRoots'
         *
         * @param outRoots Pointer to the array of found roots
         * @param size The number of valid entries in 'outRoots'
         *
         * @return The numer of unique roots now in 'outRoots'
         */
        static unsigned int unique_roots(T *outRoots, unsigned int size);

        /** Ctor. */
        inline abstract_polynom_impl(void) {
            THE_STACK_TRACE;
        };

        /**
         * The D + 1 polynom coefficients
         */
        S coefficients;

        /* Allow access to the coefficients */
        template<class Tf1, unsigned int Df1, class Sf1,
            template<class Tf2, unsigned int Df2, class Sf2> class Cf1>
            friend class abstract_polynom_impl;

    };


    /*
     * abstract_polynom_impl<T, D, S, C>::~abstract_polynom_impl
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    abstract_polynom_impl<T, D, S, C>::~abstract_polynom_impl(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::clear
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    void abstract_polynom_impl<T, D, S, C>::clear(void) {
        THE_STACK_TRACE;
        for (unsigned int i = 0; i <= D; i++) {
            this->coefficients[i] = static_cast<T>(0);
        }
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::derivative
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    C<T, D - 1, T[D]> abstract_polynom_impl<T, D, S, C>::derivative(void) const {
        THE_STACK_TRACE;
        C<T, D - 1, T[D]> rv;

        for (unsigned int i = 0; i < D; i++) {
            rv.coefficients[i] = this->coefficients[i + 1] *
                static_cast<T>(i + 1);
        }

        return rv;
    }

    /*
     * abstract_polynom_impl<T, D, S, C>::effective_degree
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    unsigned int abstract_polynom_impl<T, D, S, C>::effective_degree(void) const {
        THE_STACK_TRACE;
        for (unsigned int i = D; i > 0; i--) {
            if (!the::math::is_equal(this->coefficients[i],
                    static_cast<T>(0))) {
                return i;
            }
        }
        return 0;
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::is_zero
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    bool abstract_polynom_impl<T, D, S, C>::is_zero(void) const {
        THE_STACK_TRACE;
        for (unsigned int i = 0; i <= D; i++) {
            if (!the::math::is_equal(this->coefficients[i],
                    static_cast<T>(0))) {
                return false;
            }
        }
        return true;
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::operator()
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    T abstract_polynom_impl<T, D, S, C>::operator()(T x) {
        THE_STACK_TRACE;
        T val = this->coefficients[0];
        T xp = static_cast<T>(1);

        for (unsigned int i = 1; i <= D; i++) {
            xp *= x;
            val += this->coefficients[i] * xp;
        }

        return val;
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::operator=
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    template<class Tp, unsigned int Dp, class Sp>
    abstract_polynom_impl<T, D, S, C>&
    abstract_polynom_impl<T, D, S, C>::operator=(const C<Tp, Dp, Sp>& rhs) {
        THE_STACK_TRACE;
        unsigned int rhsed = rhs.effective_degree();
        if (rhsed > D) {
            throw the::argument_exception("rhs", __FILE__, __LINE__);
        }

        for (unsigned int i = 0; i <= rhsed; i++) {
            this->coefficients[i] = static_cast<T>(rhs.coefficients[i]);
        }
        for (unsigned int i = rhsed + 1; i <= D; i++) {
            this->coefficients[i] = static_cast<T>(0);
        }

        return *this;
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::operator==
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    template<class Tp, unsigned int Dp, class Sp>
    bool abstract_polynom_impl<T, D, S, C>::operator==(
            const C<Tp, Dp, Sp>& rhs) const {
        THE_STACK_TRACE;

        for (unsigned int i = 0; i <= ((D < Dp) ? D : Dp); i++) {
            if (!the::math::is_equal(this->coefficients[i],
                    static_cast<T>(rhs.coefficients[i]))) {
                return false;
            }
        }
        for (unsigned int i = ((D < Dp) ? D : Dp) + 1; i <= D; i++) {
            if (!the::math::is_equal(this->coefficients[i],
                    static_cast<T>(0))) {
                return false;
            }
        }
        for (unsigned int i = ((D < Dp) ? D : Dp) + 1; i <= Dp; i++) {
            if (!the::math::is_equal(rhs.coefficients[i],
                    static_cast<Tp>(0))) {
                return false;
            }
        }

        return true;
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::find_roots_deg1
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    unsigned int abstract_polynom_impl<T, D, S, C>::find_roots_deg1(const T& a0,
            const T& a1, T *outRoots, unsigned int size) {
        THE_STACK_TRACE;
        ASSERT(!is_equal(a1, static_cast<T>(0)));
        ASSERT(size > 0);

        outRoots[0] = -a0 / a1;

        return 1;
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::find_roots_deg2
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    unsigned int abstract_polynom_impl<T, D, S, C>::find_roots_deg2(const T& a0,
            const T& a1, const T& a2, T *outRoots, unsigned int size) {
        THE_STACK_TRACE;
        ASSERT(!is_equal(a2, static_cast<T>(0)));
        ASSERT(size > 0);

        T a = a2 * static_cast<T>(2);
        T b = a1 * a1 - a0 * a2 * static_cast<T>(4);

        if (is_equal(b, static_cast<T>(0))) {
            // one root
            outRoots[0] = -a1 / a;
            return 1;
        } else if (b > static_cast<T>(0)) {
            // two roots
            b = static_cast<T>(::sqrt(static_cast<double>(b)));
            outRoots[0] = (-a1 + b) / a;
            if (size > 1) {
                outRoots[1] = (-a1 - b) / a;
                return 2;
            }
            return 1;
        }

        return 0; // no roots
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::find_roots_deg3
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    unsigned int abstract_polynom_impl<T, D, S, C>::find_roots_deg3(const T& a0,
            const T& a1, const T& a2, const T& a3, T *outRoots,
            unsigned int size) {
        THE_STACK_TRACE;
        ASSERT(!is_equal(a3, static_cast<T>(0)));
        ASSERT(size > 0);
        // calculation following description at
        // http://www.mathe.tu-freiberg.de/~hebisch/cafe/kubisch.html
        // (14.03.2010)

        T p = (static_cast<T>(3) * a3 * a1) - (a2 * a2);
        T q = (static_cast<T>(27) * a3 * a3 * a0)
            - (static_cast<T>(9) * a3 * a2 * a1)
            + (static_cast<T>(2) * a2 * a2 * a2);

        if (is_equal(p, static_cast<T>(0))) { // special case
            // y1 per double calculation; not nice, but ok for now
            T y1 = static_cast<T>(::pow(static_cast<double>(-q), 1.0 / 3.0));
            outRoots[0] = (y1 - a2) / (static_cast<T>(3) * a3);
            if (size > 1) {
                // quadratic polynom through polynom division
                T y23[2];
                T qa0 = (static_cast<T>(3) * p) - (y1 * y1);
                T qa1 = -y1;
                T qa2 = static_cast<T>(1);
                unsigned int qrc = find_roots_deg2(qa0, qa1, qa2, y23, 2);
                if (qrc > 0) {
                    outRoots[1] = (y23[0] - a2) / (static_cast<T>(3) * a3);
                }
                if ((qrc > 1) && (size > 2)) {
                    outRoots[2] = (y23[1] - a2) / (static_cast<T>(3) * a3);
                    return unique_roots(outRoots, 3);
                }
                return unique_roots(outRoots, 2);
            }
            return 1;
        }
        // p != 0

        T dis = (q * q) + (static_cast<T>(4) * p * p * p);
        if (dis < static_cast<T>(0)) {
            // casus irreducibilis
            ASSERT(p < static_cast<T>(0)); // or square-root would be complex

            double cosphi = static_cast<double>(-q)
                / (2.0 * ::sqrt(-static_cast<double>(p * p * p)));
            double phi = ::acos(cosphi);
            double sqrtNegP = ::sqrt(static_cast<double>(-p));
            double phiThird = phi / 3.0;
            double piThird = the::math::pi_double / 3.0;

            T y1 = static_cast<T>(2.0 * sqrtNegP * ::cos(phiThird));
            T y2 = static_cast<T>(-2.0 * sqrtNegP * ::cos(phiThird + piThird));
            T y3 = static_cast<T>(-2.0 * sqrtNegP * ::cos(phiThird - piThird));

            outRoots[0] = (y1 - a2) / (static_cast<T>(3) * a3);
            if (size > 1) {
                outRoots[1] = (y2 - a2) / (static_cast<T>(3) * a3);
                if (size > 2) {
                    outRoots[2] = (y3 - a2) / (static_cast<T>(3) * a3);
                    return unique_roots(outRoots, 3);
                }
                return unique_roots(outRoots, 2);
            }
            return 1;
        }

        double sqrtDis = ::sqrt(static_cast<double>(dis));
        T u = static_cast<T>(0.5) * static_cast<T>(
            ::pow(-4.0 * static_cast<double>(q) + 4.0 * sqrtDis,
                1.0 / 3.0));
        T v = static_cast<T>(0.5) * static_cast<T>(
            ::pow(-4.0 * static_cast<double>(q) - 4.0 * sqrtDis,
                1.0 / 3.0));
        T y1 = u + v;
        outRoots[0] = (y1 - a2) / (static_cast<T>(3) * a3);

        if (size > 1) {
            if (is_equal(u, v)) {
                T y2 = static_cast<T>(-0.5) * (u + v);
                outRoots[1] = (y2 - a2) / (static_cast<T>(3) * a3);
                return unique_roots(outRoots, 2);
            } // else second and thrid root are complex
              // (we only return real roots)
        }

        return 1;
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::find_roots_deg4
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    unsigned int abstract_polynom_impl<T, D, S, C>::find_roots_deg4(const T& a0,
            const T& a1, const T& a2, const T& a3, const T& a4, T *outRoots,
            unsigned int size) {
        THE_STACK_TRACE;
        ASSERT(!is_equal(a4, static_cast<T>(0)));
        ASSERT(size > 0);

        // Implementation of Ferrari-Lagrange method for solving
        //  x^4 + ax^3 + bx^2 + cx + d = 0
        T a = a3 / a4;
        T b = a2 / a4;
        T c = a1 / a4;
        T d = a0 / a4;

        T asq = a * a;
        T p = b;
        T q = a * c - static_cast<T>(4) * d;
        T r = (asq - static_cast<T>(4) * b) * d + c * c;
        T y;
        { // finds the smallest x for cubic polynom x^3 + px^2 + qx + r = 0;
            T cr[3];
            unsigned int crc
                = find_roots_deg3(r, q, p, static_cast<T>(1), cr, 3);
            ASSERT(crc > 0);
            if (crc == 3) y = minimum(minimum(cr[0], cr[1]), cr[2]);
            else if (crc == 2) y = minimum(cr[0], cr[1]);
            else y = cr[0];
        }

        T esq = static_cast<T>(0.25) * asq - b - y;
        if (is_equal(esq, static_cast<T>(0))) return 0;

        T fsq = static_cast<T>(0.25) * y * y - d;
        if (is_equal(fsq, static_cast<T>(0))) return 0;

        T ef = -(static_cast<T>(0.25) * a * y + static_cast<T>(0.5) * c);
        T e, f;

        if (((a > static_cast<T>(0)) && (y > static_cast<T>(0))
                    && (c > static_cast<T>(0)))
                || ((a > static_cast<T>(0)) && (y < static_cast<T>(0))
                    && (c < static_cast<T>(0)))
                || ((a < static_cast<T>(0)) && (y < static_cast<T>(0))
                    && (c > static_cast<T>(0)))
                || ((a < static_cast<T>(0)) && (y > static_cast<T>(0))
                    && (c < static_cast<T>(0)))
                || is_equal(a, static_cast<T>(0))
                || is_equal(y, static_cast<T>(0))
                || is_equal(c, static_cast<T>(0))) {
            /* use ef - */

            if ((b < static_cast<T>(0)) && (y < static_cast<T>(0))
                    && (esq > static_cast<T>(0))) {
                e = static_cast<T>(::sqrt(static_cast<double>(esq)));
                f = ef / e;
            } else if ((d < static_cast<T>(0)) && (fsq > static_cast<T>(0))) {
                f = static_cast<T>(::sqrt(static_cast<double>(fsq)));
                e = ef / f;
            } else {
                e = static_cast<T>(::sqrt(static_cast<double>(esq)));
                f = static_cast<T>(::sqrt(static_cast<double>(fsq)));
                if (ef < static_cast<T>(0)) f = -f;
            }
        } else {
            e = static_cast<T>(::sqrt(static_cast<double>(esq)));
            f = static_cast<T>(::sqrt(static_cast<double>(fsq)));
            if (ef < static_cast<T>(0)) f = -f;
        }

        /* note that e >= nought */
        T ainv2 = a * static_cast<T>(0.5);
        T g = ainv2 - e;
        T gg = ainv2 + e;

        if (((b > static_cast<T>(0)) && (y > static_cast<T>(0)))
                || ((b < static_cast<T>(0)) && (y < static_cast<T>(0)))) {
            if ((a > static_cast<T>(0)) && !is_equal(e, static_cast<T>(0))) {
                g = (b + y) / gg;
            } else if (!is_equal(e, static_cast<T>(0))) {
                gg = (b + y) / g;
            }
        }

        T h, hh;
        if (is_equal(y, static_cast<T>(0)) && is_equal(f, static_cast<T>(0))) {
            h = hh = static_cast<T>(0);
        } else if (((f > static_cast<T>(0)) && (y < static_cast<T>(0)))
                || ((f < static_cast<T>(0)) && (y > static_cast<T>(0)))) {
            hh = static_cast<T>(-0.5) * y + f;
            h = d / hh;
        } else {
            h = static_cast<T>(-0.5) * y - f;
            hh = d / h;
        }

        unsigned int cnt = find_roots_deg2(hh, gg, static_cast<T>(1), outRoots, size);
        if ((size - cnt) > 0) {
            cnt += find_roots_deg2(h, g, static_cast<T>(1), outRoots + cnt, size - cnt);
        }

        return unique_roots(outRoots, cnt);
    }


    /*
     * abstract_polynom_impl<T, D, S, C>::unique_roots
     */
    template<class T, unsigned int D, class S,
        template<class T, unsigned int D, class S> class C>
    unsigned int abstract_polynom_impl<T, D, S, C>::unique_roots(T *outRoots,
            unsigned int size) {
        THE_STACK_TRACE;
        if (size <= 1) return size;
        if (size == 2) return (is_equal(outRoots[0], outRoots[1])) ? 1 : 2;

        // o(n^2) search to keep the implementation simple
        // change this if the degree of the polynom gets LARGE
        bool found;
        unsigned int cnt = 1;
        for (unsigned int i = 1; i < size; i++) {
            found = false;
            for (unsigned int j = 0; j < cnt; j++) {
                if (is_equal(outRoots[i], outRoots[j])) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                outRoots[cnt++] = outRoots[i];
            }
        }

        return cnt;
    }


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_POLYNOM_IMPL_H_INCLUDED */
