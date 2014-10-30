/*
 * the/math/abstract_size.h
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
 * AbstractDimension.h  28.09.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_SIZE_H_INCLUDED
#define THE_MATH_ABSTRACT_SIZE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/math/abstract_size_impl.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * This class represents sizes in D-dimensional space. 
     */
    template<class T, unsigned int D, class S> class abstract_size 
            : public abstract_size_impl<T, D, S, abstract_size> {

    public:

        /** Dtor. */
        ~abstract_size(void);

        /**
         * Assignment operator. This operator never creates an alias.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline abstract_size& operator =(const abstract_size& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** 
         * This operator allows for arbitrary dimension to dimension 
         * assignments. If the left hand side operand has a smaller dimension,
         * the values will be truncated, if it has larger dimensionality, it 
         * will be padded with zeroes.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_size& operator =(
                const abstract_size<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** Typedef of super class. */
        typedef abstract_size_impl<T, D, S, the::math::abstract_size> 
            super;

        /**
         * Disallow instances of this class.
         */
        inline abstract_size(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_size_impl;
    };


    /*
     * the::math::abstract_size<T, D, S>::~abstract_size
     */
    template<class T, unsigned int D, class S>
    abstract_size<T, D, S>::~abstract_size(void) {
        THE_STACK_TRACE;
    }


    /**
     * This is the partial template specialisation for two-dimensional
     * dimensions. It provides additional named accessors for the components
     * of the dimension.
     */
    template<class T, class S> class abstract_size<T, 2, S> 
            : public abstract_size_impl<T, 2, S, abstract_size> {

    public:

        /** Behaves like primary class template. */
        ~abstract_size(void);

        /**
         * Answer the height of the dimension.
         *
         * @return The height.
         */
        inline const T& get_height(void) const {
            THE_STACK_TRACE;
            return this->ext[1];
        }

        /**
         * Answer the width of the dimension.
         *
         * @return The width.
         */
        inline const T& get_width(void) const {
            THE_STACK_TRACE;
            return this->ext[0];
        }

        /**
         * Answer the height of the dimension.
         *
         * @return The height.
         */
        inline const T& height(void) const {
            THE_STACK_TRACE;
            return this->ext[1];
        }

        /**
         * set the components of the dimension.
         *
         * @param width  The new width.
         * @param height The new height.
         */
        inline void set(const T& width, const T& height) {
            THE_STACK_TRACE;
            this->ext[0] = width;
            this->ext[1] = height;
        }

        /**
         * Change the height.
         *
         * @param height The new height
         */
        inline void set_height(const T& height) {
            THE_STACK_TRACE;
            this->ext[1] = height;
        }

        /**
         * Change the width.
         *
         * @param height The new width
         */
        inline void set_width(const T& width) {
            THE_STACK_TRACE;
            this->ext[0] = width;
        }

        /**
         * Answer the width of the dimension.
         *
         * @return The width.
         */
        inline const T& width(void) const {
            THE_STACK_TRACE;
            return this->ext[0];
        }

        /** Behaves like primary class template. */
        inline abstract_size& operator =(const abstract_size& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_size& operator =(
                const abstract_size<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** Typedef of super class. */
        typedef abstract_size_impl<T, 2, S, the::math::abstract_size> 
            super;

        /**
         * Disallow instances of this class.
         */
        inline abstract_size(void) : super() {}

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_size_impl;
    };


    /*
     * the::math::abstract_size<T, 2, S>::~abstract_size
     */
    template<class T, class S>
    abstract_size<T, 2, S>::~abstract_size(void) {
        THE_STACK_TRACE;
    }


    /**
     * This is the partial template specialisation for three-dimensional
     * dimensions. It provides additional named accessors for the components
     * of the dimension.
     */
    template<class T, class S> class abstract_size<T, 3, S> 
            : public abstract_size_impl<T, 3, S, abstract_size> {

    public:

        /** Behaves like primary class template. */
        ~abstract_size(void);

        /**
         * Answer the depth of the dimension.
         *
         * @return The depth.
         */
        inline const T& depth(void) const {
            THE_STACK_TRACE;
            return this->ext[2];
        }

        /**
         * Answer the depth of the dimension.
         *
         * @return The depth.
         */
        inline const T& get_depth(void) const {
            THE_STACK_TRACE;
            return this->ext[2];
        }

        /**
         * Answer the height of the dimension.
         *
         * @return The height.
         */
        inline const T& get_height(void) const {
            THE_STACK_TRACE;
            return this->ext[1];
        }

        /**
         * Answer the width of the dimension.
         *
         * @return The width.
         */
        inline const T& get_width(void) const {
            THE_STACK_TRACE;
            return this->ext[0];
        }

        /**
         * Answer the height of the dimension.
         *
         * @return The height.
         */
        inline const T& height(void) const {
            THE_STACK_TRACE;
            return this->ext[1];
        }

        /**
         * set the components of the dimension.
         *
         * @param width  The new width.
         * @param height The new height.
         * @param depth  The new depth.
         */
        inline void set(const T& width, const T& height, const T& depth) {
            THE_STACK_TRACE;
            this->ext[0] = width;
            this->ext[1] = height;
            this->ext[2] = depth;
        }

        /**
         * Change the depth.
         *
         * @param depth The new depth
         */
        inline void set_depth(const T& depth) {
            THE_STACK_TRACE;
            this->ext[2] = depth;
        }

        /**
         * Change the height.
         *
         * @param height The new height
         */
        inline void set_height(const T& height) {
            THE_STACK_TRACE;
            this->ext[1] = height;
        }

        /**
         * Change the width.
         *
         * @param height The new width
         */
        inline void set_width(const T& width) {
            THE_STACK_TRACE;
            this->ext[0] = width;
        }

        /**
         * Answer the width of the dimension.
         *
         * @return The width.
         */
        inline const T& width(void) const {
            THE_STACK_TRACE;
            return this->ext[0];
        }

        /** Behaves like primary class template. */
        inline abstract_size& operator =(const abstract_size& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_size& operator =(
                const abstract_size<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** Typedef of super class. */
        typedef abstract_size_impl<T, 3, S, the::math::abstract_size> 
            super;

        /**
         * Disallow instances of this class.
         */
        inline abstract_size(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1, 
            template<class Tf2, unsigned int Df2, class Sf2> class Cf> 
            friend class abstract_size_impl;
    };


    /*
     * the::math::abstract_size<T, 3, S>::~abstract_size
     */
    template<class T, class S>
    abstract_size<T, 3, S>::~abstract_size(void) {
        THE_STACK_TRACE;
    }

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_SIZE_H_INCLUDED */
