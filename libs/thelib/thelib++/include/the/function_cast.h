/*
 * the/functioncast.h
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
 * functioncast.h
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_FUNCTIONCAST_H_INCLUDED
#define THE_FUNCTIONCAST_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"


/**
 * The 'function_cast' is used to cast between function pointers and void 
 * pointers. If a non-void, non-function pointer is used the behaviour is
 * undefined. Be sure not to cast away any specifications of calling 
 * conventions or this might result in stack corruption.
 */

#ifdef THE_WINDOWS

#define function_cast reinterpret_cast

#else /* THE_WINDOWS */

/**
 * Utility template for implementing the cast from void pointer to an 
 * arbitrary function pointer. Template parameter 'T' is the function pointer
 * type.
 */
template<typename T> class function_cast {
public:

    /**
     * Ctor.
     *
     * @param input The function pointer
     */
    explicit function_cast(T input) {
        value.typePtr = input;
    }

    /**
     * Ctor.
     *
     * @param input The void pointer.
     */
    explicit function_cast(void * input) {
        value.voidPtr = input;
    }

    /**
     * Answers the void pointer of this cast.
     *
     * @return The void pointer of this cast.
     */
    void * void_ptr(void) {
        return value.voidPtr;
    }

    /**
     * Answers the function pointer of this cast.
     *
     * @return The function pointer of this cast.
     */
    operator T(void) {
        return value.typePtr;
    }

private:

    /** The nested helper type for this cast. */
    typedef union {
        void * voidPtr;
        T typePtr;
    } castCrowbar;

    /** The pointer of this cast. */
    castCrowbar value;

};


/**
 * Template specialisation for casts from function pointers to void pointers.
 */
template<> class function_cast<void*> {
public:

    /**
     * Ctor.
     * The template parameter 'Tp' is the function pointer type.
     *
     * @param input The function pointer.
     */
    template<typename Tp> explicit function_cast(Tp input) {
        function_cast<Tp> helper(input);
        this->ptr = helper.VoidPtr();
    }

    /**
     * Answers the void pointer of this cast.
     *
     * @return The void pointer of this cast.
     */
    operator void*(void) {
        return this->ptr;
    }

private:

    /** The pointer of this cast */
    void *ptr;

};

#endif /* THE_WINDOWS */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_FUNCTIONCAST_H_INCLUDED */
