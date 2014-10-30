/*
 * the/math/abstract_matrix.h
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
 * abstract_matrix.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_MATRIX_H_INCLUDED
#define THE_MATH_ABSTRACT_MATRIX_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/math/abstract_matrix_impl.h"
#include "the/invalid_operation_exception.h"
#include "the/math/quaternion.h"
#include "the/math/types.h"


namespace the {
namespace math {

    /**
     * all matrix implementations must inherit from this class. Do not inherit
     * directly from abstract_matrix_impl as the abstraction layer of 
     * abstract_matrix ensures that the implementation can work correctly and
     * instantiate derived classes.
     */
    template<class T, unsigned int D, matrix_layout L, class S>
    class abstract_matrix 
            : public abstract_matrix_impl<T, D, L, S, abstract_matrix> {

    public:

        /** Dtor. */
        ~abstract_matrix(void);

        /**
         * Calculates the characteristic polynom of the matrix
         *
         * @return The characteristic polynom of the matrix
         *
         * @throw Exception if the calculation of the polynom fails.
         */
        inline polynom<T, D> characteristic_polynom(void) const {
            THE_STACK_TRACE;
            return super::characteristic_polynom();
        }

        /**
         * answer the determinant of this matrix.
         *
         * Note that the implementation uses a Gaussian elimination and is 
         * therefore very slow.
         *
         * @return The determinant of the matrix.
         */
        T determinant(void) const;

        /**
         * Calculates eigenvalues and eigenvectors of the matrix. The order of
         * eigenvalues is undefined. The eigenvectors will be ordered like the
         * eigenvalues. at most 'size' results will be written to the output.
         * a DxD matrix has a most D unique real eigenvalues.
         *
         * @param outEigenvalues Pointer to the array receiving the found
         *                       eigenvalues. If null, no eigenvalues will be
         *                       stored.
         * @param outEigenvectors Pointer to the array receiving the found
         *                        eigenvectors. If null, no eigenvectors will
         *                        be stored.
         * @param size The size of 'outEigenvalues' and 'outEigenvectors' in
         *             number of elements.
         *
         * @return The number of results written to the output arrays.
         */
        unsigned int find_eigenvalues(T *outEigenvalues,
            vector<T, D> *outEigenvectors, unsigned int size) const;

        /**
         * assignment operator.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline abstract_matrix& operator =(const abstract_matrix& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * assigment for arbitrary matrices. a valid static_cast between T and 
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
        inline abstract_matrix& operator =(
                const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** a typedef for the super class. */
        typedef abstract_matrix_impl<T, D, L, S, the::math::abstract_matrix>
            super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_matrix(void) : super() {
            THE_STACK_TRACE;
        }

    private:

        /**
         * allow abstract_matrix_impl to assign from itself to the abstract_matrix
         * subclass. This is required for implementing serveral arithmetic 
         * operations in abstract_matrix_impl, which must initialise their return
         * value by copying themselves.
         *
         * This ctor is private as it should only be used on deep-storage 
         * instantiations. Shallow storage instantiations MUST NEVER ExPOSE OR 
         * USE this ctor.
         *
         * @param rhs The object to be cloned.
         */
        template<class S1>
        inline abstract_matrix(const abstract_matrix_impl<T, D, L, S1, 
                the::math::abstract_matrix>& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.PeekComponents(), 
                super::CNT_COMPONENTS * sizeof(T));
        }

        /* allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, matrix_layout Lf1, class Sf1,
            template<class Tf2, unsigned int Df2, matrix_layout Lf2, class Sf2> 
            class Cf>
            friend class abstract_matrix_impl;

    };


    /*
     * the::math::abstract_matrix<T, D, L, S>::~abstract_matrix
     */
    template<class T, unsigned int D, matrix_layout L, class S>
    abstract_matrix<T, D, L, S>::~abstract_matrix(void) {
        THE_STACK_TRACE;
    }


    /*
     * abstract_matrix<T, D, L, S>::find_eigenvalues
     */
    template<class T, unsigned int D, matrix_layout L, class S>
    unsigned int abstract_matrix<T, D, L, S>::find_eigenvalues(
            T *outEigenvalues, vector<T, D> *outEigenvectors,
            unsigned int size) const {
        THE_STACK_TRACE;
        if (this->IsSymmetric()) {
            return this->find_eigenvalues_sym(outEigenvalues,
                outEigenvectors, size);
        }
        if (outEigenvectors == NULL) {
            return this->characteristic_polynom().find_roots(
                outEigenvalues, size);
        }

        // TODO: Implement something better

        throw the::invalid_operation_exception("find_eigenvalues",
            __FILE__, __LINE__);

        return 0;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::determinant
     */
    template<class T, unsigned int D, matrix_layout L, class S>
    T abstract_matrix<T, D, L, S>::determinant(void) const {
        THE_STACK_TRACE;
#define a(r, c) a[(r) * D + (c)]
        double a[D * D];                    // input matrix for algorithm
        double f;                           // Multiplication factor.
        double max;                         // Row pivotising.
        unsigned int pRow;                  // Pivot row.
        unsigned int s;                     // Current eliminination step.
        T retval = static_cast<T>(1);       // The resulting determinant.

        /*
         * Create double precision row-major matrix copy as well-defined basis
         * for Gau� elimination. 
         */
        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = 0; c < D; c++) {
                a(r, c) = static_cast<double>(
                    this->components[super::index_of(r, c)]);
            }
        }

        /* Gau� elimination. */
        s = 0;
        do {

            /* Pivotising. */
            max = ::fabs(a(s, s));
            pRow = s; 
            for (unsigned int r = s + 1; r < D; r++) {
                if (::fabs(a(r, s)) > max) {
                    max = ::fabs(a(r, s));
                    pRow = r;
                }
            }

            if (max < the::math::double_epsilon) {
                /*
                 * Matrix is not invertable, because the column cannot be 
                 * deleted. The determinant is zero, iff the matrix is not
                 * invertable.
                 */
                return static_cast<T>(0);
            }

            if (pRow != s) {
                // if necessary, exchange the row
                double h;

                for (unsigned int c = s ; c < D; c++) {
                    h = a(s, c);
                    a(s, c) = a(pRow, c);
                    a(pRow, c) = h;
                }

                retval *= -1.0; // Exchaning rows changes sign.
            } 

            /* Elimination. */
            for (unsigned int r = s + 1; r < D; r++ ) {
                f = -a(r, s) / a(s, s);
                for (unsigned int c = s; c < D; c++) {
                    a(r, c) += f * a(s, c);
                } 
            }

            s++;
        } while (s < D);

        /* Compute determinant as product of the diagonal. */
        assert(D > 0);
        assert(::fabs(retval) == 1.0);
        for (unsigned int i = 0; i < D; i++) {
            retval *= a(i, i);
        }

        return retval;
