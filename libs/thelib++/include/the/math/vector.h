/*
 * the/math/vector.h
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
 * vector.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller. Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_VECTOR_H_INCLUDED
#define THE_MATH_VECTOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"
#include "the/math/abstract_vector.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * This is the implementation of an abstract_vector that uses its own memory 
     * in a statically allocated array of dimension D. Usually, you want to use
     * this vector class or derived classes.
     *
     * See documentation of abstract_vector for further information about the 
     * vector classes.
     */
    template<class T, unsigned int D> 
    class vector : public abstract_vector<T, D, T[D]> {

    public:

        /**
         * Create a null vector.
         */
        vector(void);

        /**
         * Create a new vector initialised with 'components'. 'components' must
         * not be a NULL pointer. 
         *
         * @param components The initial vector components.
         */
        explicit inline vector(const T *components) : super() {
            THE_STACK_TRACE;
            assert(components != NULL);
            ::memcpy(this->components, components, D * sizeof(T));
        }

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline vector(const vector& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.components, D * sizeof(T));
        }

        /**
         * Create a copy of 'rhs'. This ctor allows for arbitrary vector to
         * vector conversions.
         *
         * @param rhs The vector to be cloned.
         */
        template<class Tp, unsigned int Dp, class Sp>
        vector(const abstract_vector<Tp, Dp, Sp>& rhs);

        /** Dtor. */
        ~vector(void);

        /**
         * Assignment.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        inline vector& operator =(const vector& rhs) {
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
        inline vector& operator =(const abstract_vector<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    private:

        /** A typedef for the super class. */
        typedef abstract_vector<T, D, T[D]> super;
    };


    /*
     * the::math::vector<T, D>::vector
     */
    template<class T, unsigned int D>
    vector<T, D>::vector(void) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->components[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::vector<T, D>::vector
     */
    template<class T, unsigned int D>
    template<class Tp, unsigned int Dp, class Sp>
    vector<T, D>::vector(const abstract_vector<Tp, Dp, Sp>& rhs) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->components[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->components[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::vector<T, D>::~vector
     */
    template<class T, unsigned int D>
    vector<T, D>::~vector(void) {
        THE_STACK_TRACE;
    }


    /**
     * Partial template specialisation for two-dimensional vectors. This class 
     * provides an additional constructor with single components.
     */
    template<class T> 
    class vector<T, 2> : public abstract_vector<T, 2, T[2]> {

    public:

        /** Behaves like primary class template. */
        vector(void);

        /** Behaves like primary class template. */
        explicit inline vector(const T *components) : super() {
            THE_STACK_TRACE;
            assert(components != NULL);
            ::memcpy(this->components, components, D * sizeof(T));
        }

        /**
         * Create a new vector.
         *
         * @param x The x-component.
         * @param y The y-component.
         */
        inline vector(const T& x, const T& y) : super() {
            THE_STACK_TRACE;
            this->components[0] = x;
            this->components[1] = y;
        }

        /** Behaves like primary class template. */
        inline vector(const vector& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.components, D * sizeof(T));
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        vector(const abstract_vector<Tp, Dp, Sp>& rhs);

        /** Behaves like primary class template. */
        ~vector(void);

        /** Behaves like primary class template. */
        inline vector& operator =(const vector& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline vector& operator =(const abstract_vector<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    private:

        /** The dimension of the vector. */
        static const unsigned int D;

        /** A typedef for the super class. */
        typedef abstract_vector<T, 2, T[2]> super;
    };


    /*
     * the::math::vector<T, 2>::vector
     */
    template<class T> vector<T, 2>::vector(void) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->components[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::vector<T, 2>::vector
     */
    template<class T>
    template<class Tp, unsigned int Dp, class Sp>
    vector<T, 2>::vector(const abstract_vector<Tp, Dp, Sp>& rhs) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->components[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->components[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::vector<T, 2>::~vector
     */
    template<class T> vector<T, 2>::~vector(void) {
        THE_STACK_TRACE;
    }


    /*
     * vector<T, 2>::D
     */
    template<class T> const unsigned int vector<T, 2>::D = 2;


    /**
     * Partial template specialisation for three-dimensional vectors. This
     * class provides an additional constructor with single components.
     */
    template<class T> 
    class vector<T, 3> : public abstract_vector<T, 3, T[3]> {

    public:

        /** Behaves like primary class template. */
        vector(void);

        /** Behaves like primary class template. */
        explicit inline vector(const T *components) : super() {
            THE_STACK_TRACE;
            assert(components != NULL);
            ::memcpy(this->components, components, D * sizeof(T));
        }

        /**
         * Create a new vector.
         *
         * @param x The x-component.
         * @param y The y-component.
         * @param z The z-component.
         */
        inline vector(const T& x, const T& y, const T& z) : super() {
            THE_STACK_TRACE;
            this->components[0] = x;
            this->components[1] = y;
            this->components[2] = z;
        }

        /** Behaves like primary class template. */
        inline vector(const vector& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.components, D * sizeof(T));
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        vector(const abstract_vector<Tp, Dp, Sp>& rhs);

        /** Behaves like primary class template. */
        ~vector(void);

        /** Behaves like primary class template. */
        inline vector& operator =(const vector& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline vector& operator =(const abstract_vector<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    private:

        /** The dimension of the vector. */
        static const unsigned int D;

        /** A typedef for the super class. */
        typedef abstract_vector<T, 3, T[3]> super;
    };


    /*
     * the::math::vector<T, 3>::vector
     */
    template<class T> vector<T, 3>::vector(void) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->components[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::vector<T, 3>::vector
     */
    template<class T>
    template<class Tp, unsigned int Dp, class Sp>
    vector<T, 3>::vector(const abstract_vector<Tp, Dp, Sp>& rhs) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->components[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->components[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::vector<T, 3>::~vector
     */
    template<class T> vector<T, 3>::~vector(void) {
        THE_STACK_TRACE;
    }


    /*
     * vector<T, 3>::D
     */
    template<class T> const unsigned int vector<T, 3>::D = 3;


    /**
     * Partial template specialisation for four-dimensional vectors. This
     * class provides an additional constructor with single components.
     */
    template<class T> 
    class vector<T, 4> : public abstract_vector<T, 4, T[4]> {

    public:

        /** Behaves like primary class template. */
        vector(void);

        /** Behaves like primary class template. */
        explicit inline vector(const T *components) : super() {
            THE_STACK_TRACE;
            assert(components != NULL);
            ::memcpy(this->components, components, D * sizeof(T));
        }

        /**
         * Create a new vector.
         *
         * @param x The x-component.
         * @param y The y-component.
         * @param z The z-component.
         * @param w The w-component.
         */
        inline vector(const T& x, const T& y, const T& z, const T& w)
                : super() {
            THE_STACK_TRACE;
            this->components[0] = x;
            this->components[1] = y;
            this->components[2] = z;
            this->components[3] = w;
        }

        /** Behaves like primary class template. */
        inline vector(const vector& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.components, D * sizeof(T));
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        vector(const abstract_vector<Tp, Dp, Sp>& rhs);

        /** Behaves like primary class template. */
        ~vector(void);

        /** Behaves like primary class template. */
        inline vector& operator =(const vector& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline vector& operator =(const abstract_vector<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    private:

        /** The dimension of the vector. */
        static const unsigned int D;

        /** A typedef for the super class. */
        typedef abstract_vector<T, 4, T[4]> super;
    };


    /*
     * the::math::vector<T, 4>::vector
     */
    template<class T> vector<T, 4>::vector(void) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->components[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::vector<T, 4>::vector
     */
    template<class T>
    template<class Tp, unsigned int Dp, class Sp>
    vector<T, 4>::vector(const abstract_vector<Tp, Dp, Sp>& rhs) : super() {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->components[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->components[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::vector<T, 4>::~vector
     */
    template<class T> vector<T, 4>::~vector(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


    /*
     * vector<T, 4>::D
     */
    template<class T> const unsigned int vector<T, 4>::D = 4;


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_VECTOR_H_INCLUDED */
