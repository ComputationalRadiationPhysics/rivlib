/*
 * the/math/shallow_vector.h
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
 * Shallowvector.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller. Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_SHALLOW_VECTOR_H_INCLUDED
#define THE_MATH_SHALLOW_VECTOR_H_INCLUDED
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
     * This is the implementation of an abstract_vector that uses its a 
     * user-provided memory block for its data. The user-provided memory
     * block is interpreted as vector components. Note, that this class has
     * the inherent problem of producing aliases. You should only use it, if you
     * really require vector operations on memory external to the vector
     * objects.
     *
     * See documentation of abstract_vector for further information about the 
     * vector classes.
     */
    template<class T, unsigned int D> 
    class shallow_vector : public abstract_vector<T, D, T *> {

    public:

        /**
         * Create a new vector initialised using 'components' as data. The
         * vector will operate on these data. The caller is responsible that
         * the memory designated by 'components' lives as long as the object
         * and all its aliases exist.
         *
         * @param components The initial vector memory. This must not be a NULL
         *                   pointer.
         */
        explicit inline shallow_vector(T *components) {
            THE_STACK_TRACE;
            assert(components != NULL);
            this->components = components;
        }

        /**
         * Clone 'rhs'. This operation will create an alias of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline shallow_vector(const shallow_vector& rhs) {
            THE_STACK_TRACE;
            this->components = rhs.components;
        }

        /** Dtor. */
        ~shallow_vector(void);

        /**
         * Set a new component pointer. The vector uses from this point the
         * memory designated by 'components' instead of the memory passed to
         * the ctor or in previous calls to this method for its operations.
         * The caller remains owner of the memory designated by 'components'
         * and must ensure that it lives as long as this object and all its
         * aliases live.
         *
         * @param components The new vector component memory. This must not be
         *                   a NULL vector.
         */
//#pragma deprecated(SetComponents)
//        inline void SetComponents(T *components) {
//            assert(components != NULL);
//            this->components = components;
//        }

        /**
         * Assignment.
         *
         * This operation does <b>not</b> create aliases. 
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        inline shallow_vector& operator =(const shallow_vector& rhs) {
            THE_STACK_TRACE;
            abstract_vector<T, D, T *>::operator =(rhs);
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
        inline shallow_vector& operator =(
                const abstract_vector<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            abstract_vector<T, D, T *>::operator =(rhs);
            return *this;
        }

    private:

        /** 
         * Forbidden ctor. A default ctor would be inherently unsafe for
         * shallow vectors.
         */
        inline shallow_vector(void) {}
    };


    /*
     * shallow_vector<T, D>::~shallow_vector
     */
    template<class T, unsigned int D>
    shallow_vector<T, D>::~shallow_vector(void) {
        THE_STACK_TRACE;
    }

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_SHALLOW_VECTOR_H_INCLUDED */
