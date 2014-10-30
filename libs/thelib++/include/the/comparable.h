/*
 * include\the\comparable.h
 *
 * Copyright (C) 2012 TheLib Team (http://www.thelib.org/license)
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

#ifndef THE_COMPARABLE_H_INCLUDED
#define THE_COMPARABLE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/equatable.h"
#include "the/stack_trace.h"


namespace the {

    /**
     * This class derives a consistent behaviour of operator <, operator <=,
     * operator ==, operator !=, operator >, and operator >= for classes that
     * implement the compare() method.
     *
     * Subclasses (which must provide their type via the T parameter) only
     * must implement the compare() method. This template will derive the 
     * implementation of the operators and of equatable<T> from the result of
     * this method.
     */
    template<class T> class comparable : public equatable<T> {

    public:

        /** Dtor. */
        virtual ~comparable(void);

        /**
         * Compares this object to 'rhs'.
         *
         * Notes for implementors:
         *
         * Given three objects a, b, and c, the result of the compare() method
         * must satisfy the following conditions:
         * 
         * a.compare(a) == 0
         * a.compare(b) == 0 implies b.compare(a) == 0
         * a.compare(b) == 0 and b.compare(c) == 0 implies a.compare(c) == 0
         * a.compare(b) < 0 implies b.compare(a) > 0 and vice versa
         * a.compare(b) < 0 and b.compare(c) < 0 implies a.compare(c) < 0
         *
         * The return value of a.compare(b) must be negative (e.g. -1) if the
         * condition a < b holds true.
         *
         * Likewise, the return value fo a.compare(b) must be positive (e.g. 1) 
         * if the condition b > a holds true.
         *
         * @param rhs The object to be compared.
         *
         * @return A negative number if this object is less than 'rhs', 
         *         zero if this object is equal to 'rhs',
         *         a positive number if this object is larger thatn 'rhs'.
         */
        virtual int compare(const T& rhs) const = 0;

        /**
         * Determines whether this object and 'rhs' are equal.
         *
         * @param rhs The object to be compared.
         *
         * @return true if this object and 'rhs' are equal, false otherwise.
         */
        virtual bool equals(const T& rhs) const;

        /**
         * Tests whether this object is less than 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if this object is less than 'rhs', false otherwise.
         */
        inline bool operator <(const T& rhs) const {
            THE_STACK_TRACE;
            return (this->compare(rhs) < 0);
        }

        /**
         * Tests whether this object is less than or equal to 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if this object is less than or equal to 'rhs', 
         *         false otherwise.
         */
        inline bool operator <=(const T& rhs) const {
            THE_STACK_TRACE;
            return (this->compare(rhs) <= 0);
        }

        /**
         * Tests whether this object is greater than 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if this object is greater than 'rhs', false otherwise.
         */
        inline bool operator >(const T& rhs) const {
            THE_STACK_TRACE;
            return (this->compare(rhs) > 0);
        }

        /**
         * Tests whether this object is greater than or equal to 'rhs'.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if this object is greater than or equal to 'rhs',
         *         false otherwise.
         */
        inline bool operator >=(const T& rhs) const {
            THE_STACK_TRACE;
            return (this->compare(rhs) >= 0);
        }

    protected:

        /**
         * Initialises a new instance.
         */
        comparable(void);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline comparable(const comparable& rhs) {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline comparable& operator =(const comparable& rhs) {
            THE_STACK_TRACE;
            return *this;
        }

    };

} /* end namespace the */


/*
 * the::comparable<T>::~comparable
 */
template<class T> the::comparable<T>::~comparable(void) {
    THE_STACK_TRACE;
}


/*
 * the::comparable<T>::equals
 */
template<class T> bool the::comparable<T>::equals(const T& rhs) const {
    THE_STACK_TRACE;
    return (this->compare(rhs) == 0);
}


/*
 * the::comparable<T>::comparable
 */
template<class T> the::comparable<T>::comparable(void) {
    THE_STACK_TRACE;
}


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_COMPARABLE_H_INCLUDED */
