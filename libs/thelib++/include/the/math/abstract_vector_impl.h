/*
 * the/math/abstract_vector_impl.h
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
 * AbstractvectorImplImpl.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_VECTOR_IMPL_H_INCLUDED
#define THE_MATH_ABSTRACT_VECTOR_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/config.h"
#include <limits>
#include "the/assert.h"
#include "the/index_out_of_range_exception.h"
#include "the/math/functions.h"
#include "the/memory.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * This template implements the major behaviour of vectors. Its only 
     * intended use is being the super class of the abstractvector template
     * and its partial template specialisation. It should never be used in
     * any other context. It is especially not intended to be an interface!
     *
     * This template has several template parameters that allow instantiations
     * for different scalar types, different dimensions and different storage
     * classes. If the storage class S is T[D], the implementation uses its
     * own memory for storing its data. If S is T *, an instance that uses
     * foreign memory is created - we call this "shallow vectors". This 
     * instantiation is used to make a user defined memory block behave like a
     * vector. The template parameter C must be the direct subclass. It allows
     * this class to create appropriate return values for the arithmetic
     * operations. We use this approach to share this implementation between
     * the primary class template abstractvector and its partial template
     * specialisations.
     *
     * T: The type used for scalars and vector components.
     * D: The dimensions of the vector, which must be an integer greater or
     *    equal 1.
     * S: The "storage class". This can be either T[D] for a "deep vector" or
     *    T * for a "shallow vector". Other instantiations are inherently 
     *    dangerous and should never be used.
     * C: The direct subclass, i. e. abstractvector. This allows the 
     *    implementation to create the required return values.
     */
    template<class T, unsigned int D, class S, 
            template<class T, unsigned int D, class S> class C> 
            class abstract_vector_impl {

    public:

        /** Dtor. */
        ~abstract_vector_impl(void);

        /**
         * Answer the angle between this vector and 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return The angle between this vector and 'rhs'.
         */
        angle_rad_t angle(const C<T, D, S>& rhs) const;

        /**
         * Answer the dot product of this vector and 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return The dot product of this vector and 'rhs'.
         */
        T dot(const C<T, D, S>& rhs) const;

        /**
         * Answer whether the vector is normalised.
         *
         * @return true, if the vector is normalised, false otherwise.
         */
        inline bool is_normalised(void) const {
            THE_STACK_TRACE;
            return is_equal<T>(this->length(), static_cast<T>(1));
        }

        /**
         * Answer whether the vector is a null vector.
         *
         * @return true, if the vector is a null vector, false otherwise.
         */
        bool is_null(void) const;

        /**
         * Answer whether the rhs vector and this are parallel.
         *
         * If both vectors are null vectors, they are not considered to be 
         * parallel and the the return value will be false.
         *
         * @return true, if both vectors are parallel, false otherwise.
         */
        template<class Tp, class Sp>
        bool is_parallel(const C<Tp, D, Sp>& rhs) const;

        /**
         * Answer the length of the vector.
         *
         * @return The length of the vector.
         */
        T length(void) const;

        /**
         * Answer the maximum norm of the vector.
         *
         * @return The maximum norm of the vector.
         */
        T max_norm(void) const;

        /**
         * Answer the euclidean norm (length) of the vector.
         *
         * @return The length of the vector.
         */
        inline T norm(void) const {
            THE_STACK_TRACE;
            return this->length();
        }

        /**
         * normalise the vector.
         *
         * @return The OLD length of the vector.
         */
        T normalise(void);

        /**
         * Directly access the internal pointer holding the vector components.
         * The object remains owner of the memory returned.
         *
         * @return The vector components in an array.
         */
        inline T *peek_components(void) {
            THE_STACK_TRACE;
            return this->components;
        }

        /**
         * Directly access the internal pointer holding the vector components. 
         * The object remains owner of the memory returned.
         *
         * @return The vector components in an array.
         */
        inline const T *peek_components(void) const {
            THE_STACK_TRACE;
            return this->components;
        }

        /**
         * Make this vector a null vector.
         */
        void set_null(void);

        /**
         * Make this vector having a length of 'newlength'.
         *
         * Note that a null vector cannot be scaled and will remain unchanged.
         *
         * @param newlength The new length of the vector.
         *
         * @return The old length of the vector.
         */
        T scale_to_length(const T newlength);

        /**
         * Answer the square of the length of the vector.
         *
         * @return The square of the length of the vector.
         */
        T square_length(void) const;

        /**
         * Answer the taxicab or Manhattan norm.
         *
         * @return The taxicab norm of the vector.
         */
        T taxicab_norm(void) const;

        /**
         * Assignment.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        abstract_vector_impl<T, D, S, C>& operator =(const C<T, D, S>& rhs);

        /**
         * Assigment for arbitrary vectors. A valid static_cast between T and
         * Tp is a precondition for instantiating this template.
         *
         * This operation does <b>not</b> create aliases. 
         *
         * If the two operands have different dimensions, the behaviour is as 
         * follows: If the left hand side operand has lower dimension, the 
         * highest (Dp - D) dimensions are discarded. If the left hand side
         * operand has higher dimension, the missing dimensions are filled with 
         * zero components.
         *
         * Subclasses must ensure that sufficient memory for the 'components' 
         * member has been allocated before calling this operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        template<class Tp, unsigned int Dp, class Sp>
        abstract_vector_impl<T, D, S, C>& operator =(const C<Tp, Dp, Sp>& rhs);

        /**
         * Test for equality. This operation uses the E function which the 
         * template has been instantiated for.
         *
         * @param rhs The right hand side operand.
         *
         * @param true, if 'rhs' and this vector are equal, false otherwise.
         */
        bool operator ==(const C<T, D, S>& rhs) const;

        /**
         * Test for equality of arbitrary vector types. This operation uses the
         * IsEqual function of the left hand side operand. Note that vectors 
         * with different dimensions are never equal.
         *
         * @param rhs The right hand side operand.
         *
         * @param true, if 'rhs' and this vector are equal, false otherwise.
         */
        template<class Tp, unsigned int Dp, class Sp>
        bool operator ==(const C<Tp, Dp, Sp>& rhs) const;

        /**
         * Test for inequality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if 'rhs' and this vector are not equal, false
         *         otherwise.
         */
        inline bool operator !=(const C<T, D, S>& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Test for inequality of arbitrary vectors. See operator == for
         * details.
         * further
         * @param rhs The right hand side operand.
         *
         * @return true, if 'rhs' and this vector are not equal, false
         *         otherwise.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline bool operator !=(const C<Tp, Dp, Sp>& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Negate the vector.
         *
         * @return The negated version of this vector.
         */
        C<T, D, S> operator -(void) const;

        /**
         * Answer the sum of this vector and 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return The sum of this and 'rhs'.
         */
        template<class Tp, class Sp>
        C<T, D, T[D]> operator +(const C<Tp, D, Sp>& rhs) const;

        /**
         * Add 'rhs' to this vector and answer the sum.
         *
         * @param rhs The right hand side operand.
         *
         * @return The sum of this and 'rhs'.
         */
        template<class Tp, class Sp>
        abstract_vector_impl<T, D, S, C>& operator +=(const C<Tp, D, Sp>& rhs);

        /**
         * Answer the difference between this vector and 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return The difference between this and 'rhs'.
         */
        template<class Tp, class Sp>
        C<T, D, T[D]> operator -(const C<Tp, D, Sp>& rhs) const;

        /**
         * Subtract 'rhs' from this vector and answer the difference.
         *
         * @param rhs The right hand side operand.
         *
         * @return The difference between this and 'rhs'.
         */
        template<class Tp, class Sp>
        abstract_vector_impl<T, D, S, C>& operator -=(const C<Tp, D, Sp>& rhs);

        /**
         * Scalar multiplication.
         *
         * @param rhs The right hand side operand.
         *
         * @return The result of the scalar multiplication.
         */
        C<T, D, T[D]> operator *(const T rhs) const;

        /**
         * Scalar multiplication assignment operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return The result of the scalar multiplication.
         */
        abstract_vector_impl<T, D, S, C>& operator *=(const T rhs);

        /**
         * Scalar division operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return The result of the scalar division.
         */
        C<T, D, T[D]> operator /(const T rhs) const;

        /**
         * Scalar division assignment operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return The result of the scalar division.
         */
        abstract_vector_impl<T, D, S, C>& operator /=(const T rhs);

        /**
         * Performs a component-wise multiplication.
         *
         * @param rhs The right hand side operand.
         *
         * @return The product of this and rhs.
         */
        template<class Tp, class Sp>
        C<T, D, T[D]> operator *(const C<Tp, D, Sp>& rhs) const;

        /**
         * Multiplies 'rhs' component-wise with this vector and returns
         * the result.
         *
         * @param rhs The right hand side operand.
         *
         * @return The product of this and rhs
         */
        template<class Tp, class Sp>
        abstract_vector_impl<T, D, S, C>& operator *=(const C<Tp, D, Sp>& rhs);

        /**
         * Component access.
         *
         * @param i The index of the requested component, which must be within
         *          [0, D - 1].
         *
         * @return A reference on the 'i'th component.
         *
         * @throws out_of_range_exception, If 'i' is not within [0, D[.
         */
        T& operator [](const int i);

        /**
         * Component access.
         *
         * @param i The index of the requested component, which must be within
         *          [0, D - 1].
         *
         * @return A reference on the 'i'th component.
         *
         * @throws out_of_range_exception, If 'i' is not within [0, D[.
         */
        const T& operator [](const int i) const;

    protected:

        /**
         * Disallow instances of this class. This ctor does nothing!
         */
        inline abstract_vector_impl(void) {};

        /** 
         * The vector components. This can be a T * pointer or a T[D] static
         * array.
         */
        S components;
    };


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::~abstract_vector_impl
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    abstract_vector_impl<T, D, S, C>::~abstract_vector_impl(void) {
        THE_STACK_TRACE;
    }


    /*
     * abstract_vector_impl<T, D, S, C>::angle
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    angle_rad_t abstract_vector_impl<T, D, S, C>::angle(
            const C<T, D, S>& rhs) const {
        THE_STACK_TRACE;
        C<T, D, T[D]> v1;
        C<T, D, T[D]> v2;

        for (unsigned int d = 0; d < D; d++) {
            v1.components[d] = this->components[d];
            v2.components[d] = rhs.components[d];
        }

        v1.normalise();
        v2.normalise();

        return static_cast<angle_rad_t>(::acos(v1.dot(v2)));
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::dot
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    T abstract_vector_impl<T, D, S, C>::dot(const C<T, D, S>& rhs) const {
        THE_STACK_TRACE;
        T retval = static_cast<T>(0);

        for (unsigned int d = 0; d < D; d++) {
            retval += this->components[d] * rhs.components[d];
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::is_null
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    bool abstract_vector_impl<T, D, S, C>::is_null(void) const {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            if (!is_equal<T>(this->components[d], static_cast<T>(0))) {
                return false;
            }
        }
        /* No non-null value found. */

        return true;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::is_parallel
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    template<class Tp, class Sp>
    bool abstract_vector_impl<T, D, S, C>::is_parallel(
            const C<Tp, D, Sp>& rhs) const {
        THE_STACK_TRACE;
        T factor = static_cast<T>(0); // this = factor * rhs
        bool inited = false; // if factor is initialized

        for (unsigned int d = 0; d < D; d++) {

            if (is_equal<T>(this->components[d], static_cast<T>(0))) {
                // compare component of rhs in type of lhs
                if (!is_equal<T>(rhs.components[d], static_cast<T>(0))) {
                    return false; // would yield to a factor of zero
                }
                // both zero, so go on.

            } else {
                // compare component of rhs in type of lhs
                if (is_equal<T>(rhs.components[d], static_cast<T>(0))) {
                    return false; // would yield to a factor of infinity
                } else {
                    // both not zero, check if factor is const over all
                    // components
                    if (inited) {
                        if (!is_equal<T>(factor, this->components[d] 
                                / static_cast<T>(rhs.components[d]))) {
                            return false;
                        }
                    } else {
                        factor = this->components[d] 
                            / static_cast<T>(rhs.components[d]);
                        inited = true;
                    }
                }
            }
        }

        return inited;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::length
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    T abstract_vector_impl<T, D, S, C>::length(void) const {
        THE_STACK_TRACE;
        T retval = static_cast<T>(0);

        for (unsigned int d = 0; d < D; d++) {
            retval += sqr(this->components[d]);
        }

        return sqrt(retval);
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::max_norm
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    T abstract_vector_impl<T, D, S, C>::max_norm(void) const {
        THE_STACK_TRACE;
#ifdef _MSC_VER
#pragma push_macro("min")
#undef min
#pragma push_macro("max")
#undef max
#endif /* _MSC_VER */
        T retval = std::numeric_limits<T>::is_integer 
            ? std::numeric_limits<T>::min() : -std::numeric_limits<T>::max();
#ifdef _MSC_VER
#pragma pop_macro("min")
#pragma pop_macro("max")
#endif /* _MSC_VER */

        for (unsigned int d = 0; d < D; d++) {
            if (abs(this->components[d]) > retval) {
                retval = abs(this->components[d]);
            }
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::normalise
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    T abstract_vector_impl<T, D, S, C>::normalise(void) {
        THE_STACK_TRACE;
        T length = this->length();

        if (length != static_cast<T>(0)) {
            for (unsigned int d = 0; d < D; d++) {
                this->components[d] /= length;
            }

        } else {
            for (unsigned int d = 0; d < D; d++) {
                this->components[d] = static_cast<T>(0);
            }
        }

        return length;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::set_null
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    void abstract_vector_impl<T, D, S, C>::set_null(void) {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->components[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::scale_to_length
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    T abstract_vector_impl<T, D, S, C>::scale_to_length(const T newlength) {
        THE_STACK_TRACE;
        T retval = this->length();

        if (retval != static_cast<T>(0)) {
            T scaleFactor = newlength / retval;

            for (unsigned int d = 0; d < D; d++) {
                this->components[d] *= scaleFactor;
            }
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::square_length
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    T abstract_vector_impl<T, D, S, C>::square_length(void) const {
        THE_STACK_TRACE;
        T retval = static_cast<T>(0);

        for (unsigned int d = 0; d < D; d++) {
            retval += sqr(this->components[d]);
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::taxicab_norm
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    T abstract_vector_impl<T, D, S, C>::taxicab_norm(void) const {
        THE_STACK_TRACE;
        T retval = static_cast<T>(0);

        for (unsigned int d = 0; d < D; d++) {
            retval += this->components[d];
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator =
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    abstract_vector_impl<T, D, S, C>&
    abstract_vector_impl<T, D, S, C>::operator =(const C<T, D, S>& rhs) {
        THE_STACK_TRACE;

        if (this != &rhs) {
            ::memcpy(this->components, rhs.components, D * sizeof(T));
        }

        return *this;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator =
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    template<class Tp, unsigned int Dp, class Sp>
    abstract_vector_impl<T, D, S, C>&
    abstract_vector_impl<T, D, S, C>::operator =(const C<Tp, Dp, Sp>& rhs) {
        THE_STACK_TRACE;

        if (static_cast<void *>(this) != static_cast<const void *>(&rhs)) {
            for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
                this->components[d] = static_cast<T>(rhs[d]);
            }
            for (unsigned int d = Dp; d < D; d++) {
                this->components[d] = static_cast<T>(0);
            }            
        }

        return *this;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator ==
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    bool abstract_vector_impl<T, D, S, C>::operator ==(
            const C<T, D, S>& rhs) const {
        THE_STACK_TRACE;

        for (unsigned int d = 0; d < D; d++) {
            if (!is_equal<T>(this->components[d], rhs.components[d])) {
                return false;
            }
        }

        return true;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator ==
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    template<class Tp, unsigned int Dp, class Sp>
    bool abstract_vector_impl<T, D, S, C>::operator ==(
            const C<Tp, Dp, Sp>& rhs) const {
        THE_STACK_TRACE;
        if (D != Dp) {
            return false;
        }

        for (unsigned int d = 0; d < D; d++) {
            if (!is_equal<T>(this->components[d], rhs[d])) {
                return false;
            }
        }

        return true;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator -
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    C<T, D, S> abstract_vector_impl<T, D, S, C>::operator -(void) const {
        THE_STACK_TRACE;
        C<T, D, S> retval;

        for (unsigned int d = 0; d < D; d++) {
            retval.components[d] = -this->components[d];
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator +
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    template<class Tp, class Sp>
    C<T, D, T[D]> abstract_vector_impl<T, D, S, C>::operator +(
            const C<Tp, D, Sp>& rhs) const {
        THE_STACK_TRACE;
        C<T, D, T[D]> retval;

        for (unsigned int d = 0; d < D; d++) {
            retval.components[d] = this->components[d] + rhs.components[d];
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator +=
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    template<class Tp, class Sp>
    abstract_vector_impl<T, D, S, C>&
    abstract_vector_impl<T, D, S, C>::operator +=(const C<Tp, D, Sp>& rhs) {
        THE_STACK_TRACE;

        for (unsigned int d = 0; d < D; d++) {
            this->components[d] += rhs.components[d];
        }

        return *this;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator -
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    template<class Tp, class Sp>
    C<T, D, T[D]> abstract_vector_impl<T, D, S, C>::operator -(
            const C<Tp, D, Sp>& rhs) const {
        THE_STACK_TRACE;
        C<T, D, T[D]> retval;

        for (unsigned int d = 0; d < D; d++) {
            retval.components[d] = this->components[d] - rhs.components[d];
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator -=
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    template<class Tp, class Sp>
    abstract_vector_impl<T, D, S, C>&
    abstract_vector_impl<T, D, S, C>::operator -=(const C<Tp, D, Sp>& rhs) {
        THE_STACK_TRACE;

        for (unsigned int d = 0; d < D; d++) {
            this->components[d] -= rhs.components[d];
        }
       
        return *this;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator *
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    C<T, D, T[D]> abstract_vector_impl<T, D, S, C>::operator *(
            const T rhs) const {
        THE_STACK_TRACE;
        C<T, D, T[D]> retval;

        for (unsigned int d = 0; d < D; d++) {
            retval.components[d] = this->components[d] * rhs;
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator *=
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    abstract_vector_impl<T, D, S, C>&
    abstract_vector_impl<T, D, S, C>::operator *=(const T rhs) {
        THE_STACK_TRACE;

        for (unsigned int d = 0; d < D; d++) {
            this->components[d] *= rhs;
        }

        return *this;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator /
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    C<T, D, T[D]> abstract_vector_impl<T, D, S, C>::operator /(
            const T rhs) const {
        THE_STACK_TRACE;
        C<T, D, T[D]> retval;

        for (unsigned int d = 0; d < D; d++) {
            retval.components[d] = this->components[d] / rhs;
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator /=
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    abstract_vector_impl<T, D, S, C>&
    abstract_vector_impl<T, D, S, C>::operator /=(const T rhs) {
        THE_STACK_TRACE;

        for (unsigned int d = 0; d < D; d++) {
            this->components[d] /= rhs;
        }

        return *this;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator *
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    template<class Tp, class Sp>
    C<T, D, T[D]> abstract_vector_impl<T, D, S, C>::operator *(
            const C<Tp, D, Sp>& rhs) const {
        THE_STACK_TRACE;
        C<T, D, T[D]> retval;

        for (unsigned int d = 0; d < D; d++) {
            retval.components[d] = this->components[d] * rhs.components[d];
        }

        return retval;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator *=
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    template<class Tp, class Sp>
    abstract_vector_impl<T, D, S, C>&
    abstract_vector_impl<T, D, S, C>::operator *=(const C<Tp, D, Sp>& rhs) {
        THE_STACK_TRACE;

        for (unsigned int d = 0; d < D; d++) {
            this->components[d] *= rhs.components[d];
        }

        return *this;
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator []
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    T& abstract_vector_impl<T, D, S, C>::operator [](const int i) {
        THE_STACK_TRACE;
        if ((i >= 0) && (i < static_cast<int>(D))) {
            return this->components[i];
        } else {
            throw index_out_of_range_exception(i, 0, D - 1,
                __FILE__, __LINE__);
        }
    }


    /*
     * the::math::abstract_vector_impl<T, D, S, C>::operator []
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C>
    const T& abstract_vector_impl<T, D, S, C>::operator [](const int i) const {
        THE_STACK_TRACE;
        if ((i >= 0) && (i < static_cast<int>(D))) {
            return this->components[i];
        } else {
            throw index_out_of_range_exception(i, 0, D - 1,
                __FILE__, __LINE__);
        }
    }
    
} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_VECTOR_IMPL_H_INCLUDED */
