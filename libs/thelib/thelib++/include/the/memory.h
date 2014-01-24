/*
 * the/memutils.h
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
 * memutils.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). Alle Rechte vorbehalten.
 */

#ifndef THE_MEMORY_H_INCLUDED
#define THE_MEMORY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <cstdlib>
#include <memory.h>

#include "the/assert.h"
#include "the/stack_trace.h"
#include "the/types.h"


#ifndef NULL
#ifdef __cplusplus
#define NULL (0)
#else
#define NULL ((void *) 0)
#endif /* __cplusplus */
#endif /* NULL */


/* TODO: This should be moved somewhere safe */
#ifndef va_copy
#define va_copy(d,s) ((d) = (s))
#endif /* va_copy */


namespace the {

    /**
     * Returns a typed pointer, offsetted from a generic void pointer
     *
     * @param ptr The void pointer
     * @param off The offset for the pointer to be returned in bytes
     *
     * @param A typed pointer, offsetted from 'ptr' by 'off' bytes
     *
     * @tparam T The type to cast the pointer to.
     */
    template<class T> inline T *as_at(void *ptr, size_t off) {
        THE_STACK_TRACE;
        return reinterpret_cast<T *>(
            static_cast<uint8_t *>(ptr) + off);
    }


    /**
     * Returns a typed pointer, offsetted from a generic void pointer
     *
     * @param ptr The void pointer
     * @param off The offset for the pointer to be returned in bytes
     *
     * @param A typed pointer, offsetted from 'ptr' by 'off' bytes
     *
     * @tparam T The type to cast the pointer to.
     */
    template<class T> inline const T *as_at(const void *ptr, size_t off) {
        THE_STACK_TRACE;
        return reinterpret_cast<const T *>(
            static_cast<const uint8_t *>(ptr) + off);
    }


    /**
     * If not NULL, release the memory designated by 'inOutPtr' using the C++ 
     * array delete operator and set 'inOutPtr' NULL afterwards.
     *
     * @param inOutPtr The pointer designating the memory to be released.
     *
     * @tparam T The type of the objects to be deleted.
     */
    template<class T> inline void safe_array_delete(T *& inOutPtr) {
        THE_STACK_TRACE;
        if (inOutPtr != nullptr) {
            delete[] inOutPtr;
            inOutPtr = nullptr;
        }
    }


    /**
     * If not NULL, release the memory designated by 'inOutPtr' using the C++ 
     * delete operator and set 'inOutPtr' NULL afterwards.
     *
     * @param inOutPtr The pointer designating the memory to be released.
     *
     * @tparam T The type of the object to be deleted.
     */
    template<class T> inline void safe_delete(T *& inOutPtr) {
        THE_STACK_TRACE;
        if (inOutPtr != nullptr) {
            delete inOutPtr;
            inOutPtr = nullptr;
        }
    }


    /**
     * If not NULL, release the memory designated by 'inOutPtr' using the C
     * free() function and set 'inOutPtr' NULL afterwards.
     *
     * @param inOutPtr The pointer designating the memory to be released.
     *
     * @tparam T The type of the object to be freed.
     */
    template<class T> inline void safe_free(T *& inOutPtr) {
        THE_STACK_TRACE;
        if (inOutPtr != nullptr) {
            ::free(inOutPtr);
            inOutPtr = nullptr;
        }
    }


#ifdef THE_WINDOWS
    /**
     * If not NULL, release the memory designated by 'inOutPtr' using the WIN32
     * LocalFree() function and set 'inOutPtr' NULL afterwards.
     *
     * @param inOutPtr The pointer designating the memory to be released.
     *
     * @tparam T The type of the object to be freed.
     */
    template<class T> inline void safe_local_free(T *& inOutPtr) {
        THE_STACK_TRACE;
        if (inOutPtr != nullptr) {
            ::LocalFree(reinterpret_cast<HLOCAL>(inOutPtr));
            inOutPtr = nullptr;
        }
    }
#endif /* THE_WINDOWS */


    /**
     * If not NULL, release the memory designated by 'inOutPtr' using the C++
     * ::operator delete() and set 'inOutPtr' NULL afterwards.
     *
     * @param inOutPtr The pointer designating the memory to be released.
     *
     * @tparam T The type of the object to be deleted.
     */
    template<class T> inline void safe_operator_delete(T *& inOutPtr) {
        THE_STACK_TRACE;
        if (inOutPtr != nullptr) {
            ::operator delete(inOutPtr);
            inOutPtr = nullptr;
        }
    }


    /**
     * Fills the memory block beginning at 'ptr' with zeros in a more secure
     * manner than the::zero_memory.
     *
     * On Windows, this method will ensure that the memory is overwritten
     * promptly.
     *
     * On Linux, this method is equivalent to the::zero_memory;
     *
     * @param ptr  A pointer to the starting address of the block of memory to
     *             fill with zeros.
     * @param size The size of the block of memory to fill with zeros, in bytes.
     *
     * @tparam T The type of the pointer designating the memory block.
     */
    template<class T> inline void secure_zero_memory(T *ptr, size_t size) {
        THE_STACK_TRACE;
#ifdef THE_WINDOWS
        ::SecureZeroMemory(ptr, size);
#else /* THE_WINDOWS */
        zero_memory(ptr, size);
#endif /* THE_WINDOWS */
    }


    /**
     * Fills the memory block starting at 'ptr' with sizeof(T) zeros in a more 
     * secure manner than the::zero_memory.
     *
     * On Windows, this method will ensure that the memory is overwritten
     * promptly.
     *
     * On Linux, this method is equivalent to the::zero_memory;
     *
     * @param ptr  A pointer to the starting address of the block of memory to
     *             fill with zeros.
     *
     * @tparam T The type of the pointer designating the memory block.
     */
    template<class T> inline void secure_zero_memory(T *ptr) {
        THE_STACK_TRACE;
        the::secure_zero_memory(ptr, sizeof(T));
    }


    /**
     * Fills the memory block beginning at 'ptr' with zeros.
     *
     * @param ptr  A pointer to the starting address of the block of memory to
     *             fill with zeros.
     * @param size The size of the block of memory to fill with zeros, in bytes.
     *
     * @tparam T The type of the pointer designating the memory block.
     */
    template<class T> inline void zero_memory(T *ptr, size_t size) {
        THE_STACK_TRACE;
#ifdef THE_WINDOWS
        ::ZeroMemory(ptr, size);
#else /* THE_WINDOWS */
        ::memset(ptr, 0, size);
#endif /* THE_WINDOWS */
    }


    /**
     * Fills the memory block starting at 'ptr' with sizeof(T) zeros.
     *
     * @param ptr  A pointer to the starting address of the block of memory to
     *             fill with zeros.
     *
     * @tparam T The type of the pointer designating the memory block.
     */
    template<class T> inline void zero_memory(T *ptr) {
        THE_STACK_TRACE;
        the::zero_memory(ptr, sizeof(T));
    }

} /* end namespace the */


// mueller: Same as for macros above.
//#ifndef THE_WINDOWS
///**
// * Set 'size' bytes to zero beginning at 'ptr'.
// */
//#define ZeroMemory(ptr, size) memset((ptr), 0, (size))
//
//#define SecureZeroMemory ZeroMemory
//#endif /* !THE_WINDOWS */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MEMORY_H_INCLUDED */
