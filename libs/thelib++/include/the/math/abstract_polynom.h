/*
 * the/math/abstract_polynom.h
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
 * Abstractpolynom.h
 *
 * Copyright (C) 2006 - 2010 by Visualisierungsinstitut Universitaet Stuttgart. 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_POLYNOM_H_INCLUDED
#define THE_MATH_ABSTRACT_POLYNOM_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/math/abstract_polynom_impl.h"
#include "the/not_supported_exception.h"
#include "the/stack_trace.h"


namespace the {
namespace math {

    /**
     * All polynom implementations must inherit from this class. Do not
     * inherit directly from abstract_polynom_impl as the abstraction layer of
     * abstract_polynom ensures that the implementation can work correctly and
     * instantiate derived classes.
     *
     * The one-dimensional polynom is defined by its coefficients a_0 ... a_d
     * as:
     *  f(x) := a_d * x^d + a_{d-1} * x^{d-1} + ... + a_1 * x + a_0
     *
     * T scalar type
     * D Degree of the polynom
     * S Coefficient storage
     */
    template<class T, unsigned int D, class S>
    class abstract_polynom
        : public abstract_polynom_impl<T, D, S, abstract_polynom> {
    public:

        /** Dtor. */
        ~abstract_polynom(void);

        /**
         * Finds the roots of the polynom. If roots only touch the x-axis they
         * will be present more than once in the output. If the output array
         * as not enough space to store all found roots only up to 'size'
         * roots will be stored. The order of the roots is undefined.
         *
         * A polynom of degree D has a maximum number of D roots.
         *
         * @param outRoots Pointer to the array to receive the found roots
         * @param size The size of the array to receive the found roots in
         *             number of elements.
         *
         * @return The number of valid roots in the out parameter.
         */
        inline unsigned int find_roots(T *outRoots, unsigned int size) const {
            THE_STACK_TRACE;
            if (size == 0) return 0;
            switch (this->effective_degree()) {
                case 0: return 0;
                case 1: return find_roots_deg1(this->coefficients[0],
                            this->coefficients[1], outRoots, size);
                case 2: return find_roots_deg2(this->coefficients[0],
                            this->coefficients[1], this->coefficients[2],
                            outRoots, size);
                case 3: return find_roots_deg3(this->coefficients[0],
                            this->coefficients[1], this->coefficients[2],
                            this->coefficients[3], outRoots, size);
                case 4: return find_roots_deg4(this->coefficients[0],
                            this->coefficients[1], this->coefficients[2],
                            this->coefficients[3], this->coefficients[4],
                            outRoots, size);
                default: break;
            }

            // TODO: Implement numeric root finding (bairstow?)

            throw not_supported_exception("find_roots",
                __FILE__, __LINE__);

            return 0;
        }

        /**
         * Assignment operator
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         *
         * @throw IllegalParamException if 'rhs' has an effective degree larger
         *        than D.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_polynom<T, D, S>& operator=(
                const abstract_polynom<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator=(rhs);
            return *this;
        }

    protected:

        /** A typedef for the super class. */
        typedef abstract_polynom_impl<T, D, S, the::math::abstract_polynom>
            super;

        /** Ctor. */
        inline abstract_polynom(void) : super() {
            THE_STACK_TRACE;
        }

    private:

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1,
            template<class Tf2, unsigned int Df2, class Sf2> class Cf>
            friend class abstract_polynom_impl;

    };


    /*
     * abstract_polynom<T, D, S>::~abstract_polynom
     */
    template<class T, unsigned int D, class S>
    abstract_polynom<T, D, S>::~abstract_polynom(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


    /**
     * Partial template specialisation for polynoms of degree 1.
     */
    template<class T, class S>
    class abstract_polynom<T, 1, S> :
        public abstract_polynom_impl<T, 1, S, abstract_polynom> {
    public:

        /** Dtor. */
        ~abstract_polynom(void);

        /**
         * Finds the roots of the polynom. If roots only touch the x-axis they
         * will be present more than once in the output. If the output array
         * as not enough space to store all found roots only up to 'size'
         * roots will be stored. The order of the roots is undefined.
         *
         * A polynom of degree D has a maximum number of D roots.
         *
         * @param outRoots Pointer to the array to receive the found roots
         * @param size The size of the array to receive the found roots in
         *             number of elements.
         *
         * @return The number of valid roots in the out parameter.
         */
        inline unsigned int find_roots(T *outRoots, unsigned int size) const {
            THE_STACK_TRACE;
            if (size == 0) return 0;
            if (is_equal(this->coefficients[1], static_cast<T>(0))) return 0;
            return find_roots_deg1(this->coefficients[0], this->coefficients[1],
                outRoots, size);
        }

        /**
         * Finds the root of the polynom.
         *
         * A polynom of degree 1 has a maximum number of 1 root.
         *
         * @param outRoot1 Variable to receive the root.
         *
         * @return The number of valid roots in the out parameter.
         */
        inline unsigned int find_roots(T& outRoot) const {
            THE_STACK_TRACE;
            return this->find_roots(&outRoot, 1);
        }

        /**
         * Finds the root of the polynom.
         *
         * A polynom of degree 1 has a maximum number of 1 root.
         *
         * @param outRoot Variable to receive the root.
         *
         * @return True if there is a root and it has been written to outRoot.
         */
        inline bool find_root(T& outRoot) const {
            THE_STACK_TRACE;
            return (this->find_roots(&outRoot, 1) == 1);
        }

        /**
         * Assignment operator
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         *
         * @throw IllegalParamException if 'rhs' has an effective degree larger
         *        than D.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_polynom<T, 1, S>& operator=(
                const abstract_polynom<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator=(rhs);
            return *this;
        }

    protected:

        /** A typedef for the super class. */
        typedef abstract_polynom_impl<T, 1, S, the::math::abstract_polynom>
            super;

        /** Ctor. */
        inline abstract_polynom(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1,
            template<class Tf2, unsigned int Df2, class Sf2> class Cf>
            friend class abstract_polynom_impl;

    };


    /*
     * abstract_polynom<T, 1, S>::~abstract_polynom
     */
    template<class T, class S>
    abstract_polynom<T, 1, S>::~abstract_polynom(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


    /**
     * Partial template specialisation for polynoms of degree 2.
     */
    template<class T, class S>
    class abstract_polynom<T, 2, S> :
        public abstract_polynom_impl<T, 2, S, abstract_polynom> {
    public:

        /** Dtor. */
        ~abstract_polynom(void);

        /**
         * Finds the roots of the polynom. If roots only touch the x-axis they
         * will be present more than once in the output. If the output array
         * as not enough space to store all found roots only up to 'size'
         * roots will be stored. The order of the roots is undefined.
         *
         * A polynom of degree D has a maximum number of D roots.
         *
         * @param outRoots Pointer to the array to receive the found roots
         * @param size The size of the array to receive the found roots in
         *             number of elements.
         *
         * @return The number of valid roots in the out parameter.
         */
        inline unsigned int find_roots(T *outRoots, unsigned int size) const {
            THE_STACK_TRACE;
            if (size == 0) return 0;
            if (is_equal(this->coefficients[2], static_cast<T>(0))) {
                if (is_equal(this->coefficients[1], static_cast<T>(0))) {
                    return 0;
                }
                return find_roots_deg1(this->coefficients[0],
                    this->coefficients[1], outRoots, size);
            }
            return find_roots_deg2(this->coefficients[0], this->coefficients[1],
                this->coefficients[2], outRoots, size);
        }

        /**
         * Finds the roots of the polynom. If roots only touch the x-axis they
         * will be present more than once in the output. The order of the roots
         * is undefined.
         *
         * A polynom of degree 2 has a maximum number of 2 roots.
         *
         * @param outRoot1 Variable to receive the first root.
         * @param outRoot2 Variable to receive the second root.
         *
         * @return The number of valid roots in the out parameter.
         */
        inline unsigned int find_roots(T& outRoot1, T& outRoot2) const {
            THE_STACK_TRACE;
            T r[2];
            unsigned int rv = this->find_roots(r, 2);
            if (r > 0) outRoot1 = r[0];
            if (r > 1) outRoot2 = r[1];
            return rv;
        }

        /**
         * Assignment operator
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         *
         * @throw IllegalParamException if 'rhs' has an effective degree larger
         *        than D.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_polynom<T, 2, S>& operator=(
                const abstract_polynom<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator=(rhs);
            return *this;
        }

    protected:

        /** A typedef for the super class. */
        typedef abstract_polynom_impl<T, 2, S, the::math::abstract_polynom>
            super;

        /** Ctor. */
        inline abstract_polynom(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1,
            template<class Tf2, unsigned int Df2, class Sf2> class Cf>
            friend class abstract_polynom_impl;

    };


    /*
     * abstract_polynom<T, 2, S>::~abstract_polynom
     */
    template<class T, class S>
    abstract_polynom<T, 2, S>::~abstract_polynom(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


    /**
     * Partial template specialisation for polynoms of degree 3.
     */
    template<class T, class S>
    class abstract_polynom<T, 3, S> :
        public abstract_polynom_impl<T, 3, S, abstract_polynom> {
    public:

        /** Dtor. */
        ~abstract_polynom(void);

        /**
         * Finds the roots of the polynom. If roots only touch the x-axis they
         * will be present more than once in the output. If the output array
         * as not enough space to store all found roots only up to 'size'
         * roots will be stored. The order of the roots is undefined.
         *
         * A polynom of degree D has a maximum number of D roots.
         *
         * @param outRoots Pointer to the array to receive the found roots
         * @param size The size of the array to receive the found roots in
         *             number of elements.
         *
         * @return The number of valid roots in the out parameter.
         */
        inline unsigned int find_roots(T *outRoots, unsigned int size) const {
            THE_STACK_TRACE;
            if (size == 0) return 0;
            if (is_equal(this->coefficients[3], static_cast<T>(0))) {
                if (is_equal(this->coefficients[2], static_cast<T>(0))) {
                    if (is_equal(this->coefficients[1], static_cast<T>(0))) {
                        return 0;
                    }
                    return this->find_roots_deg1(this->coefficients[0],
                        this->coefficients[1], outRoots, size);
                }
                return this->find_roots_deg2(this->coefficients[0],
                    this->coefficients[1], this->coefficients[2], outRoots,
                    size);
            }
            return this->find_roots_deg3(this->coefficients[0], 
                this->coefficients[1], this->coefficients[2],
                this->coefficients[3], outRoots, size);
        }

        /**
         * Finds the roots of the polynom. If roots only touch the x-axis they
         * will be present more than once in the output. The order of the roots
         * is undefined.
         *
         * A polynom of degree 3 has a maximum number of 3 roots.
         *
         * @param outRoot1 Variable to receive the first root.
         * @param outRoot2 Variable to receive the second root.
         * @param outRoot3 Variable to receive the third root.
         *
         * @return The number of valid roots in the out parameter.
         */
        inline unsigned int find_roots(T& outRoot1, T& outRoot2, T& outRoot3)
                const {
            THE_STACK_TRACE;
            T r[3];
            unsigned int rv = this->find_roots(r, 3);
            if (r > 0) outRoot1 = r[0];
            if (r > 1) outRoot2 = r[1];
            if (r > 2) outRoot3 = r[2];
            return rv;
        }

        /**
         * Assignment operator
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         *
         * @throw IllegalParamException if 'rhs' has an effective degree larger
         *        than D.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_polynom<T, 3, S>& operator=(
                const abstract_polynom<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator=(rhs);
            return *this;
        }

    protected:

        /** A typedef for the super class. */
        typedef abstract_polynom_impl<T, 3, S, the::math::abstract_polynom>
            super;

        /** Ctor. */
        inline abstract_polynom(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1,
            template<class Tf2, unsigned int Df2, class Sf2> class Cf>
            friend class abstract_polynom_impl;

    };


    /*
     * abstract_polynom<T, 3, S>::~abstract_polynom
     */
    template<class T, class S>
    abstract_polynom<T, 3, S>::~abstract_polynom(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


    /**
     * Partial template specialisation for polynoms of degree 4.
     */
    template<class T, class S>
    class abstract_polynom<T, 4, S> :
        public abstract_polynom_impl<T, 4, S, abstract_polynom> {
    public:

        /** Dtor. */
        ~abstract_polynom(void);

        /**
         * Finds the roots of the polynom. If roots only touch the x-axis they
         * will be present more than once in the output. If the output array
         * as not enough space to store all found roots only up to 'size'
         * roots will be stored. The order of the roots is undefined.
         *
         * A polynom of degree D has a maximum number of D roots.
         *
         * @param outRoots Pointer to the array to receive the found roots
         * @param size The size of the array to receive the found roots in
         *             number of elements.
         *
         * @return The number of valid roots in the out parameter.
         */
        inline unsigned int find_roots(T *outRoots, unsigned int size) const {
            THE_STACK_TRACE;
            if (size == 0) return 0;
            if (is_equal(this->coefficients[4], static_cast<T>(0))) {
                if (is_equal(this->coefficients[3], static_cast<T>(0))) {
                    if (is_equal(this->coefficients[2], static_cast<T>(0))) {
                        if (is_equal(this->coefficients[1],
                                static_cast<T>(0))) {
                            return 0;
                        }
                        return this->find_roots_deg1(this->coefficients[0],
                            this->coefficients[1], outRoots, size);
                    }
                    return this->find_roots_deg2(this->coefficients[0],
                        this->coefficients[1], this->coefficients[2],
                        outRoots, size);
                }
                return this->find_roots_deg3(this->coefficients[0],
                    this->coefficients[1], this->coefficients[2],
                    this->coefficients[3], outRoots, size);
            }
            return this->find_roots_deg4(this->coefficients[0],
                this->coefficients[1], this->coefficients[2], 
                this->coefficients[3], this->coefficients[4],
                outRoots, size);
        }

        /**
         * Finds the roots of the polynom. If roots only touch the x-axis they
         * will be present more than once in the output. The order of the roots
         * is undefined.
         *
         * A polynom of degree 4 has a maximum number of 4 roots.
         *
         * @param outRoot1 Variable to receive the first root.
         * @param outRoot2 Variable to receive the second root.
         * @param outRoot3 Variable to receive the third root.
         * @param outRoot4 Variable to receive the fourth root.
         *
         * @return The number of valid roots in the out parameter.
         */
        inline unsigned int find_roots(T& outRoot1, T& outRoot2, T& outRoot3,
                T& outRoot4) const {
            THE_STACK_TRACE;
            T r[4];
            unsigned int rv = this->findRoots<4>(r, 4);
            if (r > 0) outRoot1 = r[0];
            if (r > 1) outRoot2 = r[1];
            if (r > 2) outRoot3 = r[2];
            if (r > 3) outRoot4 = r[3];
            return rv;
        }

        /**
         * Assignment operator
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         *
         * @throw IllegalParamException if 'rhs' has an effective degree larger
         *        than D.
         */
        template<class Tp, unsigned int Dp, class Sp>
        inline abstract_polynom<T, 4, S>& operator=(
                const abstract_polynom<Tp, Dp, Sp>& rhs) {
            THE_STACK_TRACE;
            super::operator=(rhs);
            return *this;
        }

    protected:

        /** A typedef for the super class. */
        typedef abstract_polynom_impl<T, 4, S, the::math::abstract_polynom>
            super;

        /** Ctor. */
        inline abstract_polynom(void) : super() {
            THE_STACK_TRACE;
        }

        /* Allow instances created by the implementation class. */
        template<class Tf1, unsigned int Df1, class Sf1,
            template<class Tf2, unsigned int Df2, class Sf2> class Cf>
            friend class abstract_polynom_impl;

    };


    /*
     * abstract_polynom<T, 4, S>::~abstract_polynom
     */
    template<class T, class S>
    abstract_polynom<T, 4, S>::~abstract_polynom(void) {
        THE_STACK_TRACE;
        // intentionally empty
    }


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_POLYNOM_H_INCLUDED */
