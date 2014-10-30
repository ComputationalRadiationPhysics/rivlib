/*
 * the/math/matrix.h
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
 * Matrix.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_MATRIX_H_INCLUDED
#define THE_MATH_MATRIX_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/assert.h"
#include "the/math/abstract_matrix.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * This class implements matrices that use their own memory on the stack
     * to store their components.
     */
    template<class T, unsigned int D, matrix_layout L> 
    class matrix : public abstract_matrix<T, D, L, T[D * D]> {

    public:

        /** 
         * Create the identity matrix.
         */
        inline matrix(void) : super() {
            THE_STACK_TRACE;
            this->set_identity();
        }

        /**
         * Create which has the same value for all components.
         *
         * @param value The initial value of all components.
         */
        matrix(const T& value);

        /**
         * Create a matrix using the specified components.
         *
         * @param components (D * D) components of the matrix. This must not be
         *                   NULL and according to the matrix layout L.
         */
        inline matrix(const T *components) : super() {
            THE_STACK_TRACE;
            ASSERT(components != NULL);
            ::memcpy(this->components, components, super::CNT_COMPONENTS
                * sizeof(T));
        }

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline matrix(const matrix& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.component, 
                super::CNT_COMPONENTS * sizeof(T));
        }

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
        inline matrix(const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) : super() {
            THE_STACK_TRACE;
            this->assign(rhs);
        }

        /** Dtor. */
        ~matrix(void);

        /**
         * Assignment operator.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline matrix& operator =(const matrix& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * Assigment for arbitrary matrices. A valid static_cast between T and 
         * Tp is a precondition for instantiating this template.
         *
         * This operation does <b>not</b> create aliases. 
         *
         * If the two operands have different dimensions, the behaviour is as 
         * follows: If the left hand side operand has lower dimension, the 
         * highest (Dp - D) dimensions are discarded. If the left hand side
         * operand has higher dimension, the missing dimensions are filled with 
         * parts of the identity matrix.
         *
         * Subclasses must ensure that sufficient memory for the 'coordinates'
         * member has been allocated before calling this operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
        inline matrix& operator =(const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** A typedef for the super class. */
        typedef abstract_matrix<T, D, L, T[D * D]> super;

    };


    /*
     * the::math::matrix<T, D, L>::matrix
     */
    template<class T, unsigned int D, matrix_layout L>
    matrix<T, D, L>::matrix(const T& value) : super() {
        THE_STACK_TRACE;
        for (unsigned int i = 0; i < D * D; i++) {
            this->components[i] = value;
        }
    }


    /*
     * the::math::matrix<T, D, L>::~matrix
     */
    template<class T, unsigned int D, matrix_layout L>
    matrix<T, D, L>::~matrix(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


    /**
     * This class implements matrices that use their own memory on the stack
     * to store their components.
     * Partial template specialization for 3x3 matrices.
     */
    template<class T, matrix_layout L> 
    class matrix<T, 3, L> : public abstract_matrix<T, 3, L, T[3 * 3]> {

    public:

        /** 
         * Create the identity matrix.
         */
        inline matrix(void) : super() {
            THE_STACK_TRACE;
            this->set_identity();
        }

        /**
         * Create which has the same value for all components.
         *
         * @param value The initial value of all components.
         */
        matrix(const T& value);

        /**
         * Create a matrix using the specified components.
         *
         * @param components (D * D) components of the matrix. This must not be
         *                   NULL and according to the matrix layout L.
         */
        inline matrix(const T *components) : super() {
            THE_STACK_TRACE;
            ASSERT(components != NULL);
            ::memcpy(this->components, components, super::CNT_COMPONENTS
                * sizeof(T));
        }

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline matrix(const matrix& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.component, 
                super::CNT_COMPONENTS * sizeof(T));
        }

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
        inline matrix(const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) : super() {
            THE_STACK_TRACE;
            this->assign(rhs);
        }


        /**
         * Create a matrix using the specified components. Note, that the first
         * number of the component is the row, the second the column.
         *
         * @param m11 Element in row 1, column 1.
         * @param m12 Element in row 1, column 2.
         * @param m13 Element in row 1, column 3.
         * @param m21 Element in row 2, column 1.
         * @param m22 Element in row 2, column 2.
         * @param m23 Element in row 2, column 3.
         * @param m31 Element in row 3, column 1.
         * @param m32 Element in row 3, column 2.
         * @param m33 Element in row 3, column 3.
         */
        inline matrix(const T& m11, const T& m12, const T& m13,
                const T& m21, const T& m22, const T& m23,
                const T& m31, const T& m32, const T& m33) : super() {
            THE_STACK_TRACE;
            this->components[super::index_of(0, 0)] = m11;
            this->components[super::index_of(0, 1)] = m12;
            this->components[super::index_of(0, 2)] = m13;
            this->components[super::index_of(1, 0)] = m21;
            this->components[super::index_of(1, 1)] = m22;
            this->components[super::index_of(1, 2)] = m23;
            this->components[super::index_of(2, 0)] = m31;
            this->components[super::index_of(2, 1)] = m32;
            this->components[super::index_of(2, 2)] = m33;
        }

        /**
         * Create a matrix that represents the rotation of the quaternion
         * 'rhs'.
         *
         * @param rhs The quaterion to be converted.
         */
        template<class Tp, class Sp>
        explicit inline matrix(const abstract_quaternion<Tp, Sp>& rhs)
                : super() {
            THE_STACK_TRACE;
            *this=rhs;
        }

        /** Dtor. */
        ~matrix(void);

        /**
         * Assignment operator.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline matrix& operator =(const matrix& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * Assigment for arbitrary matrices. A valid static_cast between T and 
         * Tp is a precondition for instantiating this template.
         *
         * This operation does <b>not</b> create aliases. 
         *
         * If the two operands have different dimensions, the behaviour is as 
         * follows: If the left hand side operand has lower dimension, the 
         * highest (Dp - D) dimensions are discarded. If the left hand side
         * operand has higher dimension, the missing dimensions are filled with 
         * parts of the identity matrix.
         *
         * Subclasses must ensure that sufficient memory for the 'coordinates'
         * member has been allocated before calling this operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
        inline matrix& operator =(const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * Make this matrix represent the quaterion 'rhs'.
         *
         * @param rhs The quaterion to be converted to a rotation matrix.
         *
         * @return *this.
         */
        template<class Tp, class Sp>
        inline matrix& operator =(const abstract_quaternion<Tp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** A typedef for the super class. */
        typedef abstract_matrix<T, 3, L, T[3 * 3]> super;

    };


    /*
     * the::math::matrix<T, 3, L>::matrix
     */
    template<class T, matrix_layout L>
    matrix<T, 3, L>::matrix(const T& value) : super() {
        THE_STACK_TRACE;
        for (unsigned int i = 0; i < 3 * 3; i++) {
            this->components[i] = value;
        }
    }


    /*
     * the::math::matrix<T, 3, L>::~matrix
     */
    template<class T, matrix_layout L>
    matrix<T, 3, L>::~matrix(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


    /**
     * This class implements matrices that use their own memory on the stack
     * to store their components.
     * Partial template specialization for 4x4 matrices.
     */
    template<class T, matrix_layout L> 
    class matrix<T, 4, L> : public abstract_matrix<T, 4, L, T[4 * 4]> {

    public:

        /** 
         * Create the identity matrix.
         */
        inline matrix(void) : super() {
            THE_STACK_TRACE;
            this->set_identity();
        }

        /**
         * Create which has the same value for all components.
         *
         * @param value The initial value of all components.
         */
        matrix(const T& value);

        /**
         * Create a matrix using the specified components.
         *
         * @param components (D * D) components of the matrix. This must not be
         *                   NULL and according to the matrix layout L.
         */
        inline matrix(const T *components) : super() {
            THE_STACK_TRACE;
            ASSERT(components != NULL);
            ::memcpy(this->components, components, super::CNT_COMPONENTS
                * sizeof(T));
        }

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline matrix(const matrix& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.components, 
                super::CNT_COMPONENTS * sizeof(T));
        }

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
        inline matrix(const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) : super() {
            THE_STACK_TRACE;
            this->assign(rhs);
        }


        /**
         * Create a matrix using the specified components. Note, that the first
         * number of the component is the row, the second the column.
         *
         * @param m11 Element in row 1, column 1.
         * @param m12 Element in row 1, column 2.
         * @param m13 Element in row 1, column 3.
         * @param m14 Element in row 1, column 4.
         * @param m21 Element in row 2, column 1.
         * @param m22 Element in row 2, column 2.
         * @param m23 Element in row 2, column 3.
         * @param m24 Element in row 2, column 4.
         * @param m31 Element in row 3, column 1.
         * @param m32 Element in row 3, column 2.
         * @param m33 Element in row 3, column 3.
         * @param m34 Element in row 3, column 4.
         * @param m41 Element in row 4, column 1.
         * @param m42 Element in row 4, column 2.
         * @param m43 Element in row 4, column 3.
         * @param m44 Element in row 4, column 4.
         */
        matrix(const T& m11, const T& m12, const T& m13, const T& m14,
                const T& m21, const T& m22, const T& m23, const T& m24,
                const T& m31, const T& m32, const T& m33, const T& m34,
                const T& m41, const T& m42, const T& m43, const T& m44)
                    : super() {
            THE_STACK_TRACE;
            this->components[super::index_of(0, 0)] = m11;
            this->components[super::index_of(0, 1)] = m12;
            this->components[super::index_of(0, 2)] = m13;
            this->components[super::index_of(0, 3)] = m14;
            this->components[super::index_of(1, 0)] = m21;
            this->components[super::index_of(1, 1)] = m22;
            this->components[super::index_of(1, 2)] = m23;
            this->components[super::index_of(1, 3)] = m24;
            this->components[super::index_of(2, 0)] = m31;
            this->components[super::index_of(2, 1)] = m32;
            this->components[super::index_of(2, 2)] = m33;
            this->components[super::index_of(2, 3)] = m34;
            this->components[super::index_of(3, 0)] = m41;
            this->components[super::index_of(3, 1)] = m42;
            this->components[super::index_of(3, 2)] = m43;
            this->components[super::index_of(3, 3)] = m44;
        }

        /**
         * Create a matrix that represents the rotation of the quaternion
         * 'rhs'.
         *
         * @param rhs The quaterion to be converted.
         */
        template<class Tp, class Sp>
        explicit inline matrix(const abstract_quaternion<Tp, Sp>& rhs)
                : super() {
            THE_STACK_TRACE;
            *this=rhs;
        }

        /** Dtor. */
        ~matrix(void);

        /**
         * Assignment operator.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline matrix& operator =(const matrix& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * Assigment for arbitrary matrices. A valid static_cast between T and 
         * Tp is a precondition for instantiating this template.
         *
         * This operation does <b>not</b> create aliases. 
         *
         * If the two operands have different dimensions, the behaviour is as 
         * follows: If the left hand side operand has lower dimension, the 
         * highest (Dp - D) dimensions are discarded. If the left hand side
         * operand has higher dimension, the missing dimensions are filled with 
         * parts of the identity matrix.
         *
         * Subclasses must ensure that sufficient memory for the 'coordinates'
         * member has been allocated before calling this operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
        inline matrix& operator =(const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * Make this matrix represent the quaterion 'rhs'.
         *
         * @param rhs The quaterion to be converted to a rotation matrix.
         *
         * @return *this.
         */
        template<class Tp, class Sp>
        inline matrix& operator =(const abstract_quaternion<Tp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** A typedef for the super class. */
        typedef abstract_matrix<T, 4, L, T[4 * 4]> super;

    };


    /*
     * the::math::matrix<T, 4, L>::matrix
     */
    template<class T, matrix_layout L>
    matrix<T, 4, L>::matrix(const T& value) : super() {
        THE_STACK_TRACE;
        for (unsigned int i = 0; i < 4 * 4; i++) {
            this->components[i] = value;
        }
    }


    /*
     * the::math::matrix<T, 4, L>::~matrix
     */
    template<class T, matrix_layout L>
    matrix<T, 4, L>::~matrix(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_MATRIX_H_INCLUDED */