#undef a
    }


    /**
     * Partial template specialisation for 2x2 matrices.
     */
    template<class T, matrix_layout L, class S>
    class abstract_matrix<T, 2, L, S>
            : public abstract_matrix_impl<T, 2, L, S, abstract_matrix> {

    public:

        /** Dtor. */
        ~abstract_matrix(void);

        /**
         * Calculates the characteristic polynom of the matrix
         *
         * @return The characteristic polynom of the matrix
         *
         * @throw Exception if the calculation of the polynom fails.
         */
        inline polynom<T, 2> characteristic_polynom(void) const {
            THE_STACK_TRACE;
            polynom<T, 2> rv;
            // x^2 - trace(a)x + det(a)
            rv[0] = super::determinant2x2(
                this->components[super::index_of(0, 0)],
                this->components[super::index_of(1, 0)],
                this->components[super::index_of(0, 1)],
                this->components[super::index_of(1, 1)]);
            rv[1] = -(this->components[super::index_of(0, 0)]
                + this->components[super::index_of(1, 1)]);
            rv[2] = static_cast<T>(1);
            return rv;
        }

        /**
         * answer the determinant of this matrix.
         *
         * @return The determinant of the matrix.
         */
        inline T determinant(void) const {
            THE_STACK_TRACE;
            return super::determinant2x2(
                this->components[super::index_of(0, 0)],
                this->components[super::index_of(1, 0)],
                this->components[super::index_of(0, 1)],
                this->components[super::index_of(1, 1)]);
        }

        /**
         * Calculates eigenvalues and eigenvectors of the matrix. The order of
         * eigenvalues is undefined. The eigenvectors will be ordered like the
         * eigenvalues. at most 'size' results will be written to the output.
         * a DxD matrix has a most D unique real eigenvalues.
         *
         * @param outEigenvalues Pointer to the array receiving the found
         *                       eigenvalues. If null, no eigenvalues will be
         *                       stored.
         * @param outEigenvectors Pointer to the array receiving the found
         *                        eigenvectors. If null, no eigenvectors will
         *                        be stored.
         * @param size The size of 'outEigenvalues' and 'outEigenvectors' in
         *             number of elements.
         *
         * @return The number of results written to the output arrays.
         */
        unsigned int find_eigenvalues(T *outEigenvalues,
            vector<T, 2> *outEigenvectors, unsigned int size) const;

        /**
         * assignment operator.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline abstract_matrix& operator =(const abstract_matrix& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * assigment for arbitrary matrices. a valid static_cast between T and 
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
        inline abstract_matrix& operator =(
                const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** a typedef for the super class. */
        typedef abstract_matrix_impl<T, 2, L, S, the::math::abstract_matrix>
            super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_matrix(void) : super() {
            THE_STACK_TRACE;
        }

        /**
         * allow abstract_matrix_impl to assign from itself to the abstract_matrix
         * subclass. This is required for implementing serveral arithmetic 
         * operations in abstract_matrix_impl, which must initialise their return
         * value by copying themselves.
         *
         * This ctor is private as it should only be used on deep-storage 
         * instantiations. Shallow storage instantiations MUST NEVER ExPOSE OR 
         * USE this ctor.
         *
         * @param rhs The object to be cloned.
         */
        template<class S1>
        inline abstract_matrix(const abstract_matrix_impl<T, 2, L, S1, 
                the::math::abstract_matrix>& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.PeekComponents(), 
                super::CNT_COMPONENTS * sizeof(T));
        }

        /* allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, matrix_layout Lf1, class Sf1,
            template<class Tf2, unsigned int Df2, matrix_layout Lf2, class Sf2> 
            class Cf>
            friend class abstract_matrix_impl;

    };


    /*
     * the::math::abstract_matrix<T, 2, L, S>::~abstract_matrix
     */
    template<class T, matrix_layout L, class S>
    abstract_matrix<T, 2, L, S>::~abstract_matrix(void) {
        THE_STACK_TRACE;
    }


    /*
     * abstract_matrix<T, 2, L, S, C>::find_eigenvalues
     */
    template<class T, matrix_layout L, class S>
    unsigned int abstract_matrix<T, 2, L, S>::find_eigenvalues(
            T *outEigenvalues, vector<T, 2> *outEigenvectors,
            unsigned int size) const {
        THE_STACK_TRACE;
        if (((outEigenvalues == NULL) && (outEigenvectors == NULL))
            || (size == 0)) return 0;

        // implementation based on:
        // http://www.iazd.uni-hannover.de/~erne/Mathematik1/dateien/maple/
        //    MB_5_2.html

        T ev[2];
        T evv[2][2];
        unsigned int evc = this->characteristic_polynom().find_roots(ev, 2);
        if (evc == 0) return 0; // no eigenvalues
        if (evc == 1) ev[1] = ev[0];

        if (is_equal(this->components[super::indexof(1, 0)],
                static_cast<T>(0))) {
            if (is_equal(this->components[super::indexof(0, 1)],
                    static_cast<T>(0))) {
                evv[0][0] = static_cast<T>(1);
                evv[0][1] = static_cast<T>(0);
                evv[1][0] = static_cast<T>(0);
                evv[1][1] = static_cast<T>(1);
            } else {
                evv[0][0] = ev[0] - this->components[super::indexof(1, 1)];
                evv[0][1] = this->components[super::indexof(1, 0)];
                evv[1][0] = ev[1] - this->components[super::indexof(1, 1)];
                evv[1][1] = this->components[super::indexof(1, 0)];
            }
        } else {
            evv[0][0] = this->components[super::indexof(0, 1)];
            evv[0][1] = ev[0] - this->components[super::indexof(0, 0)];
            evv[1][0] = this->components[super::indexof(0, 1)];
            evv[1][1] = ev[1] - this->components[super::indexof(0, 0)];
        }

        if (outEigenvalues != NULL) {
            outEigenvalues[0] = ev[0];
            if (size > 1) {
                outEigenvalues[1] = ev[1];
            }
        }

        if (outEigenvectors != NULL) {
            outEigenvectors[0].set(evv[0][0], evv[0][1]);
            if (size > 1) {
                outEigenvectors[1].set(evv[1][0], evv[1][1]);
            }
        }

        return (size > 1) ? 2 : 1;
    }


    /**
     * Partial template specialisation for 3x3 matrices.
     */
    template<class T, matrix_layout L, class S>
    class abstract_matrix<T, 3, L, S>
            : public abstract_matrix_impl<T, 3, L, S, abstract_matrix> {

    public:

        /** Dtor. */
        ~abstract_matrix(void);

        /**
         * Calculates the characteristic polynom of the matrix
         *
         * @return The characteristic polynom of the matrix
         *
         * @throw Exception if the calculation of the polynom fails.
         */
        inline polynom<T, 3> characteristic_polynom(void) const {
            THE_STACK_TRACE;
            polynom<T, 3> p;
            p[0] = this->determinant();
            p[1] = -(
                super::determinant2x2(
                    this->components[super::index_of(1, 1)],
                    this->components[super::index_of(2, 1)],
                    this->components[super::index_of(1, 2)],
                    this->components[super::index_of(2, 2)])
                + super::determinant2x2(
                    this->components[super::index_of(0, 0)],
                    this->components[super::index_of(2, 0)],
                    this->components[super::index_of(0, 2)],
                    this->components[super::index_of(2, 2)])
                + super::determinant2x2(
                    this->components[super::index_of(0, 0)],
                    this->components[super::index_of(1, 0)],
                    this->components[super::index_of(0, 1)],
                    this->components[super::index_of(1, 1)]));
            p[2] = this->trace();
            p[3] = static_cast<T>(-1);
            return p;
        }

        /**
         * answer the determinant of this matrix.
         *
         * @return The determinant of the matrix.
         */
        inline T determinant(void) const {
            THE_STACK_TRACE;
            return super::determinant3x3(
                this->components[super::index_of(0, 0)],
                this->components[super::index_of(1, 0)],
                this->components[super::index_of(2, 0)],
                this->components[super::index_of(0, 1)],
                this->components[super::index_of(1, 1)],
                this->components[super::index_of(2, 1)],
                this->components[super::index_of(0, 2)],
                this->components[super::index_of(1, 2)],
                this->components[super::index_of(2, 2)]);
        }

        /**
         * Calculates eigenvalues and eigenvectors of the matrix. The order of
         * eigenvalues is undefined. The eigenvectors will be ordered like the
         * eigenvalues. at most 'size' results will be written to the output.
         * a DxD matrix has a most D unique real eigenvalues.
         *
         * @param outEigenvalues Pointer to the array receiving the found
         *                       eigenvalues. If null, no eigenvalues will be
         *                       stored.
         * @param outEigenvectors Pointer to the array receiving the found
         *                        eigenvectors. If null, no eigenvectors will
         *                        be stored.
         * @param size The size of 'outEigenvalues' and 'outEigenvectors' in
         *             number of elements.
         *
         * @return The number of results written to the output arrays.
         */
        unsigned int find_eigenvalues(T *outEigenvalues,
            vector<T, 3> *outEigenvectors, unsigned int size) const;

        /**
         * answer if this matrix describes a pure rotation. This is the case
         * if the matrix is orthogonal and has a determinant of one.
         *
         * @return true, if this matrix describes a pure rotation.
         */
        bool is_rotation(void) const;

        /**
         * assignment operator.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline abstract_matrix& operator =(const abstract_matrix& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * assigment for arbitrary matrices. a valid static_cast between T and 
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
        inline abstract_matrix& operator =(
                const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) {
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
        abstract_matrix& operator =(const abstract_quaternion<Tp, Sp>& rhs);

        /**
         * answer the quaternion representing the rotation of this matrix.
         *
         * @return a quaternion representing the rotation.
         *
         * @throw illegal_state_exception if the matrix is not a rotation-only
         *                              matrix.
         */
        operator quaternion<T>(void) const;

    protected:

        /** a typedef for the super class. */
        typedef abstract_matrix_impl<T, 3, L, S, the::math::abstract_matrix>
            super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_matrix(void) : super() {
            THE_STACK_TRACE;
        }

        /**
         * allow abstract_matrix_impl to assign from itself to the abstract_matrix
         * subclass. This is required for implementing serveral arithmetic 
         * operations in abstract_matrix_impl, which must initialise their return
         * value by copying themselves.
         *
         * This ctor is private as it should only be used on deep-storage 
         * instantiations. Shallow storage instantiations MUST NEVER ExPOSE OR 
         * USE this ctor.
         *
         * @param rhs The object to be cloned.
         */
        template<class S1>
        inline abstract_matrix(const abstract_matrix_impl<T, 3, L, S1, 
                the::math::abstract_matrix>& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.PeekComponents(), 
                super::CNT_COMPONENTS * sizeof(T));
        }

        /* allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, matrix_layout Lf1, class Sf1,
            template<class Tf2, unsigned int Df2, matrix_layout Lf2, class Sf2> 
            class Cf>
            friend class abstract_matrix_impl;

    };


    /*
     * the::math::abstract_matrix<T, 3, L, S>::~abstract_matrix
     */
    template<class T, matrix_layout L, class S>
    abstract_matrix<T, 3, L, S>::~abstract_matrix(void) {
        THE_STACK_TRACE;
    }


    /*
     * abstract_matrix<T, 3, L, S>::find_eigenvalues
     */
    template<class T, matrix_layout L, class S>
    unsigned int abstract_matrix<T, 3, L, S>::find_eigenvalues(
            T *outEigenvalues, vector<T, 3> *outEigenvectors,
            unsigned int size) const {
        THE_STACK_TRACE;
        if (this->is_symmetric()) {
            return this->find_eigenvalues_sym(outEigenvalues,
                outEigenvectors, size);
        }
        if (outEigenvectors == NULL) {
            return this->characteristic_polynom().find_roots(
                outEigenvalues, size);
        }

        // TODO: Implement something better

        throw the::invalid_operation_exception("find_eigenvalues",
            __FILE__, __LINE__);

        return 0;
    }


    /*
     * abstract_matrix<T, 3, L, S>::is_rotation
     */
    template<class T, matrix_layout L, class S>
    bool abstract_matrix<T, 3, L, S>::is_rotation(void) const {
        THE_STACK_TRACE;
        return is_equal(super::determinant3x3(
                this->components[super::index_of(0, 0)],
                this->components[super::index_of(1, 0)],
                this->components[super::index_of(2, 0)],
                this->components[super::index_of(0, 1)],
                this->components[super::index_of(1, 1)],
                this->components[super::index_of(2, 1)],
                this->components[super::index_of(0, 2)],
                this->components[super::index_of(1, 2)],
                this->components[super::index_of(2, 2)]), static_cast<T>(1))
            && is_equal((this->components[super::index_of(0, 0)]
                    *   this->components[super::index_of(0, 0)])
                + (     this->components[super::index_of(1, 0)]
                    *   this->components[super::index_of(1, 0)])
                + (     this->components[super::index_of(2, 0)]
                    *   this->components[super::index_of(2, 0)]),
                static_cast<T>(1))
            && is_equal((this->components[super::index_of(0, 0)]
                    *   this->components[super::index_of(0, 1)])
                + (     this->components[super::index_of(1, 0)]
                    *   this->components[super::index_of(1, 1)])
                + (     this->components[super::index_of(2, 0)]
                    *   this->components[super::index_of(2, 1)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 0)]
                    *   this->components[super::index_of(0, 2)])
                + (     this->components[super::index_of(1, 0)]
                    *   this->components[super::index_of(1, 2)])
                + (     this->components[super::index_of(2, 0)]
                    *   this->components[super::index_of(2, 2)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 1)]
                    *   this->components[super::index_of(0, 0)])
                + (     this->components[super::index_of(1, 1)]
                    *   this->components[super::index_of(1, 0)])
                + (     this->components[super::index_of(2, 1)]
                    *   this->components[super::index_of(2, 0)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 1)]
                    *   this->components[super::index_of(0, 1)])
                + (     this->components[super::index_of(1, 1)]
                    *   this->components[super::index_of(1, 1)])
                + (     this->components[super::index_of(2, 1)]
                    *   this->components[super::index_of(2, 1)]),
                static_cast<T>(1))
            && is_equal((this->components[super::index_of(0, 1)]
                    *   this->components[super::index_of(0, 2)])
                + (     this->components[super::index_of(1, 1)]
                    *   this->components[super::index_of(1, 2)])
                + (     this->components[super::index_of(2, 1)]
                    *   this->components[super::index_of(2, 2)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 2)]
                    *   this->components[super::index_of(0, 0)])
                + (     this->components[super::index_of(1, 2)]
                    *   this->components[super::index_of(1, 0)])
                + (     this->components[super::index_of(2, 2)]
                    *   this->components[super::index_of(2, 0)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 2)]
                    *   this->components[super::index_of(0, 1)])
                + (     this->components[super::index_of(1, 2)]
                    *   this->components[super::index_of(1, 1)])
                + (     this->components[super::index_of(2, 2)]
                    *   this->components[super::index_of(2, 1)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 2)]
                    *   this->components[super::index_of(0, 2)])
                + (     this->components[super::index_of(1, 2)]
                    *   this->components[super::index_of(1, 2)])
                + (     this->components[super::index_of(2, 2)]
                    *   this->components[super::index_of(2, 2)]),
                static_cast<T>(1));
    }


    /*
     * abstract_matrix<T, 3, L, S>::operator =
     */
    template<class T, matrix_layout L, class S>
    template<class Tp, class Sp>
    abstract_matrix<T, 3, L, S>& abstract_matrix<T, 3, L, S>::operator =(
            const abstract_quaternion<Tp, Sp>& rhs) {
        THE_STACK_TRACE;
        quaternion<T> q(rhs);
        q.normalise();

        this->components[super::index_of(0, 0)] 
            = sqr(q.w()) + sqr(q.x()) - sqr(q.y()) - sqr(q.z());
        this->components[super::index_of(0, 1)] 
            = static_cast<T>(2) * (q.x() * q.y() - q.w() * q.z());
        this->components[super::index_of(0, 2)] 
            = static_cast<T>(2) * (q.w() * q.y() + q.x() * q.z()); 

        this->components[super::index_of(1, 0)] 
            = static_cast<T>(2) * (q.w() * q.z() + q.x() * q.y());
        this->components[super::index_of(1, 1)] 
            = sqr(q.w()) - sqr(q.x()) + sqr(q.y()) - sqr(q.z());
        this->components[super::index_of(1, 2)] 
            = static_cast<T>(2) * (q.y() * q.z() - q.w() * q.x());

        this->components[super::index_of(2, 0)] 
            = static_cast<T>(2) * (q.x() * q.z() - q.w() * q.y());
        this->components[super::index_of(2, 1)] 
            = static_cast<T>(2) * (q.w() * q.x() + q.y() * q.z());
        this->components[super::index_of(2, 2)] 
            = sqr(q.w()) - sqr(q.x()) - sqr(q.y()) + sqr(q.z());

        return *this;
    }


    /*
     * the::math::abstract_matrix<T, 3, L, S>::operator Quaternion<T>
     */
    template<class T, matrix_layout L, class S>
    abstract_matrix<T, 3, L, S>::operator quaternion<T>(void) const {
        THE_STACK_TRACE;
        quaternion<T> q;
        if (!this->is_rotation()) {
            throw invalid_operation_exception("Matrix is not rotation-only", __FILE__, __LINE__);
        }
        try {
            q.set_from_rotation_matrix(this->components[super::index_of(0, 0)],
                this->components[super::index_of(0, 1)],
                this->components[super::index_of(0, 2)],
                this->components[super::index_of(1, 0)],
                this->components[super::index_of(1, 1)],
                this->components[super::index_of(1, 2)],
                this->components[super::index_of(2, 0)],
                this->components[super::index_of(2, 1)],
                this->components[super::index_of(2, 2)]);
        } catch(...) {
            throw invalid_operation_exception("Matrix is not rotation-only", __FILE__, __LINE__);
        }
        return q;
    }


    /**
     * Partial template specialisation for 4x4 matrices.
     */
    template<class T, matrix_layout L, class S>
    class abstract_matrix<T, 4, L, S>
            : public abstract_matrix_impl<T, 4, L, S, abstract_matrix> {

    public:

        /** Dtor. */
        ~abstract_matrix(void);

        /**
         * Calculates the characteristic polynom of the matrix
         *
         * @return The characteristic polynom of the matrix
         *
         * @throw Exception if the calculation of the polynom fails.
         */
        inline polynom<T, 4> characteristic_polynom(void) const {
            THE_STACK_TRACE;

            // TODO: Implement something better

            return super::characteristic_polynom();
        }

        /**
         * answer the determinant of this matrix.
         *
         * @return The determinant of the matrix.
         */
        inline T determinant(void) const {
            THE_STACK_TRACE;
            return super::determinant4x4(
                this->components[super::index_of(0, 0)],
                this->components[super::index_of(1, 0)],
                this->components[super::index_of(2, 0)],
                this->components[super::index_of(3, 0)],
                this->components[super::index_of(0, 1)],
                this->components[super::index_of(1, 1)],
                this->components[super::index_of(2, 1)],
                this->components[super::index_of(3, 1)],
                this->components[super::index_of(0, 2)],
                this->components[super::index_of(1, 2)],
                this->components[super::index_of(2, 2)],
                this->components[super::index_of(3, 2)],
                this->components[super::index_of(0, 3)],
                this->components[super::index_of(1, 3)],
                this->components[super::index_of(2, 3)],
                this->components[super::index_of(3, 3)]);
        }

        /**
         * Calculates eigenvalues and eigenvectors of the matrix. The order of
         * eigenvalues is undefined. The eigenvectors will be ordered like the
         * eigenvalues. at most 'size' results will be written to the output.
         * a DxD matrix has a most D unique real eigenvalues.
         *
         * @param outEigenvalues Pointer to the array receiving the found
         *                       eigenvalues. If null, no eigenvalues will be
         *                       stored.
         * @param outEigenvectors Pointer to the array receiving the found
         *                        eigenvectors. If null, no eigenvectors will
         *                        be stored.
         * @param size The size of 'outEigenvalues' and 'outEigenvectors' in
         *             number of elements.
         *
         * @return The number of results written to the output arrays.
         */
        unsigned int find_eigenvalues(T *outEigenvalues,
            vector<T, 4> *outEigenvectors, unsigned int size) const;

        /**
         * answer if this matrix describes a pure rotation. This is the case
         * if the upper left 3x3 matrix is orthogonal and has a determinant of
         * one, and the remaining components are equal to those from the
         * identity matrix.
         *
         * @return true, if this matrix describes a pure rotation.
         */
        bool is_rotation(void) const;

        /**
         * assignment operator.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline abstract_matrix& operator =(const abstract_matrix& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /**
         * assigment for arbitrary matrices. a valid static_cast between T and 
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
        inline abstract_matrix& operator =(
                const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) {
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
        abstract_matrix& operator =(const abstract_quaternion<Tp, Sp>& rhs);

        /**
         * answer the quaternion representing the rotation of this matrix.
         *
         * @return a quaternion representing the rotation.
         *
         * @throw illegal_state_exception if the matrix is not a rotation-only
         *                              matrix.
         */
        operator quaternion<T>(void) const;

    protected:

        /** a typedef for the super class. */
        typedef abstract_matrix_impl<T, 4, L, S, the::math::abstract_matrix>
            super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_matrix(void) : super() {
            THE_STACK_TRACE;
        }

        /**
         * allow abstract_matrix_impl to assign from itself to the abstract_matrix
         * subclass. This is required for implementing serveral arithmetic 
         * operations in abstract_matrix_impl, which must initialise their return
         * value by copying themselves.
         *
         * This ctor is private as it should only be used on deep-storage 
         * instantiations. Shallow storage instantiations MUST NEVER ExPOSE OR 
         * USE this ctor.
         *
         * @param rhs The object to be cloned.
         */
        template<class S1>
        inline abstract_matrix(const abstract_matrix_impl<T, 4, L, S1, 
                the::math::abstract_matrix>& rhs) : super() {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.peek_components(), 
                super::CNT_COMPONENTS * sizeof(T));
        }

        /* allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, matrix_layout Lf1, class Sf1,
            template<class Tf2, unsigned int Df2, matrix_layout Lf2, class Sf2> 
            class Cf>
            friend class abstract_matrix_impl;

    };


    /*
     * the::math::abstract_matrix<T, 4, L, S>::~abstract_matrix
     */
    template<class T, matrix_layout L, class S>
    abstract_matrix<T, 4, L, S>::~abstract_matrix(void) {
        THE_STACK_TRACE;
    }


    /*
     * abstract_matrix<T, 4, L, S>::find_eigenvalues
     */
    template<class T, matrix_layout L, class S>
    unsigned int abstract_matrix<T, 4, L, S>::find_eigenvalues(
            T *outEigenvalues, vector<T, 4> *outEigenvectors,
            unsigned int size) const {
        THE_STACK_TRACE;
        if (this->is_symmetric()) {
            return this->find_eigenvalues_sym(outEigenvalues,
                outEigenvectors, size);
        }
        if (outEigenvectors == NULL) {
            return this->characteristic_polynom().find_roots(
                outEigenvalues, size);
        }

        // TODO: Implement something better

        throw the::invalid_operation_exception("find_eigenvalues",
            __FILE__, __LINE__);

        return 0;
    }


    /*
     * abstract_matrix<T, 4, L, S>::find_eigenvalues
     */
    template<class T, matrix_layout L, class S>
    bool abstract_matrix<T, 4, L, S>::is_rotation(void) const {
        THE_STACK_TRACE;
        return is_equal(super::determinant3x3(
                this->components[super::index_of(0, 0)],
                this->components[super::index_of(1, 0)],
                this->components[super::index_of(2, 0)],
                this->components[super::index_of(0, 1)],
                this->components[super::index_of(1, 1)],
                this->components[super::index_of(2, 1)],
                this->components[super::index_of(0, 2)],
                this->components[super::index_of(1, 2)],
                this->components[super::index_of(2, 2)]), static_cast<T>(1))
            && is_equal((this->components[super::index_of(0, 0)]
                    *   this->components[super::index_of(0, 0)])
                + (     this->components[super::index_of(1, 0)]
                    *   this->components[super::index_of(1, 0)])
                + (     this->components[super::index_of(2, 0)]
                    *   this->components[super::index_of(2, 0)]),
                static_cast<T>(1))
            && is_equal((this->components[super::index_of(0, 0)]
                    *   this->components[super::index_of(0, 1)])
                + (     this->components[super::index_of(1, 0)]
                    *   this->components[super::index_of(1, 1)])
                + (     this->components[super::index_of(2, 0)]
                    *   this->components[super::index_of(2, 1)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 0)]
                    *   this->components[super::index_of(0, 2)])
                + (     this->components[super::index_of(1, 0)]
                    *   this->components[super::index_of(1, 2)])
                + (     this->components[super::index_of(2, 0)]
                    *   this->components[super::index_of(2, 2)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 1)]
                    *   this->components[super::index_of(0, 0)])
                + (     this->components[super::index_of(1, 1)]
                    *   this->components[super::index_of(1, 0)])
                + (     this->components[super::index_of(2, 1)]
                    *   this->components[super::index_of(2, 0)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 1)]
                    *   this->components[super::index_of(0, 1)])
                + (     this->components[super::index_of(1, 1)]
                    *   this->components[super::index_of(1, 1)])
                + (     this->components[super::index_of(2, 1)]
                    *   this->components[super::index_of(2, 1)]),
                static_cast<T>(1))
            && is_equal((this->components[super::index_of(0, 1)]
                    *   this->components[super::index_of(0, 2)])
                + (     this->components[super::index_of(1, 1)]
                    *   this->components[super::index_of(1, 2)])
                + (     this->components[super::index_of(2, 1)]
                    *   this->components[super::index_of(2, 2)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 2)]
                    *   this->components[super::index_of(0, 0)])
                + (     this->components[super::index_of(1, 2)]
                    *   this->components[super::index_of(1, 0)])
                + (     this->components[super::index_of(2, 2)]
                    *   this->components[super::index_of(2, 0)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 2)]
                    *   this->components[super::index_of(0, 1)])
                + (     this->components[super::index_of(1, 2)]
                    *   this->components[super::index_of(1, 1)])
                + (     this->components[super::index_of(2, 2)]
                    *   this->components[super::index_of(2, 1)]),
                static_cast<T>(0))
            && is_equal((this->components[super::index_of(0, 2)]
                    *   this->components[super::index_of(0, 2)])
                + (     this->components[super::index_of(1, 2)]
                    *   this->components[super::index_of(1, 2)])
                + (     this->components[super::index_of(2, 2)]
                    *   this->components[super::index_of(2, 2)]),
                static_cast<T>(1))
            && is_equal(this->components[super::index_of(3, 0)],
                static_cast<T>(0))
            && is_equal(this->components[super::index_of(3, 1)],
                static_cast<T>(0))
            && is_equal(this->components[super::index_of(3, 2)],
                static_cast<T>(0))
            && is_equal(this->components[super::index_of(3, 3)],
                static_cast<T>(1))
            && is_equal(this->components[super::index_of(2, 3)],
                static_cast<T>(0))
            && is_equal(this->components[super::index_of(1, 3)],
                static_cast<T>(0))
            && is_equal(this->components[super::index_of(0, 3)],
                static_cast<T>(0));
    }


    /*
     * abstract_matrix<T, 4, L, S>::operator =
     */
    template<class T, matrix_layout L, class S>
    template<class Tp, class Sp>
    abstract_matrix<T, 4, L, S>& abstract_matrix<T, 4, L, S>::operator =(
            const abstract_quaternion<Tp, Sp>& rhs) {
        THE_STACK_TRACE;
        quaternion<T> q(rhs);
        q.normalise();

        this->components[super::index_of(0, 0)] 
            = sqr(q.w()) + sqr(q.x()) - sqr(q.y()) - sqr(q.z());
        this->components[super::index_of(0, 1)] 
            = static_cast<T>(2) * (q.x() * q.y() - q.w() * q.z());
        this->components[super::index_of(0, 2)] 
            = static_cast<T>(2) * (q.w() * q.y() + q.x() * q.z()); 
        this->components[super::index_of(0, 3)] = static_cast<T>(0);

        this->components[super::index_of(1, 0)] 
            = static_cast<T>(2) * (q.w() * q.z() + q.x() * q.y());
        this->components[super::index_of(1, 1)] 
            = sqr(q.w()) - sqr(q.x()) + sqr(q.y()) - sqr(q.z());
        this->components[super::index_of(1, 2)] 
            = static_cast<T>(2) * (q.y() * q.z() - q.w() * q.x());
        this->components[super::index_of(1, 3)] = static_cast<T>(0);

        this->components[super::index_of(2, 0)] 
            = static_cast<T>(2) * (q.x() * q.z() - q.w() * q.y());
        this->components[super::index_of(2, 1)] 
            = static_cast<T>(2) * (q.w() * q.x() + q.y() * q.z());
        this->components[super::index_of(2, 2)] 
            = sqr(q.w()) - sqr(q.x()) - sqr(q.y()) + sqr(q.z());
        this->components[super::index_of(2, 3)] = static_cast<T>(0);

        this->components[super::index_of(3, 0)] = static_cast<T>(0);
        this->components[super::index_of(3, 1)] = static_cast<T>(0);
        this->components[super::index_of(3, 2)] = static_cast<T>(0);
        this->components[super::index_of(3, 3)] = static_cast<T>(1);

        return *this;
    }

    /*
     * the::math::abstract_matrix<T, 4, L, S>::operator quaternion<T>
     */
    template<class T, matrix_layout L, class S>
    abstract_matrix<T, 4, L, S>::operator quaternion<T>(void) const {
        THE_STACK_TRACE;
        quaternion<T> q;
        if (!this->is_rotation()) {
            throw invalid_operation_exception("Matrix is not rotation-only", __FILE__, __LINE__);
        }
        try {
            q.set_from_rotation_matrix(this->components[super::index_of(0, 0)],
                this->components[super::index_of(0, 1)],
                this->components[super::index_of(0, 2)],
                this->components[super::index_of(1, 0)],
                this->components[super::index_of(1, 1)],
                this->components[super::index_of(1, 2)],
                this->components[super::index_of(2, 0)],
                this->components[super::index_of(2, 1)],
                this->components[super::index_of(2, 2)]);
        } catch(...) {
            throw invalid_operation_exception("Matrix is not rotation-only", __FILE__, __LINE__);
        }
        return q;
    }

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_MATRIX_H_INCLUDED */
