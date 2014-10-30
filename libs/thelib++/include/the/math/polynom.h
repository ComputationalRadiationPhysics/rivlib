/*
 * the/math/polynom.h
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
 * polynom.h
 *
 * Copyright (C) 2006 - 2010 by Visualisierungsinstitut Universitaet Stuttgart. 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_POLYNOM_H_INCLUDED
#define THE_MATH_POLYNOM_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/math/abstract_polynom.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * One-dimensional polynom of degree d.
     *
     * The one-dimensional polynom is defined by its coefficients a_0 ... a_d
     * as:
     *  f(x) := a_d * x^d + a_{d-1} * x^{d-1} + ... + a_1 * x + a_0
     *
     * T scalar type
     * D Degree of the polynom
     */
    template<class T, unsigned int D>
    class polynom : public abstract_polynom<T, D, T[D + 1]> {
    public:

        /** Ctor. */
        polynom(void) : super() {
            THE_STACK_TRACE;
            for (unsigned int i = 0; i <= D; i++) {
                this->coefficients[i] = static_cast<T>(0);
            }
        }

        /**
         * Copy ctor
         *
         * @param src The object to clone from
         */
        template<class Tp, class Sp>
        polynom(const abstract_polynom<Tp, D, Sp> src) : super() {
            THE_STACK_TRACE;
            super::operator=(src);
        }

        /**
         * Ctor.
         *
         * @param coefficients Pointer to the coefficients of the polynom.
         *                     Must be an array of type T and size (D + 1).
         *                     The values will be copied.
         */
        explicit polynom(const T* coefficients) : super() {
            THE_STACK_TRACE;
            for (unsigned int i = 0; i <= D; i++) {
                this->coefficients[i] = coefficients[i];
            }
        }

        /** Dtor. */
        ~polynom(void);

        /**
         * Assignment operator
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         *
         * @throw IllegalParamException if 'rhs' has an effective degree
         *        larger than D.
         */
        template<class Tp, unsigned int Dp>
        inline polynom<T, D>& operator=(const polynom<Tp, Dp>& rhs) {
            THE_STACK_TRACE;
            super::operator=(rhs);
            return *this;
        }

    protected:

        /** A typedef for the super class. */
        typedef abstract_polynom<T, D, T[D + 1]> super;

    };


    /*
     * polynom<T, D>::~polynom
     */
    template<class T, unsigned int D>
    polynom<T, D>::~polynom(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_POLYNOM_H_INCLUDED */
