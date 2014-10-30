/*
 * the/math/abstract_matrix_impl.h
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
 * AbstractMatrixImpl.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_MATRIX_IMPL_H_INCLUDED
#define THE_MATH_ABSTRACT_MATRIX_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <iomanip>
#include <ios>
#include <iostream>

#include "the/assert.h"
#include "the/exception.h"
#include "the/force_inline.h"
#include "the/memory.h"
#include "the/index_out_of_range_exception.h"
#include "the/string.h"
#include "the/stack_trace.h"

#include "the/math/functions.h"
#include "the/math/point.h"
#include "the/math/polynom.h"
#include "the/math/shallow_vector.h"
#include "the/math/vector.h"


namespace the {
namespace math {

    /**
     * Possible matrix memory layouts.
     *
     * matrix_layout::column_major means that the columns are stored one after another, i. e.
     * the rows are stored contiguously. This is memory layout is used by
     * OpenGL.
     *
     * matrix_layout::row_major means that the rows are stored one after another, i. e. the
     * columns are stored contiguosly. This layout is used for multidimensional
     * arrays in C and in Direct3D.
     */
    enum class matrix_layout {
        column_major = 1,
        row_major
    };

    /**
     * Implementation of matrix behaviour. Do not use this class directly. It is
     * used to implement the same inheritance pattern as for vectors, points 
     * etc. See the documentation of abstract_vectorImpl for further details.
     *
     * T scalar type.
     * D Dimension of the matrix
     * L Matrix memory layout
     * S Matrix storage
     * C Deriving subclass.
     */
    template<class T, unsigned int D, matrix_layout L, class S,
            template<class T, unsigned int D, matrix_layout L, class S> class C>
    class abstract_matrix_impl {

    public:

        /** 
         * Typedef for a matrix with "deep storage" class. Objects of this type
         * are used as return value for methods and operators that must create
         * and return new instances.
         */
        typedef C<T, D, L, T[D * D]> deep_storage_matrix;

        /** Dtor. */
        ~abstract_matrix_impl(void);

        /**
         * dump the matrix to the specified stream.
         *
         * @param out The stream to dump the matrix to.
         */
        void dump(std::ostream& out) const;

        /**
         * Get the matrix component at the specified position.
         *
         * @param row The row to return.
         * @param col The column to return.
         *
         * @return The matrix value at 'row', 'col'.
         *
         * @throws out_of_range_exception If 'row' and 'col' does not designate a 
         *         valid matrix component within [0, D[.
         */
        inline T get_at(const int row, const int col) const {
            THE_STACK_TRACE;
            return (*this)(row, col);
        }

        /**
         * Get the matrix component at the specified position.
         *
         * @param row The row to return.
         * @param col The column to return.
         *
         * @return a reference to 'row', 'col'.
         *
         * @throws out_of_range_exception If 'row' and 'col' does not designate a 
         *                             valid matrix component within [0, D[.
         */
        inline T& get_at(const int row, const int col) {
            THE_STACK_TRACE;
            return (*this)(row, col);
        }

        /**
         * answer the 'col'th column vector.
         *
         * @param col The index of the column within [0, D[.
         *
         * @return The requested vector.
         *
         * @throws out_of_range_exception If 'col' is not within [0, D[.
         */
        vector<T, D> get_column(const int col) const;

        /**
         * answer the 'row'th row vector.
         *
         * @param row The index of the row within [0, D[.
         *
         * @return The requested vector.
         *
         * @throws out_of_range_exception If 'row' is not within [0, D[.
         */
        vector<T, D> get_row(const int row) const;

        /** 
         * inverts the matrix.
         *
         * Note that the implementation uses a Gaussian elimination and is 
         * therefore very slow.
         *
         * @return true, if the matrix was inverted, false, if the matrix is not
         *         invertable.
         */
        bool invert(void);

        /**
         * answer, whether the matrix is the identity matrix.
         *
         * @return true, if the matrix is the identity matrix, false otherwise.
         */
        bool is_identity(void) const;

        /**
         * answer, whether the matrix is the null matrix.
         *
         * @return true, if the matrix is the null matrix, false otherwise.
         */
        bool is_null(void) const;

        /**
         * answer, whether the matrix is orthogonal.
         * Note: O(n^3) matrix multiplication
         *
         * @return true, if the matrix is orthogonal, false otherwise.
         */
        bool is_orthogonal(void) const;

        /**
         * answer, whether the matrix is symmetric.
         *
         * @return true, if the matrix is symmetric, false otherwise.
         */
        bool is_symmetric(void) const;

        /**
         * Direct access to the matrix components. The object remains owner
         * of the memory designated by the returned pointer.
         *
         * Note that the layout of the matrix components is dependent on the
         * template parameter L.
         *
         * @return a pointer to the matrix components.
         */
        inline const T *peek_components(void) const {
            THE_STACK_TRACE;
            return this->components;
        }

        /**
         * Direct access to the matrix components. The object remains owner
         * of the memory designated by the returned pointer.
         *
         * Note that the layout of the matrix components is dependent on the
         * template parameter L.
         *
         * @return a pointer to the matrix components.
         */
        inline T *peek_components(void) {
            THE_STACK_TRACE;
            return this->components;
        }

        /**
         * Set a new value for a matrix component.
         *
         * @param row   The row to set.
         * @param col   The column to set.
         * @param value The new value at 'row', 'col'.
         *
         * @return The previous value at the updated position.
         */
        void set_at(const int row, const int col, const T value);

        /**
         * Make this matrix the identity matrix.
         */
        void set_identity(void);

        /**
         * Make this matrix a null matrix.
         */
        void set_null(void);

        /**
         * answer the trace (sum over the diagonal) of the matrix.
         *
         * @return The trace of the matrix.
         */
        T trace(void) const;

        /**
         * transposes the matrix.
         */
        void transpose(void);

        /**
         * Componentwise add of this matrix and 'rhs'. The result is assigned
         * to this matrix.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Sp>
        abstract_matrix_impl<T, D, L, S, C>& operator +=(
            const C<T, D, L, Sp>& rhs);

        /**
         * Componentwise add of this matrix and 'rhs'. 
         *
         * @param rhs The right hand side operand.
         *
         * @return The resulting matrix holding the sum of this one and 'rhs'.
         */
        template<class Sp>
        inline deep_storage_matrix operator +(const C<T, D, L, Sp>& rhs) const {
            THE_STACK_TRACE;
            deep_storage_matrix retval = *this;
            retval += rhs;
            return retval;
        }

        /**
         * Componentwise subtract of 'rhs' from this matrix. The result is 
         * assigned to this matrix.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Sp>
        abstract_matrix_impl<T, D, L, S, C>& operator -=(
            const C<T, D, L, Sp>& rhs);

        /**
         * Componentwise subtract of 'rhs' from this matrix.
         *
         * @param rhs The right hand side operand.
         *
         * @return The resulting matrix holding the difference.
         */
        template<class Sp>
        inline deep_storage_matrix operator -(const C<T, D, L, S>& rhs) const {
            THE_STACK_TRACE;
            deep_storage_matrix retval = *this;
            retval -= rhs;
            return retval;
        }

        /**
         * Matrix multiplication. 'rhs' is multiplied with this matrix from
         * the right and the result is assigned to this matrix.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Sp>
        inline abstract_matrix_impl<T, D, L, S, C>& operator *=(
                const C<T, D, L, Sp>& rhs) {
            THE_STACK_TRACE;
            return (*this = *this * rhs);
        }

        /**
         * Matrix multiplication. 'rhs' is multiplied with this matrix from the
         * right side.
         *
         * @param rhs The right hand side operand.
         *
         * @return The product of this matrix and 'rhs'.
         */
        template<class Sp>
        deep_storage_matrix operator *(const C<T, D, L, Sp>& rhs) const;

        /**
         * Scalar multiplication. The result is assigned to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        abstract_matrix_impl<T, D, L, S, C>& operator *=(const T rhs);

        /**
         * Scalar multiplication.
         *
         * @param rhs The right hand side operand.
         *
         * @return 'rhs' * *this.
         */
        inline deep_storage_matrix operator *(const T rhs) const {
            THE_STACK_TRACE;
            deep_storage_matrix retval = *this;
            retval *= rhs;
            return retval;
        }

        /**
         * Componentwise division. The result is assigned to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        abstract_matrix_impl<T, D, L, S, C>& operator /=(const T rhs);

        /**
         * Componentwise division.
         *
         * @param rhs The right hand side operand.
         *
         * @return This matrix having all components divided by 'rhs'.
         */
        inline deep_storage_matrix operator /(const T rhs) const {
            THE_STACK_TRACE;
            deep_storage_matrix retval = *this;
            retval /= rhs;
            return retval;
        }

        /**
         * Multiplies this matrix with the vector 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return The transformed vector.
         */
        template<class Sp>
        vector<T, D> operator *(const abstract_vector<T, D, Sp>& rhs) const;

        /**
         * Extends 'rhs' to be a homogenous vector and multiplies the matrix 
         * with this vector. Before returning the (D - 1) dimensional result,
         * a perspective divide is performed to normalise the last component
         * to 1. This component is discarded in the result.
         *
         * @param rhs The right hand side operand.
         *
         * @return The transformed vector.
         */
        template<class Sp>
        vector<T, D - 1> operator *(
            const abstract_vector<T, D - 1, Sp>& rhs) const;

        /**
         * Multiplies this matrix with the point 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return The transformed point.
         */
        template<class Sp>
        inline point<T, D> operator *(const abstract_point<T, D, Sp>& rhs) const {
            THE_STACK_TRACE;
            shallow_vector<T, D> v(const_cast<double *>(rhs.peek_coordinates()));
            return point<T, D>((*this * v).peek_components());
        }

        /**
         * Extends 'rhs' to be a point with homogenous coordinates and 
         * multiplies the matrix with this point. Before returning the (D - 1)
         * dimensional result, a perspective divide is performed to normalise 
         * the last component to 1. This component is discarded in the result.
         *
         * @param rhs The right hand side operand.
         *
         * @return The transformed point.
         */
        template<class Sp>
        inline point<T, D - 1> operator *(
                const abstract_point<T, D - 1, Sp>& rhs) const {
            THE_STACK_TRACE;
            shallow_vector<T, D - 1> v(const_cast<double *>(
                rhs.peek_coordinates()));
            return point<T, D - 1>((*this * v).peek_components());
        }

        /**
         * assignment operator.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        abstract_matrix_impl<T, D, L, S, C>& operator =(const C<T, D, L, S>& rhs);

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
        inline abstract_matrix_impl<T, D, L, S, C>& operator =(
                const C<Tp, Dp, Lp, Sp>& rhs) {
            THE_STACK_TRACE;
            if (static_cast<void *>(this) != static_cast<const void *>(&rhs)) {
                this->assign(rhs);
            }

            return *this;
        }

        /**
         * Test for equality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are equal, false otherwise.
         */
        bool operator ==(const C<T, D, L, S>& rhs) const;

        /**
         * Test for equality of arbitrary matrices. This operation uses the
         * is_equal function of the left hand side operand. Note that matrices 
         * with different dimensions are never equal.
         *
         * @param rhs The right hand side operand.
         *
         * @param true, if 'rhs' and this point are equal, false otherwise.
         */
        template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
        bool operator ==(const C<Tp, Dp, Lp, Sp>& rhs) const;

        /**
         * Test for inequality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are not equal, false otherwise.
         */
        inline bool operator !=(const C<T, D, L, S>& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Test for inequality of arbitrary matrices. See operator == for further
         * details.
         *
         * @param rhs The right hand side operand.
         *
         * @param true, if 'rhs' and this vector are not equal, false otherwise.
         */
        template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
        inline bool operator !=(const C<Tp, Dp, Lp, Sp>& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        ///**
        // * Directly access the 'i'th component of the matrix. Note, that the 
        // * semantic of the 'i'th component depends on the matrix memory layout
        // * defined by the template parameter L and the matrix dimension D.
        // *
        // * @param i The index of the component within [0, D * D[.
        // *
        // * @return a reference to the 'i'th component.
        // *
        // * @throws out_of_range_exception, if 'i' is not within [0, D * D[.
        // */
        //T& operator [](const int i);

        ///**
        // * Directly access the 'i'th component of the matrix. Note, that the 
        // * semantic of the 'i'th component depends on the matrix memory layout
        // * defined by the template parameter L and the matrix dimension D.
        // *
        // * @param i The index of the component within [0, D * D[.
        // *
        // * @return Tthe 'i'th component.
        // *
        // * @throws out_of_range_exception, if 'i' is not within [0, D * D[.
        // */
        //T operator [](const int i) const;

        /**
         * Get the matrix component at the specified position.
         *
         * @param row The row to return.
         * @param col The column to return.
         *
         * @return The matrix value at 'row', 'col'.
         *
         * @throws out_of_range_exception If 'row' and 'col' does not designate a 
         *         valid matrix component within [0, D[.
         */
        T operator ()(const int row, const int col) const;

        /**
         * Get the matrix component at the specified position.
         *
         * @param row The row to return.
         * @param col The column to return.
         *
         * @return a reference to 'row', 'col'.
         *
         * @throws out_of_range_exception If 'row' and 'col' does not designate a 
         *         valid matrix component within [0, D[.
         */
        T& operator ()(const int row, const int col);

    protected:

        /**
         * Calculates the characteristic polynom of the matrix
         *
         * @return The characteristic polynom of the matrix
         *
         * @throw exception if the calculation of the polynom fails.
         */
        polynom<T, D> characteristic_polynom(void) const;

        /**
         * Calculates the determinant of the 2x2 matrix
         *  a00 a10
         *  a01 a11
         *
         * @param a00 a coefficient of the matrix
         * @param a10 a coefficient of the matrix
         * @param a01 a coefficient of the matrix
         * @param a11 a coefficient of the matrix
         *
         * @return The determinant of the matrix
         */
        static THE_FORCE_INLINE T determinant2x2(const T& a00, const T& a10,
            const T& a01, const T& a11);

        /**
         * Calculates the determinant of the 3x3 matrix
         *  a00 a10 a20
         *  a01 a11 a21
         *  a02 a12 a22
         *
         * @param a00 a coefficient of the matrix
         * @param a10 a coefficient of the matrix
         * @param a20 a coefficient of the matrix
         * @param a01 a coefficient of the matrix
         * @param a11 a coefficient of the matrix
         * @param a21 a coefficient of the matrix
         * @param a02 a coefficient of the matrix
         * @param a12 a coefficient of the matrix
         * @param a22 a coefficient of the matrix
         *
         * @return The determinant of the matrix
         */
        static THE_FORCE_INLINE T determinant3x3(const T& a00, const T& a10,
            const T& a20, const T& a01, const T& a11, const T& a21,
            const T& a02, const T& a12, const T& a22);

        /**
         * Calculates the determinant of the 4x4 matrix
         *  a00 a10 a20 a30
         *  a01 a11 a21 a31
         *  a02 a12 a22 a32
         *  a03 a13 a23 a33
         *
         * @param a00 a coefficient of the matrix
         * @param a10 a coefficient of the matrix
         * @param a20 a coefficient of the matrix
         * @param a30 a coefficient of the matrix
         * @param a01 a coefficient of the matrix
         * @param a11 a coefficient of the matrix
         * @param a21 a coefficient of the matrix
         * @param a31 a coefficient of the matrix
         * @param a02 a coefficient of the matrix
         * @param a12 a coefficient of the matrix
         * @param a22 a coefficient of the matrix
         * @param a32 a coefficient of the matrix
         * @param a03 a coefficient of the matrix
         * @param a13 a coefficient of the matrix
         * @param a23 a coefficient of the matrix
         * @param a33 a coefficient of the matrix
         *
         * @return The determinant of the matrix
         */
        static THE_FORCE_INLINE T determinant4x4(const T& a00, const T& a10,
            const T& a20, const T& a30, const T& a01, const T& a11,
            const T& a21, const T& a31, const T& a02, const T& a12,
            const T& a22, const T& a32, const T& a03, const T& a13,
            const T& a23, const T& a33);

        /**
         * Compute the index of the matrix element at 'row', 'col' depending
         * on the matrix size and layout of the instantiation. No bounds check
         * is done.
         *
         * @param row The zero-based index of the row.
         * @param col The zero-based index of the column.
         *
         * @return The index of the specified element in the components array of
         *         the current instantiation.
         */
        static inline int index_of(const int row, const int col) {
            THE_STACK_TRACE;
            return (L == matrix_layout::column_major) ? (col * D + row) : (row * D + col);
        }

        /**
         * answer the index of 'idx' in a matrix having the opposite memory
         * layout, but the same dimension, i. e. effectively the index of the 
         * transposed element.
         *
         * @param idx The index of a component when using the memory layout 
         *            defined by the template parameter L.
         *
         * @return The index of 'idx' when using the other memory layout.
         */
        static inline int transpose_index(const int idx) {
            THE_STACK_TRACE;
            return (idx % D) * D + (idx / D);
        }

        /** The number of components the matrix consists of. */
        static const unsigned int CNT_COMPONENTS;

        /** Ctor. */
        inline abstract_matrix_impl(void) {
            THE_STACK_TRACE;
        };

        /**
         * assign the components of 'rhs' to this matrix for the assignment
         * operator and copy ctor.
         */
        template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
        void assign(const C<Tp, Dp, Lp, Sp>& rhs);

        /**
         * Numerically find the eigenvalues of a SYMMETRIC matrix! Except for
         * this limitation this method behaves like 'FindEigenvalues'.
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
        unsigned int find_eigenvalues_sym(T *outEigenvalues,
            vector<T, D> *outEigenvectors, unsigned int size) const;

        /** 
         * The matrix components. Their memory layout is defined by the
         * template parameter L.
         */
        S components;

        ///**
        // *
        // */
        //template<class Tf1, unsigned int Df1, matrix_layout Lf1, class Sf1,
        //    template<class Tf2, unsigned int Df2, matrix_layout Lf2, class Sf2> 
        //    class Cf1>
        //friend deep_storage_matrix operator *(const T lhs, 
        //    Cf1<Tf2, Df2, Lf2, Sf2> rhs);
    private:

        /**
         * Computes 'sqrt(a * a + b * b)' without destructive underflow or
         * overflow.
         *
         * @param a The first operand
         * @param b The second operand
         *
         * @return The result
         */
        static inline double pythag(double a, double b) {
            THE_STACK_TRACE;
            double absa = ::fabs(a);
            double absb = ::fabs(b);
            if (absa > absb) {
                absb /= absa;
                absb *= absb;
                return absa * sqrt(1.0 + absb);
            }
            if (is_equal(absb, 0.0)) return 0.0;
            absa /= absb;
            absa *= absa;
            return absb * sqrt(1.0 + absa);
        }

    };


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::~abstract_matrix_impl
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    abstract_matrix_impl<T, D, L, S, C>::~abstract_matrix_impl(void) {
        THE_STACK_TRACE;
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::dump
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>   
    void abstract_matrix_impl<T, D, L, S, C>::dump(std::ostream& out) const {
        THE_STACK_TRACE;
        out << std::setiosflags(std::ios::fixed) << std::setprecision(3) 
            << std::setfill(' ');

        for (unsigned int r = 0; r < D; r++) {
            out << "{ ";

            for (unsigned int c = 0; c < D; c++) {
                out << std::setw(7) << this->components[index_of(r, c)];
                
                if (c < D - 1) {
                    out << ", ";
                }
            }
            out << " }" << std::endl;
        }

        out << std::resetiosflags(std::ios::fixed);
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::get_column
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>   
    vector<T, D> abstract_matrix_impl<T, D, L, S, C>::get_column(
            const int col) const {
        THE_STACK_TRACE;
        vector<T, D> retval;

        for (unsigned int r = 0; r < D; r++) {
            retval[r] = this->components[index_of(r, col)];
        }

        return retval;
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::get_row
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>   
    vector<T, D> abstract_matrix_impl<T, D, L, S, C>::get_row(
            const int row) const {
        THE_STACK_TRACE;
        vector<T, D> retval;

        for (unsigned int c = 0; c < D; c++) {
            retval[c] = this->components[index_of(row, c)];
        }

        return retval;
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::invert
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>    
    bool abstract_matrix_impl<T, D, L, S, C>::invert(void) {
        THE_STACK_TRACE;
#define a(r, c) a[(r) * 2 * D + (c)]
        double a[2 * D * D];    // input matrix for algorithm
        double f;               // Multiplication factor.
        double max;             // Row pivotising.
        unsigned int pRow;      // Pivot row.
        unsigned int s;         // Current eliminination step.
        
        /* Create double precision matrix and add identity at the right. */
        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = 0; c < D; c++) {
                a(r, c) = static_cast<double>(this->components[index_of(r, c)]);
            }

            for (unsigned int c = 0; c < D; c++) {
                a(r, c + D) = (r == c) ? 1.0 : 0.0;
            }
        }

        /* Gauss elimination. */
        s = 0;
        do {
            // pivotising avoids unnecessary canceling if a zero is in the 
            // diagonal and increases the precision.
            max = ::fabs(a(s, s));
            pRow = s; 
            for (unsigned int r = s + 1; r < D; r++) {
                if (::fabs(a(r, s)) > max) {
                    max = ::fabs(a(r, s));
                    pRow = r;
                }
            }

            if (max < double_epsilon) {
                return false;   // delete is not possible
            }

            if (pRow != s) {
                // if necessary, exchange the row
                double h;

                for (unsigned int c = s ; c < 2 * D; c++) {
                    h = a(s, c);
                    a(s, c) = a(pRow, c);
                    a(pRow, c) = h;
                }
            } 

            // eliminations row is divided by pivot-coefficient f = a[s][s]
            f = a(s, s);
            for (unsigned int c = s; c < 2 * D; c++) {
                a(s, c) /= f;
            }

            for (unsigned int r = 0; r < D; r++ ) {
                if (r != s) {
                    f = -a(r, s);
                    for (unsigned int c = s; c < 2 * D; c++) {
                        a(r, c) += f * a(s, c);
                    }
                } 
            }

            s++;
        } while (s < D);

        /* Copy identity on the right which is now inverse. */
        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = 0; c < D; c++) { 
                this->components[index_of(r, c)] = static_cast<T>(a(r, D + c));
            }
        }

        return true;
#undef a
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::is_identity
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>    
    bool abstract_matrix_impl<T, D, L, S, C>::is_identity(void) const {
        THE_STACK_TRACE;
        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = 0; c < D; c++) {
                if (!is_equal(this->components[index_of(r, c)], 
                        static_cast<T>((r == c) ? 1 : 0))) {
                    return false;
                }
            } /* end for (unsigned int c = 0; c < D; c++) */
        } /* end for (unsigned int r = 0; r < D; r++) */

        return true;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::is_null
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>   
    bool abstract_matrix_impl<T, D, L, S, C>::is_null(void) const {
        THE_STACK_TRACE;
        for (unsigned int c = 0; c < CNT_COMPONENTS; c++) {
            if (!is_equal(this->components[c], static_cast<T>(0))) {
                return false;
            }
        }

        return true;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::is_orthogonal
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>   
    bool abstract_matrix_impl<T, D, L, S, C>::is_orthogonal(void) const {
        THE_STACK_TRACE;
        // Test: a * a^T = I

        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = 0; c < D; c++) {
                T t = static_cast<T>(0);
                for (unsigned int i = 0; i < D; i++) {
                    t += this->components[index_of(r, i)]
                        * this->components[index_of(i, c)];
                }
                if (!is_equal(t, static_cast<T>((r == c) ? 1 : 0))) {
                    return false;
                }
            }
        }

        return true;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::is_symmetric
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>   
    bool abstract_matrix_impl<T, D, L, S, C>::is_symmetric(void) const {
        THE_STACK_TRACE;
        for (unsigned int r = 1; r < D; r++) {
            for (unsigned int c = 0; c < r; c++) {
                if (!is_equal(this->components[index_of(r, c)], 
                        this->components[index_of(c, r)])) {
                    return false;
                }
            } /* end for (unsigned int c = 0; c < D; c++) */
        } /* end for (unsigned int r = 0; r < D; r++) */
        return true;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::set_at
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    void abstract_matrix_impl<T, D, L, S, C>::set_at(const int row, const int col,
            const T value) {
        THE_STACK_TRACE;

        int idx = index_of(row, col);

        if ((idx >= 0) && (idx < static_cast<int>(CNT_COMPONENTS))) {
            this->components[idx] = value;

        } else {
            throw index_out_of_range_exception(idx, 0, CNT_COMPONENTS - 1, __FILE__, 
                __LINE__);
        }        
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::set_identity
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    void abstract_matrix_impl<T, D, L, S, C>::set_identity(void) {
        THE_STACK_TRACE;
        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = 0; c < D; c++) {
                this->components[index_of(r, c)] 
                    = static_cast<T>((r == c) ? 1 : 0);
            }
        }
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::set_null
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    void abstract_matrix_impl<T, D, L, S, C>::set_null(void) {
        THE_STACK_TRACE;
        for (unsigned int i = 0; i < CNT_COMPONENTS; i++) {
            this->components[i] = static_cast<T>(0);
        }
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::trace
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>  
    T abstract_matrix_impl<T, D, L, S, C>::trace(void) const {
        THE_STACK_TRACE;
        T retval = static_cast<T>(0);

        for (unsigned int i = 0; i < D; i++) {
            retval += this->components[index_of(i, i)];
        }

        return retval;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::transpose
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>  
    void abstract_matrix_impl<T, D, L, S, C>::transpose(void) {
        THE_STACK_TRACE;
        T tmp;
        int idx1, idx2;

        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = r; c < D; c++) {
                idx1 = index_of(r, c);
                idx2 = index_of(c, r);
                tmp = this->components[idx1];
                this->components[idx1] = this->components[idx2];
                this->components[idx2] = tmp;
            }
        }
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::operator +=
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>  
    template<class Sp>
    abstract_matrix_impl<T, D, L, S, C>& 
    abstract_matrix_impl<T, D, L, S, C>::operator +=(const C<T, D, L, Sp>& rhs) {
        THE_STACK_TRACE;
        for (unsigned int c = 0; c < CNT_COMPONENTS; c++) {
            this->components[c] += rhs.peek_components()[c];
        }

        return *this;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::operator -=
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>  
    template<class Sp>
    abstract_matrix_impl<T, D, L, S, C>& 
    abstract_matrix_impl<T, D, L, S, C>::operator -=(const C<T, D, L, Sp>& rhs) {
        THE_STACK_TRACE;
        for (unsigned int c = 0; c < CNT_COMPONENTS; c++) {
            this->components[c] -= rhs.peek_components()[c];
        }

        return *this;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::operator *
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>  
    template<class Sp>
    typename abstract_matrix_impl<T, D, L, S, C>::deep_storage_matrix
    abstract_matrix_impl<T, D, L, S, C>::operator *(
            const C<T, D, L, Sp>& rhs) const {
        THE_STACK_TRACE;
        deep_storage_matrix retval; 
        const T *rhsComps = rhs.peek_components();
        T *retvalComps = retval.peek_components();

        // TODO: Not so nice ...
        retval.set_null();

        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = 0; c < D; c++) {
                for (unsigned int i = 0; i < D; i++) {
                    retvalComps[index_of(r, c)] 
                        += this->components[index_of(r, i)]
                        * rhsComps[index_of(i, c)];
                }
            }
        }

        return retval;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::operator *=
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>  
    abstract_matrix_impl<T, D, L, S, C>& 
    abstract_matrix_impl<T, D, L, S, C>::operator *=(const T rhs) {
        THE_STACK_TRACE;
        for (unsigned int c = 0; c < CNT_COMPONENTS; c++) {
            this->components[c] *= rhs;
        }

        return *this;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::operator /=
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>  
    abstract_matrix_impl<T, D, L, S, C>& 
    abstract_matrix_impl<T, D, L, S, C>::operator /=(const T rhs) {
        THE_STACK_TRACE;
        for (unsigned int c = 0; c < CNT_COMPONENTS; c++) {
            this->components[c] /= rhs;
        }

        return *this;
    }


    /* 
     * the::math::abstract_matrix_impl<T, D, L, S, C>::operator *
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>  
    template<class Sp>
    vector<T, D> abstract_matrix_impl<T, D, L, S, C>::operator *(
            const abstract_vector<T, D, Sp>& rhs) const {
        THE_STACK_TRACE;
        vector<T, D> retval;
        assert(retval.is_null());

        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = 0; c < D; c++) {
                retval[r] += this->components[index_of(r, c)] * rhs[c];
            }
        }

        return retval;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::operator *
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    template<class Sp>
    vector<T, D - 1> abstract_matrix_impl<T, D, L, S, C>::operator *(
            const abstract_vector<T, D - 1, Sp>& rhs) const {
        THE_STACK_TRACE;
        vector<T, D - 1> retval;
        assert(retval.is_null());

        /* Compute w-component of result. */
        T w = this->components[index_of(D - 1, D - 1)];
        for (unsigned int i = 0; i < D - 1; i++) {
            w += this->components[index_of(D - 1, i)] * rhs[i];
        }

        /* Compute normal product of D - 1 vector. */
        for (unsigned int r = 0; r < D - 1; r++) {
            for (unsigned int c = 0; c < D - 1; c++) {
                retval[r] += this->components[index_of(r, c)] * rhs[c];
            }
        }

        /* Perspective divide. */
        return (retval / w);
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::operator =
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    abstract_matrix_impl<T, D, L, S, C>& 
    abstract_matrix_impl<T, D, L, S, C>::operator =(const C<T, D, L, S>& rhs) {
        THE_STACK_TRACE;

        if (this != &rhs) {
            ::memcpy(this->components, rhs.components, CNT_COMPONENTS 
                * sizeof(T));
        }

        return *this;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::operator ==
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    bool abstract_matrix_impl<T, D, L, S, C>::operator ==(
            const C<T, D, L, S>& rhs) const {
        THE_STACK_TRACE;

        for (unsigned int d = 0; d < CNT_COMPONENTS; d++) {
            if (!is_equal<T>(this->components[d], rhs.components[d])) {
                return false;
            }
        }

        return true;
    }


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::operator ==
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
    bool abstract_matrix_impl<T, D, L, S, C>::operator ==(
            const C<Tp, Dp, Lp, Sp>& rhs) const {
        THE_STACK_TRACE;
        
        if (D != Dp) {
            return false;
        }

        for (unsigned int d = 0; d < CNT_COMPONENTS; d++) {
            if (!is_equal<T>(this->components[d], (L == Lp) ? rhs.components[d] 
                    : rhs.components[transpose_index(d)])) {
                return false;
            }
        }

        return true;        
    }


    ///*
    // * the::math::abstract_matrix_impl<T, D, L, S, C>::operator []
    // */
    //template<class T, unsigned int D, matrix_layout L, class S,
    //    template<class T, unsigned int D, matrix_layout L, class S> class C>
    //T& abstract_matrix_impl<T, D, L, S, C>::operator [](const int i) {

    //    if ((i >= 0) && (i < static_cast<int>(CNT_COMPONENTS))) {
    //        return this->components[i];
    //    } else {
    //        throw out_of_range_exception(i, 0, D - 1, __FILE__, __LINE__);
    //    }
    //}


    ///*
    // * the::math::abstract_matrix_impl<T, D, L, S, C>::operator []
    // */
    //template<class T, unsigned int D, matrix_layout L, class S,
    //    template<class T, unsigned int D, matrix_layout L, class S> class C>
    //T abstract_matrix_impl<T, D, L, S, C>::operator [](const int i) const {

    //    if ((i >= 0) && (i < static_cast<int>(CNT_COMPONENTS))) {
    //        return this->components[i];
    //    } else {
    //        throw out_of_range_exception(i, 0, D - 1, __FILE__, __LINE__);
    //    }
    //}

    /*
     * abstract_matrix_impl<T, D, L, S, C>::operator ()
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    T abstract_matrix_impl<T, D, L, S, C>::operator ()(const int row, 
            const int col) const {
        THE_STACK_TRACE;

        int idx = index_of(row, col);

        if ((idx >= 0) && (idx < static_cast<int>(CNT_COMPONENTS))) {
            return this->components[idx];
        } else {
            throw index_out_of_range_exception(idx, 0, CNT_COMPONENTS - 1,
                __FILE__, __LINE__);
        }
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::operator ()
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    T& abstract_matrix_impl<T, D, L, S, C>::operator ()(const int row, 
            const int col) {
        THE_STACK_TRACE;
        int idx = index_of(row, col);

        if ((idx >= 0) && (idx < static_cast<int>(CNT_COMPONENTS))) {
            return this->components[idx];
        } else {
            throw index_out_of_range_exception(idx, 0, CNT_COMPONENTS - 1,
                __FILE__, __LINE__);
        }
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::CNT_COMPONENTS
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    const unsigned int abstract_matrix_impl<T, D, L, S, C>::CNT_COMPONENTS 
        = D * D;


    /*
     * the::math::abstract_matrix_impl<T, D, L, S, C>::assign
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    template<class Tp, unsigned int Dp, matrix_layout Lp, class Sp>
    void abstract_matrix_impl<T, D, L, S, C>::assign(
            const C<Tp, Dp, Lp, Sp>& rhs) {
        THE_STACK_TRACE;

        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = 0; c < D; c++) {
                if ((r < Dp) && (c < Dp)) {
                    this->components[index_of(r, c)] 
                        = static_cast<T>(rhs.get_at(r, c));
                } else if (r == c) {
                    this->components[index_of(r, c)] = static_cast<T>(1);
                } else {
                    this->components[index_of(r, c)] = static_cast<T>(0);
                } /* end if ((r < Dp) && (c < Dp)) */
            } /* end for (unsigned int c = 0; c < D; c++) */
        } /* end for (unsigned int r = 0; r < D; r++) */
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::characteristic_polynom
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    polynom<T, D>
    abstract_matrix_impl<T, D, L, S, C>::characteristic_polynom(void) const {
        THE_STACK_TRACE;
        // method of Faddejew-Leverrier
        // http://de.wikipedia.org/wiki/algorithmus_von_Faddejew-Leverrier
        polynom<T, D> c;
        deep_storage_matrix B[2];

        B[0].set_null();
        c[D] = static_cast<T>(1);
        B[1].set_null(); // B1 = a * B0 = a * 0 = 0

        for (unsigned int k = 1; k <= D; k++) {
            unsigned int a = k % 2;
            unsigned int b = 1 - a;

            for (unsigned int i = 0; i < D; i++) {
                B[a](i, i) += c[D - k + 1];
            }

            B[b] = (*this);
            B[b] *= B[a];

            c[D - k] = B[b].trace() * static_cast<T>(-1) / static_cast<T>(k);
        }

        B[1 - (D % 2)] = (*this);
        B[0] *= B[1];
        for (unsigned int i = 0; i < D; i++) {
            B[0](i, i) += c[0];
        }

        if (!B[0].is_null()) {
            throw exception("Characteristic polynom calculation failed",
                __FILE__, __LINE__);
        }

        return c;
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::determinant2x2
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    T abstract_matrix_impl<T, D, L, S, C>::determinant2x2(const T& a00,
            const T& a10, const T& a01, const T& a11) {
        THE_STACK_TRACE;
        return a00 * a11 - a01 * a10;
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::determinant3x3
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    T abstract_matrix_impl<T, D, L, S, C>::determinant3x3(const T& a00,
            const T& a10, const T& a20, const T& a01, const T& a11,
            const T& a21, const T& a02, const T& a12, const T& a22) {
        THE_STACK_TRACE;
        // rule of sarrus
        return a00 * a11 * a22 + a01 * a12 * a20 + a02 * a10 * a21
            - a02 * a11 * a20 - a01 * a10 * a22 - a00 * a12 * a21;
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::determinant4x4
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    T abstract_matrix_impl<T, D, L, S, C>::determinant4x4(const T& a00,
            const T& a10, const T& a20, const T& a30, const T& a01,
            const T& a11, const T& a21, const T& a31, const T& a02,
            const T& a12, const T& a22, const T& a32, const T& a03,
            const T& a13, const T& a23, const T& a33) {
        THE_STACK_TRACE;
        // Method: 1 x laplace + sarrus
        return a02
            * determinant3x3(a10, a20, a30, a11, a21, a31, a13, a23, a33)
            - a12
            * determinant3x3(a00, a20, a30, a01, a21, a31, a03, a23, a33)
            + a22
            * determinant3x3(a00, a10, a30, a01, a11, a31, a03, a13, a33)
            - a32
            * determinant3x3(a00, a10, a20, a01, a11, a21, a03, a13, a23);
    }


    /*
     * abstract_matrix_impl<T, D, L, S, C>::find_eigenvalues_sym
     */
    template<class T, unsigned int D, matrix_layout L, class S,
        template<class T, unsigned int D, matrix_layout L, class S> class C>
    unsigned int abstract_matrix_impl<T, D, L, S, C>::find_eigenvalues_sym(T *outEigenvalues,
            vector<T, D> *outEigenvectors, unsigned int size) const {
        THE_STACK_TRACE;

        if (((outEigenvalues == NULL) && (outEigenvectors == NULL))
                || (size == 0)) return 0;

#define a(r, c) a[(r) * D + (c)]
        double a[D * D];                    // input matrix for algorithm
        double d[D];                        // diagonal elements
        double e[D];                        // off-diagonal elements

        for (unsigned int r = 0; r < D; r++) {
            for (unsigned int c = 0; c < D; c++) {
                a(r, c) = static_cast<double>(this->components[index_of(r, c)]);
            }
        }

        // 1. Householder reduction.
        int l, k, j, i;
        double scale, hh, h, g, f;

        for (i = D; i >= 2; i--) {
            l = i - 1;
            h = scale = 0.0;
            if (l > 1) {
                for (k = 1; k <= l; k++) {
                    scale += ::fabs(a(i - 1, k - 1));
                }
                if (is_equal(scale, 0.0)) {
                    e[i - 1] = a(i - 1, l - 1);
                } else {
                    for (k = 1; k <= l; k++) {
                        a(i - 1, k - 1) /= scale;
                        h += a(i - 1, k - 1) * a(i - 1, k - 1);
                    }
                    f = a(i - 1, l - 1);
                    g = ((f >= 0.0) ? -::sqrt(h) : ::sqrt(h));
                    e[i - 1] = scale * g;
                    h -= f * g;
                    a(i - 1, l - 1) = f - g;
                    f = 0.0;
                    for (j = 1; j <= l; j++) {
                        a(j - 1, i - 1) = a(i - 1, j - 1) / h;
                        g = 0.0;
                        for (k = 1; k <= j; k++) {
                            g += a(j - 1, k - 1) * a(i - 1, k - 1);
                        }
                        for (k = j + 1; k <= l; k++) {
                            g += a(k - 1, j - 1) * a(i - 1, k - 1);
                        }
                        e[j - 1] = g / h;
                        f += e[j - 1] * a(i - 1, j - 1);
                    }
                    hh = f / (h + h);
                    for (j = 1; j <= l; j++) {
                        f = a(i - 1, j - 1);
                        e[j - 1] = g = e[j - 1] - hh * f;
                        for (k = 1; k <= j; k++) {
                            a(j - 1, k - 1) -= (f * e[k - 1]
                                + g * a(i - 1, k - 1));
                        }
                    }
                }
            } else {
                e[i - 1] = a(i - 1, l - 1);
            }
            d[i - 1] = h;
        }
        d[0] = 0.0;
        e[0] = 0.0;
        for (i = 1; i <= static_cast<int>(D); i++) {
            l = i - 1;
            if (!is_equal(d[i - 1], 0.0)) {
                for (j = 1; j <= l ; j++) {
                    g = 0.0;
                    for (k = 1; k <= l; k++) {
                        g += a(i - 1, k - 1) * a(k - 1, j - 1);
                    }
                    for (k = 1; k <= l; k++) {
                        a(k - 1, j - 1) -= g * a(k - 1, i - 1);
                    }
                }
            }
            d[i - 1] = a(i - 1, i - 1);
            a(i - 1, i - 1) = 1.0;
            for (j = 1; j <= l; j++) {
                a(j - 1, i - 1) = a(i - 1, j - 1) = 0.0;
            }
        }

        // 2. Calculation von eigenvalues and eigenvectors (QL algorithm)
#ifdef sign
#error sign macro already in use! Code rewrite required!
#endif
#define sign(a, b) ((b) >= 0.0 ? ::fabs(a) : -::fabs(a))

        int m, iter;
        double s, r, p, dd, c, b;
        const int MaX_ITER = 30;

        for (i = 2; i <= static_cast<int>(D); i++) {
            e[i - 2] = e[i - 1];
        }
        e[D - 1] = 0.0;

        for (l = 1; l <= static_cast<int>(D); l++) {
            iter = 0;
            do {
                for (m = l; m <= static_cast<int>(D) - 1; m++) {
                    dd = ::fabs(d[m - 1]) + ::fabs(d[m - 1 + 1]);
                    if (is_equal(::fabs(e[m - 1]) + dd, dd)) break;
                }
                if (m != l) {
                    if (iter++ == MaX_ITER) {
                        throw the::exception(
                            "Too many iterations in FindEigenvalues",
                            __FILE__, __LINE__);
                    }
                    g = (d[l - 1 + 1] - d[l - 1]) / (2.0 * e[l - 1]);
                    r = pythag(g, 1.0);
                    g = d[m - 1] - d[l - 1] + e [l - 1] / (g + sign(r, g));
                    s = c = 1.0;
                    p = 0.0;
                    for (i = m - 1; i >= l ; i--) {
                        f = s * e[i - 1];
                        b = c * e[i - 1];
                        e[i - 1 + 1] = r = pythag(f, g);
                        if (is_equal(r, 0.0)) {
                            d[i - 1 + 1] -= p;
                            e[m - 1] = 0.0;
                            break;
                        }
                        s = f / r;
                        c = g / r;
                        g = d[i - 1 + 1] - p;
                        r = (d[i - 1] - g) * s + 2.0 * c * b;
                        d[i - 1 + 1] = g + (p = s * r);
                        g = c * r - b;
                        for (k = 1; k <= static_cast<int>(D); k++) {
                            f = a(k - 1, i - 1 + 1);
                            a(k - 1, i - 1 + 1) = s * a(k - 1, i - 1) + c * f;
                            a(k - 1, i - 1) = c * a(k - 1, i - 1) - s * f;
                        }
                    }
                    if (is_equal(r, 0.0) && (i >= l)) continue;
                    d[l - 1] -= p;
                    e[l - 1] = g;
                    e[m - 1] = 0.0;
                }
            } while (m != l);
        }
#undef sign

        // 3. output
        if (outEigenvalues != NULL){
            for (i = 0; i < static_cast<int>(minimum(D, size)); i++) {
                outEigenvalues[i] = static_cast<T>(d[i]);
            }
        }
        if (outEigenvectors != NULL){
            for (i = 0; i < static_cast<int>(minimum(D, size)); i++) {
                for (j = 0; j < static_cast<int>(D); j++) {
                    outEigenvectors[i][j] = static_cast<T>(a(i, j));
                }
            }
        }

#undef a
        return minimum(D, size);
    }

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_MATRIX_IMPL_H_INCLUDED */
