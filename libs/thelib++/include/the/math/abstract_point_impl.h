/*
 * the/math/abstract_point_impl.h
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
 * AbstractPointImpl.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_POINT_IMPL_H_INCLUDED
#define THE_MATH_ABSTRACT_POINT_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/config.h"
#include "the/assert.h"
//#include "the/mathfunctions.h"
#include "the/index_out_of_range_exception.h"
#include "the/math/abstract_vector.h"
#include "the/math/types.h"
#include "the/math/vector.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * TODO: comment class
     */
    template<class T, unsigned int D, class S, 
            template<class T, unsigned int D, class S> class C> 
            class abstract_point_impl {

    public:

        /** Dtor. */
        ~abstract_point_impl(void);

        /**
         * Answer the distance from this point to 'toPoint'.
         *
         * @param toPoint The point to calculate the distance to.
         *
         * @return The distance between the two points.
         */
        template<class Tp, class Sp>
        T distance(const C<Tp, D, Sp>& toPoint) const;

        /**
         * Answer in which halfspace 'point' lies in respect to the plane
         * defined by 'this' point and the 'normal' vector.
         *
         * @param normal The normal vector defining the plane.
         * @param point The point to be tested.
         *
         * @return The halfspace the point lies in.
         */
        template<class Tpp, class Spp, class Tpv, class Spv>
        half_space half_space(const abstract_vector<Tpv, D, Spv>& normal,
            const C<Tpp, D, Spp>& point) const;

        /**
         * interpolates a position between 'this' and 'rhs' based on the value
         * of 't'.
         *
         * @param rhs The right hand side operand.
         * @param t   The interpolation value. (Should be [0, 1])
         *
         * @return The interpolated position.
         */
        template<class Tp, class Sp, class Tp2>
        C<T, D, T[D]> interpolate(const C<Tp, D, Sp>& rhs, Tp2 t) const;

        /**
         * Answer whether the point is the coordinate system origin (0, ..., 0).
         *
         * @return true, if the point is the origin, false otherwise.
         */
        bool is_origin(void) const;

        /**
         * Directly access the internal pointer holding the coordinates.
         * The object remains owner of the memory returned.
         *
         * @return The coordinates in an array.
         */
        inline T *peek_coordinates(void) {
            THE_STACK_TRACE;
            return this->coordinates;
        }

        /**
         * Directly access the internal pointer holding the coordinates.
         * The object remains owner of the memory returned.
         *
         * @return The coordinates in an array.
         */
        inline const T *peek_coordinates(void) const {
            THE_STACK_TRACE;
            return this->coordinates;
        }

        /**
         * Answer the square of the distance from this point to 'toPoint'.
         *
         * @param toPoint The point to calculate the distance to.
         *
         * @return The distance between the two points.
         */
        template<class Tp, class Sp>
        T square_distance(const C<Tp, D, Sp>& toPoint) const;

        /**
         * Assignment operator.
         *
         * This operation does <b>not</b> create aliases.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        abstract_point_impl<T, D, S, C>& operator =(const C<T, D, S>& rhs);

        /**
         * Assigment for arbitrary points. A valid static_cast between T and Tp
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
         * Subclasses must ensure that sufficient memory for the 'coordinates'
         * member has been allocated before calling this operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        template<class Tp, unsigned int Dp, class Sp>
        abstract_point_impl<T, D, S, C>& operator =(const C<Tp, Dp, Sp>& rhs);

        /**
         * Test for equality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are equal, false otherwise.
         */
        bool operator ==(const C<T, D, S>& rhs) const;

        /**
         * Test for equality of arbitrary points. This operation uses the
         * IsEqual function of the left hand side operand. Note that points 
         * with different dimensions are never equal.
         *
         * @param rhs The right hand side operand.
         *
         * @param true, if 'rhs' and this point are equal, false otherwise.
         */
        template<class Tp, unsigned int Dp, class Sp>
        bool operator ==(const C<Tp, Dp, Sp>& rhs) const;

        /**
         * Test for inequality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are not equal, false otherwise.
         */
        inline bool operator !=(const C<T, D, S>& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Test for inequality of arbitrary points. See operator == for further
         * details.
         *
         * @param rhs The right hand side operand.
         *
         * @param true, if 'rhs' and this vector are not equal, false otherwise.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline bool operator !=(const C<Tp, Dp, Sp>& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Move the point along the vector 'rhs'.
         *
         * @param rhs The direction vector to move the point to.
         *
         * @return A copy of this point moved by rhs.
         */
        template<class Tp, class Sp>
        C<T, D, T[D]> operator +(const abstract_vector<Tp, D, Sp>& rhs) const;

        /**
         * Move the point along the vector 'rhs'.
         *
         * @param rhs The direction vector to move the point to.
         *
         * @return *this.
         */
        template<class Tp, class Sp>
        abstract_point_impl<T, D, S, C>& operator +=(
            const abstract_vector<Tp, D, Sp>& rhs);

        /**
         * Move the point along the negative vector 'rhs'.
         *
         * @param rhs The direction vector to move the point against.
         *
         * @return A copy of this point moved by rhs.
         */
        template<class Tp, class Sp>
        C<T, D, T[D]> operator -(const abstract_vector<Tp, D, Sp>& rhs) const;

        /**
         * Move the point along the negative vector 'rhs'.
         *
         * @param rhs The direction vector to move the point against.
         *
         * @return *this.
         */
        template<class Tp, class Sp>
        abstract_point_impl<T, D, S, C>& operator -=(
            const abstract_vector<Tp, D, Sp>& rhs);

        /**
         * Subtract 'rhs' from this point.
         *
         * @param rhs The right hand side operand.
         *
         * @return A vector pointing from this point to 'rhs'.
         */
        template<class Tp, class Sp>
        vector<T, D> operator -(const C<Tp, D, Sp>& rhs) const;

        /**
         * Directly access the 'i'th coordinate of the point.
         *
         * @param i The index of the coordinate within [0, D[.
         *
         * @return A reference to the x-coordinate for 0, 
         *         the y-coordinate for 1, etc.
         *
         * @throws out_of_range_exception, if 'i' is not within [0, D[.
         */
        T& operator [](const int i);

        /**
         * Answer the coordinates of the point.
         *
         * @param i The index of the coordinate within [0, D[.
         *
         * @return The x-coordinate for 0, the y-coordinate for 1, etc.
         *
         * @throws out_of_range_exception, if 'i' is not within [0, D[.
         */
        T operator [](const int i) const;

        /**
         * Cast to vector. The resulting vector is the position vector of
         * this point.
         *
         * @return The position vector of the point.
         */
        inline operator vector<T, D>(void) const {
            THE_STACK_TRACE;
            return vector<T, D>(this->coordinates);
        }

    protected:

        /**
         * Disallow instances of this class. This ctor does nothing!
         */
        inline abstract_point_impl(void) {
            THE_STACK_TRACE;
        };

        /** 
         * The coordinates of the point. This can be a T * pointer or a T[D]
         * static array.
         */
        S coordinates;
    };


    /*
     * the:math::abstract_point_impl<T, D, S>::~abstract_point_impl
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    abstract_point_impl<T, D, S, C>::~abstract_point_impl(void) {
        THE_STACK_TRACE;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::distance
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, class Sp>
    T abstract_point_impl<T, D, S, C>::distance(
            const C<Tp, D, Sp>& toPoint) const {
        THE_STACK_TRACE;
        return static_cast<T>(::sqrt(static_cast<double>(
            this->square_distance(toPoint))));
    }


    /*
     * the:math::abstract_point_impl<T, D, S, C>::halfspace
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tpp, class Spp, class Tpv, class Spv>
    half_space abstract_point_impl<T, D, S, C>::half_space(
            const abstract_vector<Tpv, D, Spv>& normal,
            const C<Tpp, D, Spp>& point) const {
        THE_STACK_TRACE;
        using the::math::half_space; // because gcc 4.6 was not able to find the type by itself.

        T val = static_cast<T>(0);
        for (unsigned int d = 0; d < D; d++) {
            val += static_cast<T>(normal[d]) * (static_cast<T>(point[d])
                - this->coordinates[d]);
        }

        if (is_equal(val, static_cast<T>(0))) {
            return half_space::in_plane;

        } else if (val > static_cast<T>(0)) {
            return half_space::positive;//HALFSPACE_NEGATIVE;

        } else if (val < static_cast<T>(0)) {
            return half_space::negative;//HALFSPACE_POSITIVE;
    
        } else {
            assert(false);      // Should never happen.
            return half_space::in_plane;
        }        
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::interpolate
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, class Sp, class Tp2>
    C<T, D, T[D]> abstract_point_impl<T, D, S, C>::interpolate(
            const C<Tp, D, Sp>& rhs, Tp2 t) const {
        THE_STACK_TRACE;
        C<T, D, T[D]> retval;
        Tp2 at = static_cast<Tp2>(1) - t;

        for (unsigned int d = 0; d < D; d++) {
            retval.coordinates[d] = static_cast<Tp>(
                static_cast<Tp2>(this->coordinates[d]) * at
                + static_cast<Tp2>(rhs[d]) * t);
        }

        return retval;
    }


    /*
     * the:math::abstract_point_impl<T, D, S, C>::is_origin
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    bool abstract_point_impl<T, D, S, C>::is_origin(void) const {
        THE_STACK_TRACE;
        for (unsigned int i = 0; i < D; i++) {
            if (!is_equal<T>(this->coordinates[i], 0)) {
                return false;
            }
        }

        return true;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::square_distance
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, class Sp>
    T abstract_point_impl<T, D, S, C>::square_distance(
            const C<Tp, D, Sp>& toPoint) const {
        THE_STACK_TRACE;
        T retval = static_cast<T>(0);

        for (unsigned int i = 0; i < D; i++) {
            retval += static_cast<T>(sqr(toPoint.coordinates[i] - this->coordinates[i]));
        }

        return retval;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator =
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    abstract_point_impl<T, D, S, C>& abstract_point_impl<T, D, S, C>::operator =(
            const C<T, D, S>& rhs) {
        THE_STACK_TRACE;
        if (this != &rhs) {
            ::memcpy(this->coordinates, rhs.coordinates, D * sizeof(T));
        }

        return *this;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator =
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, unsigned int Dp, class Sp>
    abstract_point_impl<T, D, S, C>& abstract_point_impl<T, D, S, C>::operator =(
           const C<Tp, Dp, Sp>& rhs) {
        THE_STACK_TRACE;

        if (static_cast<void *>(this) != static_cast<const void *>(&rhs)) {
            for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
                this->coordinates[d] = static_cast<T>(rhs[d]);
            }
            for (unsigned int d = Dp; d < D; d++) {
                this->coordinates[d] = static_cast<T>(0);
            }            
        }

        return *this;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator ==
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    bool abstract_point_impl<T, D, S, C>::operator ==(
            const C<T, D, S>& rhs) const {
        THE_STACK_TRACE;

        for (unsigned int d = 0; d < D; d++) {
            if (!is_equal<T>(this->coordinates[d], rhs.coordinates[d])) {
                return false;
            }
        }

        return true;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator ==
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, unsigned int Dp, class Sp>
    bool abstract_point_impl<T, D, S, C>::operator ==(
            const C<Tp, Dp, Sp>& rhs) const {
        THE_STACK_TRACE;

        if (D != Dp) {
            return false;
        }

        for (unsigned int d = 0; d < D; d++) {
            if (!is_equal<T>(this->coordinates[d], rhs.coordinates[d])) {
                return false;
            }
        }

        return true;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator +
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, class Sp>
    C<T, D, T[D]> abstract_point_impl<T, D, S, C>::operator +(
            const abstract_vector<Tp, D, Sp>& rhs) const {
        THE_STACK_TRACE;
        C<T, D, T[D]> retval;

        for (unsigned int d = 0; d < D; d++) {
            retval.coordinates[d] = this->coordinates[d] 
                + static_cast<T>(rhs[d]);
        }

        return retval;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator +=
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, class Sp>
    abstract_point_impl<T, D, S, C>& abstract_point_impl<T, D, S, C>::operator +=(
           const abstract_vector<Tp, D, Sp>& rhs) {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->coordinates[d] += static_cast<T>(rhs[d]);
        }

        return *this;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator -
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, class Sp>
    C<T, D, T[D]> abstract_point_impl<T, D, S, C>::operator -(
           const abstract_vector<Tp, D, Sp>& rhs) const {
        THE_STACK_TRACE;
        C<T, D, T[D]> retval;

        for (unsigned int d = 0; d < D; d++) {
            retval.coordinates[d] = this->coordinates[d] 
                - static_cast<T>(rhs[d]);
        }

        return retval;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator -=
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, class Sp>
    abstract_point_impl<T, D, S, C>& abstract_point_impl<T, D, S, C>::operator -=(
           const abstract_vector<Tp, D, Sp>& rhs) {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->coordinates[d] -= static_cast<T>(rhs[d]);
        }

        return *this;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator -
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    template<class Tp, class Sp>
    vector<T, D> abstract_point_impl<T, D, S, C>::operator -(
           const C<Tp, D, Sp>& rhs) const {
        THE_STACK_TRACE;
        vector<T, D> retval;

        for (unsigned int d = 0; d < D; d++) {
            retval[d] = this->coordinates[d] 
                - static_cast<T>(rhs.coordinates[d]);
        }

        return retval;
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator []
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    T& abstract_point_impl<T, D, S, C>::operator [](const int i) {
        THE_STACK_TRACE;
        if ((i >= 0) && (i < static_cast<int>(D))) {
            return this->coordinates[i];
        } else {
            throw index_out_of_range_exception(i, 0, D - 1, __FILE__, __LINE__);
        }
    }


    /*
     * the:math::abstract_point_impl<T, D, S>::operator []
     */
    template<class T, unsigned int D, class S, 
        template<class T, unsigned int D, class S> class C> 
    T abstract_point_impl<T, D, S, C>::operator [](const int i) const {
        THE_STACK_TRACE;
        if ((i >= 0) && (i < static_cast<int>(D))) {
            return this->coordinates[i];
        } else {
            throw index_out_of_range_exception(i, 0, D - 1, __FILE__, __LINE__);
        }
    }
    
} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_POINT_IMPL_H_INCLUDED */
