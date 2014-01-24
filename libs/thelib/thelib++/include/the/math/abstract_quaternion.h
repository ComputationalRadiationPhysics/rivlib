/*
 * the/math/abstract_quaternion.h
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
 * abstract_quaternion.h  28.09.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_QUATERNION_H_INCLUDED
#define THE_MATH_ABSTRACT_QUATERNION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include <cmath>

#include "the/assert.h"
#include "the/argument_exception.h"
#include "the/math/functions.h"
#include "the/math/vector.h"
#include "the/stack_trace.h"
#include "the/types.h"


namespace the {
namespace math {

    /**
     * Objects of this class represent a quaternion. The quaterion is internally
     * stored in order x, y, z, w, which must be kept in mind when passing 
     * values in array form.
     *
     * This class implements the functionality of the quaterion. Acutally used
     * can only be the derived classes Quaterion and ShallowQuaterion, which use
     * different means for storing their data.
     *
     * The quaterion is a template, which allows instantiations for different
     * scalar types, but only floating point instantiations (float or double) 
     * make any sense. you should not instantiate a quaterion for other types.
     */
    template<class T, class S = T[4]> class abstract_quaternion {

    public:

        /** Dtor. */
        ~abstract_quaternion(void);

        /**
         * Answer the angle and the axis of the rotation that is 
         * represented by this quaternion. 
         *
         * @param outAngle receives the angle in radians. 
         * @param outAxis  receives the vector representing the rotation
         *                 axis. The vector is guaranteed to be normalised.
         */
        void angle_and_axis(T& outAngle, vector<T, 3>& outAxis) const;

        /**
         * conjugate the quaternion.
         */
        void conjugate(void);

        /**
         * Answer the i-component (= x-component) of the quaternion.
         *
         * @return The i-component.
         */
        inline const T& get_i(void) const {
            THE_STACK_TRACE;
            return this->components[idx_x];
        }

        /**
         * Answer the j-component (= y-component) of the quaternion.
         *
         * @return The j-component.
         */
        inline const T& get_j(void) const {
            THE_STACK_TRACE;
            return this->components[idx_y];
        }

        /**
         * Answer the k-component (= z-component) of the quaternion.
         *
         * @return The k-component.
         */
        inline const T& get_k(void) const {
            THE_STACK_TRACE;
            return this->components[idx_z];
        }

        /**
         * Answer the r-component (= w-component) of the quaternion.
         *
         * @return The r-component.
         */
        inline const T& get_r(void) const {
            THE_STACK_TRACE;
            return this->components[idx_w];
        }

        /**
         * Answer the w-component of the quaternion.
         *
         * @return The w-component.
         */
        inline const T& get_w(void) const {
            THE_STACK_TRACE;
            return this->components[idx_w];
        }

        /**
         * Answer the x-component of the quaternion.
         *
         * @return The x-component.
         */
        inline const T& get_x(void) const {
            THE_STACK_TRACE;
            return this->components[idx_x];
        }

        /**
         * Answer the y-component of the quaternion.
         *
         * @return The y-component.
         */
        inline const T& get_y(void) const {
            THE_STACK_TRACE;
            return this->components[idx_y];
        }

        /**
         * Answer the z-component of the quaternion.
         *
         * @return The z-component.
         */
        inline const T& get_z(void) const {
            THE_STACK_TRACE;
            return this->components[idx_z];
        }

        /**
         * Answer the i-component (= x-component) of the quaternion.
         *
         * @return The i-component.
         */
        inline const T& i(void) const {
            THE_STACK_TRACE;
            return this->components[idx_x];
        }

        /**
         * Answer the j-component (= y-component) of the quaternion.
         *
         * @return The j-component.
         */
        inline const T& j(void) const {
            THE_STACK_TRACE;
            return this->components[idx_y];
        }

        /**
         * Answer the k-component (= z-component) of the quaternion.
         *
         * @return The k-component.
         */
        inline const T& k(void) const {
            THE_STACK_TRACE;
            return this->components[idx_z];
        }

        //Quaternion inverse(void) const;

        /**
         * interpolates between 'this' and 'rhs' linearly based on
         * '0 <= t <= 1'.
         *
         * @param rhs The second point to interpolate to (t=1)
         * @param t The interpolation value (0..1)
         *
         * @return The interpolation result
         */
        template<class Sp, class Tp>
        inline abstract_quaternion<T, T[4]>
        interpolate(const abstract_quaternion<T, Sp>& rhs, Tp t) const {
            THE_STACK_TRACE;
            abstract_quaternion<T, T[4]> rv;
            rv.slerp(t, *this, rhs);
            return rv;
        }

        /** 
         * invert the quaternion.
         */
        void invert(void);

        /**
         * Answer whether the quaternion is pure imaginary.
         *
         * This operation uses an epsilon compare for T.
         *
         * @return true if the real part is zero, false otherwise.
         */
        inline bool is_pure(void) const {
            THE_STACK_TRACE;
            return is_equal<T>(this->components[idx_w], static_cast<T>(0));
        }

        /**
         * Answer the norm of the quaternion.
         *
         * @return The norm of the quaternion.
         */
        inline T norm(void) const {
            THE_STACK_TRACE;
            return sqrt(sqr(this->components[idx_x])
                + sqr(this->components[idx_y])
                + sqr(this->components[idx_z]) 
                + sqr(this->components[idx_w]));
        }

        /**
         * normalise the quaternion.
         *
         * @return The norm BEFOrE the normalisation.
         */
        T normalise(void);

        /**
         * Provide direct access to the components of the quaternion.
         *
         * @return A pointer to the actual components.
         */
        inline const T *peek_components(void) const {
            THE_STACK_TRACE;
            return this->components;
        }

        /**
         * Provide direct access to the components of the quaternion.
         *
         * @return A pointer to the actual components.
         */
        inline T *peek_components(void) {
            THE_STACK_TRACE;
            return this->components;
        }

        /**
         * Answer the r-component (= w-component) of the quaternion.
         *
         * @return The r-component.
         */
        inline const T& r(void) const {
            THE_STACK_TRACE;
            return this->components[idx_w];
        }

        /**
         * set the components of the quaternion.
         *
         * @param x The new x-component.
         * @param y The new y-component.
         * @param z The new z-component.
         * @param w The new w-component.
         */
        inline void set(const T& x, const T& y, const T& z, const T& w) {
            THE_STACK_TRACE;
            this->components[idx_x] = x;
            this->components[idx_y] = y;
            this->components[idx_z] = z;
            this->components[idx_w] = w;
        }

        /**
         * set the components of the quaternion in such a way that is represents
         * the specified rotation.
         *
         * @param angle The rotation angle in radians.
         * @param axis  The vector specifying the rotation axis.
         */
        template<class Tp, class Sp>
        void set(const T& angle, const abstract_vector<Tp, 3, Sp>& axis);

        /**
         * Tries to set the quarternion from rotation matrix components.
         *
         * @param m11 Matrix component row 1, column 1.
         * @param m12 Matrix component row 1, column 2.
         * @param m13 Matrix component row 1, column 3.
         * @param m21 Matrix component row 2, column 1.
         * @param m22 Matrix component row 2, column 2.
         * @param m23 Matrix component row 2, column 3.
         * @param m31 Matrix component row 3, column 1.
         * @param m32 Matrix component row 3, column 2.
         * @param m33 Matrix component row 3, column 3.
         *
         * @throw illegal_param_exception if the matrix components do not seem
         *                              to form a rotation-only matrix.
         */
        void set_from_rotation_matrix(const T& m11, const T& m12, const T& m13,
                const T& m21, const T& m22, const T& m23,
                const T& m31, const T& m32, const T& m33);

        /**
         * set the i-component (= x-component) of the quaternion.
         *
         * @param i The new value for the component.
         */
        inline void set_i(const T& i) {
            THE_STACK_TRACE;
            this->components[idx_x] = i;
        }

        /**
         * set the j-component (= y-component) of the quaternion.
         *
         * @param j The new value for the component.
         */
        inline void set_j(const T& j) {
            THE_STACK_TRACE;
            this->components[idx_y] = j;
        }

        /**
         * set the k-component (= z-component) of the quaternion.
         *
         * @param k The new value for the component.
         */
        inline void set_k(const T& k) {
            THE_STACK_TRACE;
            this->components[idx_z] = k;
        }

        /**
         * set the r-component (= w-component) of the quaternion.
         *
         * @param r The new value for the component.
         */
        inline void set_r(const T& r) {
            THE_STACK_TRACE;
            this->components[idx_w] = r;
        }

        /**
         * set the w-component of the quaternion.
         *
         * @param w The new value for the component.
         */
        inline void set_w(const T& w) {
            THE_STACK_TRACE;
            this->components[idx_w] = w;
        }

        /**
         * set the x-component of the quaternion.
         *
         * @param x The new value for the component.
         */
        inline void set_x(const T& x) {
            THE_STACK_TRACE;
            this->components[idx_x] = x;
        }

        /**
         * set the y-component of the quaternion.
         *
         * @param y The new value for the component.
         */
        inline void set_y(const T& y) {
            THE_STACK_TRACE;
            this->components[idx_y] = y;
        }

        /**
         * set the z-component of the quaternion.
         *
         * @param z The new value for the component.
         */
        inline void set_z(const T& z) {
            THE_STACK_TRACE;
            this->components[idx_z] = z;
        }

        /**
         * sets the components of the quaternion by performing a slerp 
         * interpolation between 'a' and 'b' using 'alpha' as interpolation
         * parameter [0, 1]. if 'alpha' is zero 'a' is used; if 'alpha is one
         * 'b' is used.
         *
         * @param alpha The interpolation parameter [0, 1]
         * @param a The first interpolation value, used if 'alpha' is zero.
         * @param b The second interpolation value, used if 'alpha' is one.
         */
        template<class Sp1, class Sp2>
        void slerp(float alpha, const abstract_quaternion<T, Sp1>& a, 
            const abstract_quaternion<T, Sp2>& b);

        /**
         * Make this quaternion the square of itself.
         */
        void square(void);

        /**
         * Answer the w-component of the quaternion.
         *
         * @return The w-component.
         */
        inline const T& w(void) const {
            THE_STACK_TRACE;
            return this->components[idx_w];
        }

        /**
         * Answer the x-component of the quaternion.
         *
         * @return The x-component.
         */
        inline const T& x(void) const {
            THE_STACK_TRACE;
            return components[idx_x];
        }

        /**
         * Answer the y-component of the quaternion.
         *
         * @return The y-component.
         */
        inline const T& y(void) const {
            THE_STACK_TRACE;
            return this->components[idx_y];
        }

        /**
         * Answer the z-component of the quaternion.
         *
         * @return The z-component.
         */
        inline const T& z(void) const {
            THE_STACK_TRACE;
            return this->components[idx_z];
        }

        /**
         * Assignment.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        abstract_quaternion& operator =(const abstract_quaternion& rhs);

        /**
         * Assignment. This operator allows arbitrary quaternion to
         * quaternion conversions.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Tp, class Sp>
        abstract_quaternion& operator =(const abstract_quaternion<Tp, Sp>& rhs);

        /**
         * Test for equality. The is_equal function is used for this.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are equal, false otherwise.
         */
        bool operator ==(const abstract_quaternion& rhs) const;

        /**
         * Test for equality. This operator allows comparing quaternions that
         * have been instantiated for different scalar types. The is_equal<T>
         * function for the scalar type of the left hand side operand is used
         * as comparison operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are equal, false otherwise.
         */
        template<class Tp, class Sp>
        bool operator ==(const abstract_quaternion<Tp, Sp>& rhs) const;

        /**
         * Test for inequality. The is_equal function is used for this.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are not equal, false otherwise.
         */
        inline bool operator !=(const abstract_quaternion& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Test for inequality. This operator allows comparing quaternions that
         * have been instantiated for different scalar types. The is_equal<T>
         * function for the scalar type of the left hand side operand is used
         * as comparison operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are not equal, false otherwise.
         */
        template<class Tp, class Sp>
        inline bool operator !=(const abstract_quaternion<Tp, Sp>& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Multiplication of two quaternions.
         *
         * @param rhs The right hand side operand.
         *
         * @return The result of *this * rhs.
         */
        template<class Sp>
        abstract_quaternion<T, T[4]> operator *(
            const abstract_quaternion<T, Sp>& rhs) const;

        /**
         * Multiplies a quaternion and a vector. The result is the vector 'rhs'
         * rotated by the rotation that is defined through this quaternion.
         *
         * Note: This multiplication does not compute the quaternion 
         * multiplication *this * (rhs, 0), but the actual vector transformation
         * that is equivalent to *this * (rhs, 0) * *this->conjugate().
         *
         * @param rhs The right hand side operand.
         *
         * @return The resulting vector.
         */
        template<class Sp>
        vector<T, 3> operator *(const abstract_vector<T, 3, Sp>& rhs) const;

        //operator matrix4x4(void) const;

    protected:

        /** The index of the w component. */
        static const uintptr_t idx_w;

        /** The index of the x component. */
        static const uintptr_t idx_x;

        /** The index of the y component. */
        static const uintptr_t idx_y;

        /** The index of the z component. */
        static const uintptr_t idx_z;

        /**
         * Disallow instances of this class.
         */
        inline abstract_quaternion(void) {
            THE_STACK_TRACE;
        }

        /**
         * Create a new quaternion.
         *
         * wArNiNG: Do not call this ctor but on deep storage instantiations!
         *
         * @param x The new x-component.
         * @param y The new y-component.
         * @param z The new z-component.
         * @param w The new w-component.
         */
        inline abstract_quaternion(const T& x, const T& y, const T& z, 
                const T& w) {
            THE_STACK_TRACE;
            THE_ASSERT(sizeof(this->components) == 4 * sizeof(T));
            this->components[idx_x] = x;
            this->components[idx_y] = y;
            this->components[idx_z] = z;
            this->components[idx_w] = w;
        }

        /** 
         * The components of the quaterion. These are stored in the following
         * order: x, y, z (the vector), w.
         */
        S components;
    };


    /*
     * the::math::abstract_quaternion<T, S>::~abstract_quaternion
     */
    template<class T, class S>
    abstract_quaternion<T, S>::~abstract_quaternion(void) {
        THE_STACK_TRACE;
    }


    /*
     * the::math::abstract_quaternion<T, S>::angle_and_axis
     */
    template<class T, class S>
    void abstract_quaternion<T, S>::angle_and_axis(T& outAngle, 
            vector<T, 3>& outAxis) const {
        THE_STACK_TRACE;
        T d = sqrt(sqr(this->components[idx_x]) + sqr(this->components[idx_y])
            + sqr(this->components[idx_z]));

        if (!is_equal<T>(d, static_cast<T>(0))) {
            outAxis.set_x(this->components[idx_x] / d);
            outAxis.set_y(this->components[idx_y] / d);
            outAxis.set_z(this->components[idx_z] / d);

            // TODO: Not nice.
            outAngle = static_cast<T>(2.0 
                * ::acos(static_cast<double>(this->components[idx_w])));

        } else {
            outAxis.set_x(static_cast<T>(0));
            outAxis.set_y(static_cast<T>(0));
            outAxis.set_z(static_cast<T>(1));
            outAngle = 0.0f;
        } 

        ASSERT(outAxis.is_normalised());
    }


    /*
     * the::math::abstract_quaternion<T, S>::conjugate
     */
    template<class T, class S> void abstract_quaternion<T, S>::conjugate(void) {
        THE_STACK_TRACE;
        this->components[idx_x] *= static_cast<T>(-1);
        this->components[idx_y] *= static_cast<T>(-1);
        this->components[idx_z] *= static_cast<T>(-1);
    }


    /*
     * the::math::abstract_quaternion<T, S>::invert
     */
    template<class T, class S>
    void abstract_quaternion<T, S>::invert(void) {
        THE_STACK_TRACE;
        T norm = this->norm();

        if (!is_equal<T>(norm, static_cast<T>(0))) {
            this->components[idx_x] /= -norm;
            this->components[idx_y] /= -norm;
            this->components[idx_z] /= -norm;
            this->components[idx_w] /= norm;

        } else {
            this->components[idx_x] = this->components[idx_y] 
                = this->components[idx_z] = static_cast<T>(0);
            this->components[idx_w] = static_cast<T>(1);
        }
    }


    /*
     * the::math::abstract_quaternion<T, S>::normalise
     */
    template<class T, class S>
    T abstract_quaternion<T, S>::normalise(void) {
        THE_STACK_TRACE;
        T norm = this->norm();

        if (!is_equal<T>(norm, static_cast<T>(0))) {
            this->components[idx_x] /= norm;
            this->components[idx_y] /= norm;
            this->components[idx_z] /= norm;
            this->components[idx_w] /= norm;

        } else {
            this->components[idx_x] = this->components[idx_y] 
                = this->components[idx_z] = static_cast<T>(0);
            this->components[idx_w] = static_cast<T>(1);
        }

        return norm;
    }

    /*
     * the::math::abstract_quaternion<T, S>::set
     */
    template<class T, class S>
    template<class Tp, class Sp>
    void abstract_quaternion<T, S>::set(const T& angle, 
            const abstract_vector<Tp, 3, Sp>& axis) {
        THE_STACK_TRACE;
        vector<Tp, 3> ax(axis);
        T len = ax.normalise();
        double halfAngle = 0.5 * static_cast<double>(angle);

        if (!is_equal(len, static_cast<T>(0))){
            len = static_cast<T>(::sin(halfAngle) / len);
            this->components[idx_x] = ax.x() * len;
            this->components[idx_y] = ax.y() * len;
            this->components[idx_z] = ax.z() * len;
            this->components[idx_w] = static_cast<T>(::cos(halfAngle));

        } else {
            this->components[idx_x] = this->components[idx_y] 
                = this->components[idx_z] = static_cast<T>(0);
            this->components[idx_w] = static_cast<T>(1);
        }
    }


    /*
     * abstract_quaternion<T, S>::set_from_rotation_matrix
     */
    template<class T, class S>
    void abstract_quaternion<T, S>::set_from_rotation_matrix(const T& m11,
            const T& m12, const T& m13, const T& m21, const T& m22,
            const T& m23, const T& m31, const T& m32, const T& m33) {
        THE_STACK_TRACE;
        vector<T, 3> xi(m11, m21, m31);
        vector<T, 3> yi(m12, m22, m32);
        vector<T, 3> zi(m13, m23, m33);

        xi.normalise(); // we could throw here, but let's be nice.
        yi.normalise();
        zi.normalise();

        vector<T, 3> xo(static_cast<T>(1), static_cast<T>(0),
            static_cast<T>(0));
        vector<T, 3> yo(static_cast<T>(0), static_cast<T>(1),
            static_cast<T>(0));
        vector<T, 3> zo(static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(1));

        abstract_quaternion<T, T[4]> q1;
        if (xi == xo) {
            // rot 0�
            q1.set(static_cast<T>(0), static_cast<T>(0),
                static_cast<T>(0), static_cast<T>(1));

        } else if (xi == -xo) {
            // rot 180�
            q1.set(static_cast<T>(pi_double), yo);

        } else {
            // rot something
            T angle = ::acos(xo.dot(xi));
            vector<T, 3> axis = xo.cross(xi);
            axis.normalise();
            q1.set(angle, axis);

            vector<T, 3> xb = q1 * xo;
            if (xb != xi) {
                q1.set(-angle, axis);
                xb = q1 * xo;
            }
            if (xb != xi) {
                throw argument_exception("Matrix is not rotation-only",
                    __FILE__, __LINE__);
            }

        }

        vector<T, 3> yb = q1 * yo;
        if (yi == yb) {
            // rot 0�
            if ((q1 * zo) != zi) {
                throw argument_exception("Matrix is not rotation-only",
                    __FILE__, __LINE__);
            }

        } else if (yi == -yb) {
            // rot 180�
            abstract_quaternion<T, T[4]> q2;
            q2.set(static_cast<T>(pi_double), xi);
            q1 = q2 * q1;

        } else {
            // rot something
            abstract_quaternion<T, T[4]> q2;
            T angle = ::acos(yb.dot(yi));
            vector<T, 3> axis = yb.cross(yi);
            axis.normalise();
            q2.set(angle, axis);

            vector<T, 3> yc = q2 * yb;
            if (yc != yi) {
                q2.set(-angle, axis);
                yc = q2 * yb;
            }
            if (yc != yi) {
                throw argument_exception("Matrix is not rotation-only",
                    __FILE__, __LINE__);
            }
            q1 = q2 * q1;

        }

        vector<T, 3> xb = q1 * xo;
        yb = q1 * yo;
        vector<T, 3> zb = q1 * zo;
        if ((xb != xi) || (yb != yi) || (zb!= zi)) {
            throw argument_exception("Matrix is not rotation-only",
                __FILE__, __LINE__);
        }

        *this = q1;
    }


    /*
     * the::math::abstract_quaternion<T, S>::slerp
     */
    template<class T, class S>
    template<class Sp1, class Sp2>
    void abstract_quaternion<T, S>::slerp(float alpha, 
            const abstract_quaternion<T, Sp1>& a, 
            const abstract_quaternion<T, Sp2>& b) {
        THE_STACK_TRACE;

        if (alpha < float_epsilon) {
            *this = a;
        } else if (alpha > (1.0f - float_epsilon)) {
            *this = b;
        } else {
            bool flipT;
            float slerpFrom, slerpTo, omega, sinOmega;
            float cosOmega = a.x() * b.x() + a.y() * b.y() + a.z() * b.z() 
                + a.w() * b.w();

            if ((flipT = (cosOmega < 0.0f))) {
                cosOmega = -cosOmega;
            }

            if ((1.0f - cosOmega) > 0.0001f) {
                omega = acosf(cosOmega);
                sinOmega = sinf(omega);
                slerpFrom = sinf((1.0f - alpha) * omega) / sinOmega;
                slerpTo = sinf(alpha * omega) / sinOmega;
                if (flipT) {
                    slerpTo = -slerpTo;
                }
            } else {
                slerpFrom = 1.0f - alpha;
                slerpTo = alpha;
            }

            for (unsigned int i = 0; i < 4; i++) {
                this->components[i] = slerpFrom * a.components[i]
                    + slerpTo * b.components[i];
            }
        }
    }


    /*
     * the::math::abstract_quaternion<T, S>::square
     */
    template<class T, class S> void abstract_quaternion<T, S>::square(void) {
        THE_STACK_TRACE;
        T tmp = 2 * this->components[idx_w];
        this->components[idx_w] = sqr(this->components[idx_w])
            -(sqr(this->components[idx_x] + sqr(this->components[idx_y]) 
            + sqr(this->components[idx_z])));
        this->components[idx_x] *= tmp;
        this->components[idx_y] *= tmp;
        this->components[idx_z] *= tmp;
    }


    /*
     * the::math::abstract_quaternion<T, S>::operator =
     */
    template<class T, class S>
    abstract_quaternion<T, S>& abstract_quaternion<T, S>::operator =(
           const abstract_quaternion& rhs) {
        THE_STACK_TRACE;
        if (this != &rhs) {
            ::memcpy(this->components, rhs.components, 4 * sizeof(T));
        }

        return *this;
    }


    /*
     * the::math::abstract_quaternion<T, S>::operator =
     */
    template<class T, class S>
    template<class Tp, class Sp>
    abstract_quaternion<T, S>& abstract_quaternion<T, S>::operator =(
           const abstract_quaternion<Tp, Sp>& rhs) {
        THE_STACK_TRACE;
        if (static_cast<void *>(this) != static_cast<const void *>(&rhs)) {
            this->components[idx_x] = static_cast<T>(rhs.x());
            this->components[idx_y] = static_cast<T>(rhs.y());
            this->components[idx_z] = static_cast<T>(rhs.z());
            this->components[idx_w] = static_cast<T>(rhs.w());
        }

        return *this;
    }



    /*
     * the::math::abstract_quaternion<T, S>::operator ==
     */
    template<class T, class S>
    bool abstract_quaternion<T, S>::operator ==(
            const abstract_quaternion& rhs) const {
        THE_STACK_TRACE;
        return (is_equal(this->components[idx_x], rhs.components[idx_x])
            && is_equal(this->components[idx_y], rhs.components[idx_y])
            && is_equal(this->components[idx_z], rhs.components[idx_z])
            && is_equal(this->components[idx_w], rhs.components[idx_w]));
    }


    /*
     * the::math::abstract_quaternion<T, S>::operator ==
     */
    template<class T, class S>
    template<class Tp, class Sp>
    bool abstract_quaternion<T, S>::operator ==(
            const abstract_quaternion<Tp, Sp>& rhs) const {
        THE_STACK_TRACE;
        return (is_equal<T>(this->components[idx_x], rhs.x())
            && is_equal<T>(this->components[idx_y], rhs.y())
            && is_equal<T>(this->components[idx_z], rhs.z())
            && is_equal<T>(this->components[idx_w], rhs.w()));
    }


    /*
     * the::math::abstract_quaternion<T, S>::operator *
     */
    template<class T, class S>
    template<class Sp>
    abstract_quaternion<T, T[4]> abstract_quaternion<T, S>::operator *(
            const abstract_quaternion<T, Sp>& rhs) const {
        THE_STACK_TRACE;
        return abstract_quaternion<T, T[4]>(
            this->components[idx_w] * rhs.x() 
            + rhs.w() * this->components[idx_x]
            + this->components[idx_y] * rhs.z() 
            - this->components[idx_z] * rhs.y(),

            this->components[idx_w] * rhs.y() 
            + rhs.w() * this->components[idx_y] 
            + this->components[idx_z] * rhs.x() 
            - this->components[idx_x] * rhs.z(),

            this->components[idx_w] * rhs.z() 
            + rhs.w() * this->components[idx_z] 
            + this->components[idx_x] * rhs.y() 
            - this->components[idx_y] * rhs.x(),

            this->components[idx_w] * rhs.w() 
            - (this->components[idx_x] * rhs.x()
            + this->components[idx_y] * rhs.y() 
            + this->components[idx_z] * rhs.z()));
    }


    /*
     * the::math::abstract_quaternion<T, S>::operator *
     */
    template<class T, class S>
    template<class Sp>
    vector<T, 3> abstract_quaternion<T, S>::operator *(
            const abstract_vector<T, 3, Sp>& rhs) const {
        THE_STACK_TRACE;
        vector<T, 3> u(this->components);
        return ((static_cast<T>(2.0) * ((u.dot(rhs) * u) 
            + (this->w() * u.cross(rhs))))
            + ((sqr(this->w()) - u.dot(u)) * rhs));
    }


    /*
     * the::math::abstract_quaternion<T, S>::idx_w
     */
    template<class T, class S> 
    const uintptr_t abstract_quaternion<T, S>::idx_w = 3;


    /*
     * the::math::abstract_quaternion<T, S>::idx_x
     */
    template<class T, class S> 
    const uintptr_t abstract_quaternion<T, S>::idx_x = 0;


    /*
     * the::math::abstract_quaternion<T, S>::idx_y
     */
    template<class T, class S> 
    const uintptr_t abstract_quaternion<T, S>::idx_y = 1;


    /*
     * the::math::abstract_quaternion<T, S>::idx_z
     */
    template<class T, class S> 
    const uintptr_t abstract_quaternion<T, S>::idx_z = 2;

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_QUATERNION_H_INCLUDED */
