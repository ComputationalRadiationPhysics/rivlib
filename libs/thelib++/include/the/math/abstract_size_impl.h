/*
 * the/math/abstract_size_impl.h
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
 * AbstractextImpl.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_SIZE_IMPL_H_INCLUDED
#define THE_MATH_ABSTRACT_SIZE_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/assert.h"
#include "the/math/functions.h"
#include "the/index_out_of_range_exception.h"
#include "the/stack_trace.h"
#include <memory.h>


namespace the {
namespace math {

    /**
     * This template implements the most parts of the size behaviour. Its
     * only intended use is being the super class of the abstract_size 
     * template and its partial template specialisation. The same reuse of code
     * as in abstract_vector_impl is the reason for this class.
     *
     * The following template parameters are used:
     *
     * T: The type used for the components of the size.
     * D: The dimensionality of the size, e. g. 2 for representing 2D sizes.
     * S: The "storage class". This can be either T[D] for a "deep size" or
     *    T * for a "shallow size". Other instantiations are inherently 
     *    dangerous and should never be used.
     * C: The direct subclass, i. e. abstract_size. This allows the 
     *    implementation to create the required return values. Other 
     *    instantiations are inherently dangerous and should never be used.
     */
    template<class T, unsigned int D, class S, 
            template<class T, unsigned int D, class S> class C> 
            class abstract_size_impl {

    public:

        /** Dtor. */
        ~abstract_size_impl(void);

        /**
         * Access the internal ext data directly.
         *
         * @return A pointer to the ext data.
         */
        inline const T *peek_size(void) const {
            THE_STACK_TRACE;
            return this->ext;
        }

        /**
         * Access the internal ext data directly.
         *
         * @return A pointer to the ext data.
         */
        inline T *peek_size(void) {
            THE_STACK_TRACE;
            return this->ext;
        }

        /**
         * Perform a uniform scale of the ext.
         *
         * @param factor The scaling factor.
         */
        void scale(const double factor);

        /**
         * Assignment operator. This operator never creates an alias.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        abstract_size_impl& operator =(const C<T, D, S>& rhs);

        /** 
         * This operator allows for arbitrary ext to ext 
         * assignments. If the left hand side operand has a smaller ext,
         * the values will be truncated, if it has larger extality, it 
         * will be padded with zeroes.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Tp, unsigned int Dp, class Sp>
        abstract_size_impl& operator =(const C<Tp, Dp, Sp>& rhs);

        /**
         * Answer, whether this ext and rhs are equal. The IsEqual 
         * function is used for comparing the components.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are equal.
         */
        bool operator ==(const C<T, D, S>& rhs) const;

        /**
         * Answer, whether this ext and rhs are equal. The IsEqual 
         * function of the left hand operand is used for comparing the 
         * components. If D and Dp do not match, the exts are
         * never equal.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are equal.
         */
        template<class Tp, unsigned int Dp, class Sp>
        bool operator ==(const C<Tp, Dp, Sp>& rhs) const;

        /**
         * Test for inequality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are not equal.
         */
        inline bool operator !=(const C<T, D, S>& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Test for inequality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are not equal.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline bool operator !=(const C<Tp, Dp, Sp>& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Component access. 'i' must be within [0, D[.
         *
         * @param i The component to be accessed.
         *
         * @return The component's value.
         *
         * @throws out_of_range_exception If 'i' is out of range.
         */
        T& operator [](const int i);

        /**
         * Component access. 'i' must be within [0, D[.
         *
         * @param i The component to be accessed.
         *
         * @return The component's value.
         *
         * @throws out_of_range_exception If 'i' is out of range.
         */
        const T& operator [](const int i) const;

    protected:

        /**
         * Disallow instances of this class.
         */
        inline abstract_size_impl(void) {
            THE_STACK_TRACE;
        }

        /** The sizes wrapped by this class. */
        S ext;
    };


    /*
     * the::math::abstract_size_impl<T, D, S, C>::~abstract_size_impl
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    abstract_size_impl<T, D, S, C>::~abstract_size_impl(void) {
        THE_STACK_TRACE;
    }


    /*
     * the::math::abstract_size_impl<T, D, S, C>::scale
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    void abstract_size_impl<T, D, S, C>::scale(const double factor) {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->ext[d] = static_cast<T>(factor 
                * static_cast<double>(this->ext[d]));
        }
    }


    /* 
     * the::math::abstract_size_impl<T, D, S, C>::operator =
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    abstract_size_impl<T, D, S, C>& 
    abstract_size_impl<T, D, S, C>::operator =(const C<T, D, S>& rhs) {
        THE_STACK_TRACE;
        if (this != &rhs) {
            ::memcpy(this->ext, rhs.ext, D * sizeof(T));
        }

        return *this;
    }


    /* 
     * the::math::abstract_size_impl<T, D, S, C>::operator =
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, unsigned int Dp, class Sp> 
    abstract_size_impl<T, D, S, C>& 
    abstract_size_impl<T, D, S, C>::operator =(const C<Tp, Dp, Sp>& rhs) {
        THE_STACK_TRACE;
        if (static_cast<void *>(this) != static_cast<const void *>(&rhs)) {
            for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
                this->ext[d] = static_cast<T>(rhs[d]);
            }
            for (unsigned int d = Dp; d < D; d++) {
                this->ext[d] = static_cast<T>(0);
            }   
        }

        return *this;
    }


    /*
     * the::math::abstract_size_impl<T, D, S, C>::operator ==
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    bool abstract_size_impl<T, D, S, C>::operator ==(
            const C<T, D, S>& rhs) const {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            if (!is_equal(this->ext[d], rhs.ext[d])) {
                return false;
            }
        }
        /* No difference found. */

        return true;
    }


    /*
     * the::math::abstract_size_impl<T, D, S, C>::operator ==
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, unsigned int Dp, class Sp>
    bool abstract_size_impl<T, D, S, C>::operator ==(
            const C<Tp, Dp, Sp>& rhs) const {
        THE_STACK_TRACE;
        if (D != Dp) {
            /* Cannot be equal. */
            return false;
        }

        for (unsigned int d = 0; d < D; d++) {
            if (!is_equal<T>(this->ext[d], static_cast<T>(rhs[d]))) {
                return false;
            }
        }
        /* No difference found. */

        return true;
    }


    /*
     * the::math::abstract_size_impl<T, D, S, C>::operator []
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    T& abstract_size_impl<T, D, S, C>::operator [](const int i) {
        THE_STACK_TRACE;
        ASSERT(0 <= i);
        ASSERT(i < static_cast<int>(D));

        if ((0 <= i) && (i < static_cast<int>(D))) {
            return this->ext[i];
        } else {
            throw index_out_of_range_exception(i, 0, D - 1, __FILE__, __LINE__);
        }
    }


    /*
     * the::math::abstract_size_impl<T, D, S, C>::operator []
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    const T& abstract_size_impl<T, D, S, C>::operator [](const int i) const {
        THE_STACK_TRACE;
        ASSERT(0 <= i);
        ASSERT(i < static_cast<int>(D));

        if ((0 <= i) && (i < static_cast<int>(D))) {
            return this->ext[i];
        } else {
            throw index_out_of_range_exception(i, 0, D - 1, __FILE__, __LINE__);
        }
    }
    
} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_SIZE_IMPL_H_INCLUDED */
