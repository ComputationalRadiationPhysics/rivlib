/*
 * the/math/size.h
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
 * size.h  28.09.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_SIZE_H_INCLUDED
#define THE_MATH_SIZE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "the/math/abstract_size.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * This class represents sizes in D-dimensional space. 
     */
    template<class T, unsigned int D> class size 
            : public abstract_size<T, D, T[D]> {

    public:

        /**
         * Create a zero sized size.
         */
        size(void);

        /**
         * Create a size using the data from the 'size' array.
         *
         * @param size The initial value in an array. This pointer must not
         *                  be NULL.
         */
        inline size(const T *ext) {
            THE_STACK_TRACE;
            ASSERT(ext != NULL);
            ::memcpy(this->ext, ext, D * sizeof(T));
        }

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline size(const size& rhs) {
            THE_STACK_TRACE;
            ::memcpy(this->ext, rhs.ext, D * sizeof(T));
        }

        /**
         * Allows arbitrary size to size conversion. If the new
         * object has a smaller extentality, the values will be truncated,
         * if it has larger extentality, it will be padded with zeroes.
         *
         * @param rhs The object to copy.
         */
        template<class Tp, unsigned int Dp, class Sp>
        size(const abstract_size<Tp, Dp, Sp>& rhs);

        /** Dtor. */
        ~size(void);

        /**
         * Assignment operator. This operator never creates an alias.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline size& operator =(const size& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** 
         * This operator allows for arbitrary size to size 
         * assignments. If the left hand side operand has a smaller size,
         * the values will be truncated, if it has larger extentality, it 
         * will be padded with zeroes.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline size& operator =(const abstract_size<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** A typedef for the super class. */
        typedef abstract_size<T, D, T[D]> super;

    };


    /*
     * the::math::size<T, D>::size
     */
    template<class T, unsigned int D> 
    size<T, D>::size(void) {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->ext[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::size<T, D>::size
     */
    template<class T, unsigned int D> 
    template<class Tp, unsigned int Dp, class Sp>
    size<T, D>::size(const abstract_size<Tp, Dp, Sp>& rhs) {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->ext[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->ext[d] = static_cast<T>(0);
        }  
    }


    /*
     * the::math::size<T, D>::~size
     */
    template<class T, unsigned int D> 
    size<T, D>::~size(void) {
        THE_STACK_TRACE;
    }


    /**
     * Partial template specialisation for the two-extental case. This 
     * class provides an additional constructor for creating a size by
     * specifying the components separately.
     */
    template<class T> class size<T, 2>
            : public abstract_size<T, 2, T[2]> {

    public:

        /** Behaves like primary class template. */
        size(void);

        /** Behaves like primary class template. */
        inline size(const T *ext) {
            THE_STACK_TRACE;
            ASSERT(ext != NULL);
            ::memcpy(this->ext, ext, D * sizeof(T));
        }

        /**
         * Create a new size.
         *
         * @param width  The width.
         * @param height The height.
         */
        inline size(const T& width, const T& height) {
            THE_STACK_TRACE;
            this->ext[0] = width;
            this->ext[1] = height;
        }

        /** Behaves like primary class template. */
        inline size(const size& rhs) {
            THE_STACK_TRACE;
            ::memcpy(this->ext, rhs.ext, D * sizeof(T));
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        size(const abstract_size<Tp, Dp, Sp>& rhs);

        /** Behaves like primary class template. */
        ~size(void);

        /** Behaves like primary class template. */
        inline size& operator =(const size& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline size& operator =(const abstract_size<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** The extentality. */
        static const unsigned int D;

        /** A typedef for the super class. */
        typedef abstract_size<T, 2, T[2]> super;

    };


    /*
     * the::math::size<T, 2>::size
     */
    template<class T> size<T, 2>::size(void) {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->ext[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::size<T, 2>::size
     */
    template<class T> 
    template<class Tp, unsigned int Dp, class Sp>
    size<T, 2>::size(const abstract_size<Tp, Dp, Sp>& rhs) {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->ext[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->ext[d] = static_cast<T>(0);
        }  
    }


    /*
     * the::math::size<T, 2>::~size
     */
    template<class T> size<T, 2>::~size(void) {
        THE_STACK_TRACE;
    }


    /*
     * the::math::size<T, 2>::D
     */
    template<class T> const unsigned int size<T, 2>::D = 2;


    /**
     * Partial template specialisation for the three-extental case. This 
     * class provides an additional constructor for creating a size by
     * specifying the components separately.
     */
    template<class T> class size<T, 3>
            : public abstract_size<T, 3, T[3]> {

    public:

        /** Behaves like primary class template. */
        size(void);

        /** Behaves like primary class template. */
        inline size(const T *ext) {
            THE_STACK_TRACE;
            ASSERT(ext != NULL);
            ::memcpy(this->ext, ext, D * sizeof(T));
        }

        /**
         * Create a new size.
         *
         * @param width  The width.
         * @param height The height.
         * @param depth  The depth.
         */
        inline size(const T& width, const T& height, const T& depth) {
            THE_STACK_TRACE;
            this->ext[0] = width;
            this->ext[1] = height;
            this->ext[2] = depth;
        }

        /** Behaves like primary class template. */
        inline size(const size& rhs) {
            THE_STACK_TRACE;
            ::memcpy(this->ext, rhs.ext, D * sizeof(T));
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        size(const abstract_size<Tp, Dp, Sp>& rhs);

        /** Behaves like primary class template. */
        ~size(void);

        /** Behaves like primary class template. */
        inline size& operator =(const size& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

        /** Behaves like primary class template. */
        template<class Tp, unsigned int Dp, class Sp>
        inline size& operator =(const abstract_size<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator =(rhs);
            return *this;
        }

    protected:

        /** The extentality. */
        static const unsigned int D;

        /** A typedef for the super class. */
        typedef abstract_size<T, 3, T[3]> super;

    };


    /*
     * the::math::size<T, 3>::size
     */
    template<class T> size<T, 3>::size(void) {
        THE_STACK_TRACE;
        for (unsigned int d = 0; d < D; d++) {
            this->ext[d] = static_cast<T>(0);
        }
    }


    /*
     * the::math::size<T, 3>::size
     */
    template<class T> 
    template<class Tp, unsigned int Dp, class Sp>
    size<T, 3>::size(const abstract_size<Tp, Dp, Sp>& rhs) {
        THE_STACK_TRACE;
        for (unsigned int d = 0; (d < D) && (d < Dp); d++) {
            this->ext[d] = static_cast<T>(rhs[d]);
        }
        for (unsigned int d = Dp; d < D; d++) {
            this->ext[d] = static_cast<T>(0);
        }  
    }


    /*
     * the::math::size<T, 3>::~size
     */
    template<class T> size<T, 3>::~size(void) {
        THE_STACK_TRACE;
    }


    /*
     * the::math::size<T, 3>::D
     */
    template<class T> const unsigned int size<T, 3>::D = 3;

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_SIZE_H_INCLUDED */
