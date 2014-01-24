/*
 * the/blob.h
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
 * RawStorage.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_BLOB_H_INCLUDED
#define THE_BLOB_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/memory.h"
#include "the/stack_trace.h"
#include "the/types.h"


namespace the {

    /**
     * This class wraps a block of (untyped) dynamic memory that can also be
     * resized dynamically.
     */
    class blob {

    public:

        /** The type used to specified the size in bytes of a blob. */
        typedef size_t size_type;

        /** 
         * Create a new raw storage block with the specified size.
         *
         * @param size The initial size in bytes.
         *
         * @throws std::bad_alloc If the requested memory could not be allocated.
         */
        blob(const size_type size = 0);

        /**
         * Clone 'rhs'.
         *
         * This will deep copy the whole dynamic memory.
         *
         * @param rhs The object to be cloned.
         *
         * @throws std::bad_alloc If the requested memory could not be allocated.
         */
        blob(const blob& rhs);

        /**
         * Moves the data of 'rhs' into this blob and frees the memory allocated
         * by 'rhs'.
         *
         * @param rhs The object to be moved.
         */
        blob(blob&& rhs);

        /** Dtor. */
        ~blob(void);

        /**
         * Append 'cntData' bytes of data beginning at 'data' to the end of the
         * blob. The object ensures that the specified amount of data can be 
         * appended by enlarging the blob.
         *
         * The existing this->size() bytes in the blob will not be touched.
         *
         * The caller remains owner of the memory designated by 'data'. The 
         * object will create a deep copy in this method.
         *
         * @param data    The data to be appended. It is safe to pass a NULL 
         *                pointer for 'data'. In this case, nothing will be
         *                copied, but the size is adjusted like calling
         *                assert_size(this->size() + cntData, true).
         * @param cntData The number of bytes starting at 'data' to be added.
         *
         * @return A pointer to the copy of 'data' in the blob. The object 
         *         remains owner of this data.
         *
         * @throws std::bad_alloc If the requested memory could not be allocated.
         */
        void *append(const void *data, const size_type cntData);

        /**
         * Answer the raw memory block as pointer to T. The object remains
         * owner of the memory designated by the pointer returned.
         *
         * @return The raw memory block.
         */
        template<class T> inline T *as(void) {
            THE_STACK_TRACE;
            return reinterpret_cast<T *>(this->dataPtr);
        }

        /**
         * Answer the raw memory block as pointer to T. The object remains
         * owner of the memory designated by the pointer returned.
         *
         * @return The raw memory block.
         */
        template<class T> inline const T *as(void) const {
            THE_STACK_TRACE;
            return reinterpret_cast<const T*>(this->dataPtr);
        }

        /**
         * Answer the raw memory block at an offset of 'offset' bytes as 
         * pointer to T. The object remains owner of the memory designated 
         * by the pointer returned.
         *
         * Note that the method does not perform any range checks!
         *
         * @return A pointer to the begin of the memory block + 'offset' bytes.
         */
        template<class T> inline T *as_at(const size_type offset) {
            THE_STACK_TRACE;
            return the::as_at<T>(this->dataPtr, offset);
        }

        /**
         * Answer the raw memory block at an offset of 'offset' bytes as 
         * pointer to T. The object remains owner of the memory designated 
         * by the pointer returned.
         *
         * Note that the method does not perform any range checks!
         *
         * @return A pointer to the begin of the memory block + 'offset' bytes.
         */
        template<class T> inline const T *as_at(const size_type offset) const {
            THE_STACK_TRACE;
            return the::as_at<T>(this->dataPtr, offset);
        }

        /**
         * Ensures that the dynamic memory consists of at least 'size' bytes.
         * If the memory block already has the requested size or is even larger,
         * nothing is done. If 'keepContent' is true, the content of the 
         * memory will be preserved when reallocating. Otherwise, it might be
         * deleted.
         *
         * @param size        The new size in bytes.
         * @param keepContent Set this true, for reallocating and keeping the
         *                    content.
         *
         * @return true, if the memory was reallocated, false, if the memory 
         *         block was already large enough.
         *
         * @throws std::bad_alloc If the requested memory could not be allocated.
         */
        bool assert_size(const size_type size, const bool keepContent = false);

        /**
         * Answer a pointer to the data at 'offset' byte distance from the 
         * beginning of the blob.
         *
         * Note that the method does not perform any range checks!
         *
         * @return A pointer to the begin of the memory block + 'offset' bytes.
         */
        inline void *at(const size_type offset) {
            THE_STACK_TRACE;
            return the::as_at<void>(this->dataPtr, offset);
        }

        /**
         * Answer a pointer to the data at 'offset' byte distance from the 
         * beginning of the blob.
         *
         * Note that the method does not perform any range checks!
         *
         * @return A pointer to the begin of the memory block + 'offset' bytes.
         */
        inline const void *at(const size_type offset) const {
            THE_STACK_TRACE;
            return the::as_at<void>(this->dataPtr, offset);
        }

        /**
         * Answer a pointer to the actual memory. The object remains
         * owner of the memory designated by the pointer returned.
         *
         * @return A pointer to the memory.
         */
        inline void *data(void) {
            THE_STACK_TRACE;
            return this->dataPtr;
        }

        /**
         * Answer a pointer to the actual memory. The object remains
         * owner of the memory designated by the pointer returned.
         *
         * @return A pointer to the memory.
         */
        inline const void *data(void) const {
            THE_STACK_TRACE;
            return this->dataPtr;
        }

        /**
         * Enforces the dynamic memory to contain exactly 'size' bytes. If
         * 'keepContent' is true, the memory will be reallocated so that
         * the current content remains. Otherwise, the content will be destroyed
         * and newly allocated.
         *
         * @param size        The new size in bytes.
         * @param keepContent Set this true, for reallocating and keeping the
         *                    content.
         *
         * @throws std::bad_alloc If the requested memory could not be allocated.
         */
        void enforce_size(const size_type size, const bool keepContent = false);

        /**
         * Answer the size of the memory block.
         *
         * @return The size of the memory block in bytes.
         */
        inline size_type size(void) const {
            THE_STACK_TRACE;
            return this->dataSize;
        }

        /**
         * Answer whether the memory block is empty, i. e. has no memory 
         * allocated.
         *
         * @return true, if no memory has been allocated, false otherwise.
         */
        inline bool is_empty(void) const {
            THE_STACK_TRACE;
            return (this->dataSize == 0);
        }

        /**
         * Answer whether the object has at least 'size' bytes.
         *
         * @param size The minimum size to be tested.
         *
         * @return true, if the memory the memory block is 'size' bytes or 
         *         larger, false otherwise.
         */
        inline bool is_sufficient(const size_type size) {
            THE_STACK_TRACE;
            return (this->dataSize >= size);
        }

        /**
         * Sets the whole allocated memory to zero.
         */
        void zero_all(void);

        /**
         * Assignment operation.
         *
         * This will deep copy the whole dynamic memory.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         *
         * @throws std::bad_alloc If the requested memory could not be allocated.
         */
        blob& operator =(const blob& rhs);

        /**
         * Move 'rhs' into this blob.
         *
         * The data of 'rhs' will be freed by this operation and this blob takes
         * ownership of it.
         *
         * @param rhs The right hand size operand.
         *
         * @return *this.
         */
        blob& operator =(blob&& rhs);

        /**
         * Test for equality. The content of the memory will be compared using
         * memcmp.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are equal, false otherwise.
         */
        bool operator ==(const blob& rhs) const;

        /**
         * Test for inequality. The content of the memory will be compared using
         * memcmp.
         *
         * @param rhs The right hand side operand.
         *
         * @return true, if *this and 'rhs' are not equal, false otherwise.
         */
        inline bool operator !=(const blob& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Answer a pointer to the actual memory. The object remains
         * owner of the memory designated by the pointer returned.
         *
         * @return A pointer to the memory.
         */
        inline operator void *(void) {
            THE_STACK_TRACE;
            return this->dataPtr;
        }

        /**
         * Answer a pointer to the actual memory. The object remains
         * owner of the memory designated by the pointer returned.
         *
         * @return A pointer to the memory.
         */
        inline operator const void *(void) const {
            THE_STACK_TRACE;
            return this->dataPtr;
        }

    private:

        /** Pointer to dynamic memory. */
        void *dataPtr;

        /** The size of the memory block 'data' in bytes. */
        size_type dataSize;

    };
    
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_BLOB_H_INCLUDED */
