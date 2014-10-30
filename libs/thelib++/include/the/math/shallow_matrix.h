/*
 * the/math/shallow_matrix.h
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
 * ShallowMatrix.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_SHALLOW_MATRIX_H_INCLUDED
#define THE_MATH_SHALLOW_MATRIX_H_INCLUDED
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
     * This specialisation implements a "shallow matrix", i. e. a matrix object
     * that does not own the memory of its components. You can use this class
     * e. g. to reinterpret memory received over a network as a matrix without
     * copying the data. You are responsible for providing the data memory as
     * long as the object lives. If you just want to have a easy-to-use matrix 
     * that manages its memory itself, consider using the Matrix class.
     */
    template<class T, unsigned int D, matrix_layout L> 
    class shallow_matrix : public abstract_matrix<T, D, L, T *> {

    public:

        /**
         * Create a new matrix initialised using 'components' as data. The
         * matrix will operate on these data. The caller is responsible that
         * the memory designated by 'components' lives as long as the object
         * and all its aliases exist.
         *
         * @param components The initial matrix memory. This must not be a NULL
         *                   pointer.
         */
        explicit inline shallow_matrix(T *components) {
            THE_STACK_TRACE;
            ASSERT(components != NULL);
            this->components = components;
        }

        /**
         * Clone 'rhs'. This operation will create an alias of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline shallow_matrix(const shallow_matrix& rhs) {
            THE_STACK_TRACE;
            this->components = rhs.components;
        }

        /** Dtor. */
        ~shallow_matrix(void);

        /**
         * Assignment operator.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline shallow_matrix& operator =(const shallow_matrix& rhs) {
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
        inline shallow_matrix& operator =(
                const abstract_matrix<Tp, Dp, Lp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }


    protected:

        /** A typedef for the super class. */
        typedef abstract_matrix<T, D, L, T *> super;

        /** 
         * Forbidden default ctor. 
         */
        inline shallow_matrix(void) {
            THE_STACK_TRACE;
        }

    };

    /*
     * the::math::shallow_matrix<T, D, L>::~shallow_matrix
     */
    template<class T, unsigned int D, matrix_layout L>
    shallow_matrix<T, D, L>::~shallow_matrix(void) {
        THE_STACK_TRACE;
    }

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_SHALLOW_MATRIX_H_INCLUDED */
