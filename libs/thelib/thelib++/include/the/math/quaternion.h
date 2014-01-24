/*
 * the/math/quaternion.h
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
 * Quaternion.h  28.09.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_QUATERNION_H_INCLUDED
#define THE_MATH_QUATERNION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/math/abstract_quaternion.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * A quaternion.
     */
    template<class T> class quaternion : public abstract_quaternion<T, T[4]> {

    public:

        /**
         * Create a new quaternion (<0, 0, 0>, 1).
         */
        inline quaternion(void) {
            THE_STACK_TRACE;
            this->components[super::idx_x] = this->components[super::idx_y] 
                = this->components[super::idx_z] = static_cast<T>(0);
            this->components[super::idx_w] = static_cast<T>(1);
        }

        /**
         * Create a new quaternion.
         *
         * @param x The new x-component.
         * @param y The new y-component.
         * @param z The new z-component.
         * @param w The new w-component.
         */
        inline quaternion(const T& x, const T& y, const T& z, const T& w) 
                : super(x, y, z, w) {
            THE_STACK_TRACE;
        }

        /**
         * Create a new quaternion using the specified components. 'components'
         * must be an array of four consecutive elements, ordered x, y, z, and 
         * w. The caller remains owner of the memory, the object creates a
         * deep copy.
         *
         * @param components The components of the quaternion.
         */
        inline explicit quaternion(const T *components) {
            THE_STACK_TRACE;
            ::memcpy(this->components, components, 4 * sizeof(T));
        }

        /**
         * Construct a quaternion from an angle and a rotation axis.
         *
         * @param angle The rotation angle in radians.
         * @param axis  The rotation axis.
         */
        template<class Tp, class Sp>
        inline quaternion(const T& angle, const abstract_vector<Tp, 3, Sp>& axis) {
            THE_STACK_TRACE;
            this->set(angle, axis);
        }

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be copied.
         */
        inline quaternion(const quaternion& rhs) {
            THE_STACK_TRACE;
            ::memcpy(this->components, rhs.components, 4 * sizeof(T));
        }

        /**
         * Create a copy of 'rhs'. This ctor allows for arbitrary quaternion to
         * quaternion conversions.
         *
         * @param rhs The quaternion to be cloned.
         */
        template<class Tp, class Sp>
        explicit quaternion(const abstract_quaternion<Tp, Sp>& rhs);

        /** Dtor. */
        ~quaternion(void);

        /**
         * Assignment.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline quaternion& operator =(const quaternion& rhs) {
            THE_STACK_TRACE;
            abstract_quaternion<T, T[4]>::operator =(rhs);
            return *this;
        }

        /**
         * Assignment. This operator allows arbitrary quaternion to
         * quaternion conversions.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Tp, class Sp>
        inline quaternion& operator =(const abstract_quaternion<Tp, Sp>& rhs) {
            THE_STACK_TRACE;
            abstract_quaternion<T, T[4]>::operator =(rhs);
            return *this;
        }

    private:

        /** super class typedef. */
        typedef abstract_quaternion<T, T[4]> super;
    };


    /*
     * the::math::quaternion<T>::quaternion
     */
    template<class T>
    template<class Tp, class Sp>
    quaternion<T>::quaternion(const abstract_quaternion<Tp, Sp>& rhs) {
        THE_STACK_TRACE;
        this->components[super::idx_x] = static_cast<T>(rhs.x());
        this->components[super::idx_y] = static_cast<T>(rhs.y());
        this->components[super::idx_z] = static_cast<T>(rhs.z());
        this->components[super::idx_w] = static_cast<T>(rhs.w());
    }


    /*
     * the::math::quaternion<T>::~quaternion
     */
    template<class T>
    quaternion<T>::~quaternion(void) {
        THE_STACK_TRACE;
    }


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_QUATERNION_H_INCLUDED */
