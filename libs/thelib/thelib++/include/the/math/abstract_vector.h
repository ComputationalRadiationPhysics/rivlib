/*
 * the/math/abstract_vector.h
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
 * Abstractvector.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller. Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_VECTOR_H_INCLUDED
#define THE_MATH_ABSTRACT_VECTOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/config.h"
#include "the/math/abstract_vector_impl.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     *
     */
    template<class T, unsigned int D, class S> class abstract_vector 
            : public abstract_vector_impl<T, D, S, abstract_vector> {

    public:

        /** Dtor. */
        ~abstract_vector(void);

        /**
         * Assignment.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         */
        inline abstract_vector& operator =(const abstract_vector& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
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
        inline abstract_vector& operator =(
                const abstract_vector<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** Typedef for our super class. */
        typedef abstract_vector_impl<T, D, S, the::math::abstract_vector> super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_vector(void) : super() {}

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_vector_impl;
    };


    /*
     * the::math::abstract_vector<T, D, S>::~abstract_vector
     */
    template<class T, unsigned int D, class S>
    abstract_vector<T, D, S>::~abstract_vector(void) {
        THE_STACK_TRACE;
    }


    /**
     * Partial template specialisation for two-dimensional vectors. This
     * specialisation provides named accessors to the vector's components.
     */
    template<class T, class S> class abstract_vector<T, 2, S> 
            : public abstract_vector_impl<T, 2, S, abstract_vector> {

    public:

        /** Behaves like primary class template. */
        ~abstract_vector(void);

        /**
         * Answer the x-component of the vector.
         *
         * @return The x-component of the vector.
         */
        inline const T& get_x(void) const {
            THE_STACK_TRACE;
            return this->components[0];
        }

        /**
         * Answer the y-component of the vector.
         *
         * @return The y-component of the vector.
         */
        inline const T& get_y(void) const {
            THE_STACK_TRACE;
            return this->components[1];
        }

        /**
         * set the three components of the vector.
         *
         * @param x The new x-component.
         * @param y The new y-component.
         */
        inline void set(const T& x, const T& y) {
            THE_STACK_TRACE;
            this->components[0] = x;
            this->components[1] = y;
        }

        /**
         * set the x-component of the vector.
         *
         * @param x The new x-component.
         */
        inline void set_x(const T& x) {
            THE_STACK_TRACE;
            this->components[0] = x;
        }

        /**
         * set the y-component of the vector.
         *
         * @param y The new y-component.
         */
        inline void set_y(const T& y) {
            THE_STACK_TRACE;
            this->components[1] = y;
        }

        /**
         * Answer the x-component of the vector.
         *
         * @return The x-component of the vector.
         */
        inline const T& x(void) const {
            THE_STACK_TRACE;
            return this->components[0];
        }

        /**
         * Answer the y-component of the vector.
         *
         * @return The y-component of the vector.
         */
        inline const T& y(void) const {
            THE_STACK_TRACE;
            return this->components[1];
        }

        /** Behaves like primary class template. */
        inline abstract_vector& operator =(const abstract_vector& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_vector& operator =(
                const abstract_vector<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** Typedef for our super class. */
        typedef abstract_vector_impl<T, 2, S, the::math::abstract_vector> super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_vector(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_vector_impl;
    };


    /*
     * the::math::abstract_vector<T, 2, S>::~abstract_vector
     */
    template<class T, class S>
    abstract_vector<T, 2, S>::~abstract_vector(void) {
        THE_STACK_TRACE;
    }


    /**
     * Partial template specialisation for three-dimensional vectors. This
     * specialisation provides named accessors to the vector's components
     * and some special operations.
     */
    template<class T, class S> class abstract_vector<T, 3, S> 
            : public abstract_vector_impl<T, 3, S, abstract_vector> {

    public:

        /** Dtor. */
        ~abstract_vector(void);

       /**
         * Answer the cross product of this vector and 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return The cross product of this vector and 'rhs'.
         */
        template<class Tp, class Sp> abstract_vector<T, 3, T[3]> cross(
            const abstract_vector<Tp, 3, Sp>& rhs) const;

        ///**
        // * Calculate the cross product of this vector and 'rhs' and assign it
        // * to this vector.
        // *
        // * @param rhs The right hand side operand.
        // *
        // * @return *this.
        // */
        //template<class Tp, class Sp>
        //inline abstract_vector3D& cross_assign(
        //        const abstract_vector3D<Tp, Sp>& rhs) {
        //    return (*this = this->cross(rhs));
        //}

        /**
         * Answer the x-component of the vector.
         *
         * @return The x-component of the vector.
         */
        inline const T& get_x(void) const {
            THE_STACK_TRACE;
            return this->components[0];
        }

        /**
         * Answer the y-component of the vector.
         *
         * @return The y-component of the vector.
         */
        inline const T& get_y(void) const {
            THE_STACK_TRACE;
            return this->components[1];
        }

        /**
         * Answer the z-component of the vector.
         *
         * @return The z-component of the vector.
         */
        inline const T& get_z(void) const {
            THE_STACK_TRACE;
            return this->components[2];
        }

        /**
         * set the three components of the vector.
         *
         * @param x The new x-component.
         * @param y The new y-component.
         * @param z The new z-component.
         */
        inline void set(const T& x, const T& y, const T& z) {
            THE_STACK_TRACE;
            this->components[0] = x;
            this->components[1] = y;
            this->components[2] = z;
        }

        /**
         * set the x-component of the vector.
         *
         * @param x The new x-component.
         */
        inline void set_x(const T& x) {
            THE_STACK_TRACE;
            this->components[0] = x;
        }

        /**
         * set the y-component of the vector.
         *
         * @param y The new y-component.
         */
        inline void set_y(const T& y) {
            THE_STACK_TRACE;
            this->components[1] = y;
        }

        /**
         * set the z-component of the vector.
         *
         * @param z The new z-component.
         */
        inline void set_z(const T& z) {
            THE_STACK_TRACE;
            this->components[2] = z;
        }

        /**
         * Answer the x-component of the vector.
         *
         * @return The x-component of the vector.
         */
        inline const T& x(void) const {
            THE_STACK_TRACE;
            return this->components[0];
        }

        /**
         * Answer the y-component of the vector.
         *
         * @return The y-component of the vector.
         */
        inline const T& y(void) const {
            THE_STACK_TRACE;
            return this->components[1];
        }

        /**
         * Answer the z-component of the vector.
         *
         * @return The z-component of the vector.
         */
        inline const T& z(void) const {
            THE_STACK_TRACE;
            return this->components[2];
        }

        /** Behaves like primary class template. */
        inline abstract_vector& operator =(const abstract_vector& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_vector& operator =(
                const abstract_vector<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }


    protected:

        /** Typedef for our super class. */
        typedef abstract_vector_impl<T, 3, S, the::math::abstract_vector> super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_vector(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_vector_impl;
        template<class Tf1, unsigned int Df1, class Sf1> 
            friend class abstract_vector;
    };


    /*
     * the::math::abstract_vector<T, 3, S>::~abstract_vector
     */
    template<class T, class S>
    abstract_vector<T, 3, S>::~abstract_vector(void) {
        THE_STACK_TRACE;
    }


    /*
     * the::math::abstract_vector<T, S>::cross
     */
    template<class T, class S>
    template<class Tp, class Sp> 
    abstract_vector<T, 3, T[3]> abstract_vector<T, 3, S>::cross(
            const abstract_vector<Tp, 3, Sp>& rhs) const {
        THE_STACK_TRACE;
        abstract_vector<T, 3, T[3]> retval;
        retval.set(
            this->components[1] * static_cast<T>(rhs[2])
            - this->components[2] * static_cast<T>(rhs[1]),
            this->components[2] * static_cast<T>(rhs[0])
            - this->components[0] * static_cast<T>(rhs[2]),
            this->components[0] * static_cast<T>(rhs[1])
            - this->components[1] * static_cast<T>(rhs[0]));
        return retval;
    }


    /**
     * Partial template specialisation for four-dimensional vectors. This
     * specialisation provides named accessors to the vector's components.
     */
    template<class T, class S> class abstract_vector<T, 4, S> 
            : public abstract_vector_impl<T, 4, S, abstract_vector> {

    public:

        /** Dtor. */
        ~abstract_vector(void);

        /**
         * Answer the x-component of the vector.
         *
         * @return The x-component of the vector.
         */
        inline const T& get_x(void) const {
            THE_STACK_TRACE;
            return this->components[0];
        }

        /**
         * Answer the y-component of the vector.
         *
         * @return The y-component of the vector.
         */
        inline const T& get_y(void) const {
            THE_STACK_TRACE;
            return this->components[1];
        }

        /**
         * Answer the z-component of the vector.
         *
         * @return The z-component of the vector.
         */
        inline const T& get_z(void) const {
            THE_STACK_TRACE;
            return this->components[2];
        }

        /**
         * Answer the w-component of the vector.
         *
         * @return The w-component of the vector.
         */
        inline const T& get_w(void) const {
            THE_STACK_TRACE;
            return this->components[3];
        }

        /**
         * set the three components of the vector.
         *
         * @param x The new x-component.
         * @param y The new y-component.
         * @param z The new z-component.
         * @param w The new w-component.
         */
        inline void set(const T& x, const T& y, const T& z, const T& w) {
            THE_STACK_TRACE;
            this->components[0] = x;
            this->components[1] = y;
            this->components[2] = z;
            this->components[3] = w;
        }

        /**
         * set the x-component of the vector.
         *
         * @param x The new x-component.
         */
        inline void set_x(const T& x) {
            THE_STACK_TRACE;
            this->components[0] = x;
        }

        /**
         * set the y-component of the vector.
         *
         * @param y The new y-component.
         */
        inline void set_y(const T& y) {
            THE_STACK_TRACE;
            this->components[1] = y;
        }

        /**
         * set the z-component of the vector.
         *
         * @param z The new z-component.
         */
        inline void set_z(const T& z) {
            THE_STACK_TRACE;
            this->components[2] = z;
        }

        /**
         * set the w-component of the vector.
         *
         * @param w The new w-component.
         */
        inline void set_w(const T& w) {
            THE_STACK_TRACE;
            this->components[3] = w;
        }

        /**
         * Answer the x-component of the vector.
         *
         * @return The x-component of the vector.
         */
        inline const T& x(void) const {
            THE_STACK_TRACE;
            return this->components[0];
        }

        /**
         * Answer the y-component of the vector.
         *
         * @return The y-component of the vector.
         */
        inline const T& y(void) const {
            THE_STACK_TRACE;
            return this->components[1];
        }

        /**
         * Answer the z-component of the vector.
         *
         * @return The z-component of the vector.
         */
        inline const T& z(void) const {
            THE_STACK_TRACE;
            return this->components[2];
        }

        /**
         * Answer the w-component of the vector.
         *
         * @return The w-component of the vector.
         */
        inline const T& w(void) const {
            THE_STACK_TRACE;
            return this->components[3];
        }

        /** Behaves like primary class template. */
        inline abstract_vector& operator =(const abstract_vector& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_vector& operator =(
                const abstract_vector<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }


    protected:

        /** Typedef for our super class. */
        typedef abstract_vector_impl<T, 4, S, the::math::abstract_vector> super;

        /**
         * Disallow instances of this class. 
         */
        inline abstract_vector(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_vector_impl;
        template<class Tf1, unsigned int Df1, class Sf1> 
            friend class abstract_vector;
    };


    /*
     * the::math::abstract_vector<T, 4, S>::~abstract_vector
     */
    template<class T, class S>
    abstract_vector<T, 4, S>::~abstract_vector(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


    /**
     * Scalar multiplication from left.
     *
     * @param lhs The left hand side operand, the scalar.
     * @param rhs The right hand side operand, the vector.
     *
     * @return The result of the scalar multiplication.
     */
    template<class T, unsigned int D, class S> 
    abstract_vector<T, D, T[D]> operator *(const T lhs, 
            const abstract_vector<T, D, S>& rhs) {
        THE_STACK_TRACE;
        return rhs * lhs;
    }


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_VECTOR_H_INCLUDED */
