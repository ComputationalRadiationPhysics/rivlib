/*
 * the/blob.cpp
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
 * RawStorage.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#include "the/blob.h"

#include <stdexcept>

#include "the/assert.h"
#include "the/memory.h"
#include "the/trace.h"


/*
 * the::blob::blob
 */
the::blob::blob(const size_t size) : dataPtr(nullptr), dataSize(size) {
    THE_STACK_TRACE;
    this->enforce_size(this->dataSize);
}


/*
 * the::blob::blob
 */
the::blob::blob(const blob& rhs) : dataPtr(nullptr), dataSize(rhs.dataSize) {
    THE_STACK_TRACE;
    if (this->dataSize > 0) {
        this->enforce_size(this->dataSize);
        ::memcpy(this->dataPtr, rhs.dataPtr, this->dataSize);
    }
}


/*
 * the::blob::blob
 */
the::blob::blob(blob&& rhs) : dataPtr(rhs.dataPtr), dataSize(rhs.dataSize) {
    THE_STACK_TRACE;
    rhs.dataPtr = nullptr;
    rhs.dataSize = 0;
}


/*
 * the::blob::~blob
 */
the::blob::~blob(void) {
    THE_STACK_TRACE;
    the::safe_free(this->dataPtr);
}


/*
 * the::blob::append
 */
void *the::blob::append(const void *data, const size_type cntData) {
    THE_STACK_TRACE;
    size_type offset = this->dataSize;
    void *retval = nullptr;

    this->enforce_size(this->dataSize + cntData, true);
    retval = static_cast<byte *>(this->dataPtr) + offset;

    if (data != nullptr) {
        ::memcpy(retval, data, cntData);
    }

    return retval;
}


/*
 * the::blob::assert_size
 */
bool the::blob::assert_size(const size_type size, const bool keepContent) {
    THE_STACK_TRACE;
    if (!this->is_sufficient(size)) {
        this->enforce_size(size, keepContent);
        return true;

    } else {
        return false;
    }
}


/*
 * the::blob::enforce_size
 */
void the::blob::enforce_size(const size_type size, const bool keepContent) {
    THE_STACK_TRACE;
    if ((this->dataSize = size) > 0) {

        if (keepContent) {
            THE_TRACE(THE_TRCCHL_THELIB_COMMON, THE_TRCLVL_INFO,
                "blob::assert_size reallocates %u bytes.\n",
                this->dataSize);
            this->dataPtr = ::realloc(this->dataPtr, this->dataSize);

        } else {
            //THE_TRACE(THE_TRCCHL_THELIB_COMMON, THE_TRCLVL_INFO,
            //    "blob::assert_size allocates %u bytes.\n",
            //    this->dataSize);
            the::safe_free(this->dataPtr);
            this->dataPtr = ::malloc(this->dataSize);
        }

        if (this->dataPtr == nullptr) {
            this->dataSize = 0;
            throw std::bad_alloc();
        }

    } /* end if ((this->dataSize = size) > 0) */
}


/*
 * the::blob::zero_all
 */
void the::blob::zero_all(void) {
    THE_STACK_TRACE;
    if (this->dataPtr != nullptr) {
        the::zero_memory(this->dataPtr, this->dataSize);
    }
}


/*
 * the::blob::operator =
 */
the::blob& the::blob::operator =(const blob& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        this->enforce_size(rhs.dataSize);
        ::memcpy(this->dataPtr, rhs.dataPtr, rhs.dataSize);
        THE_ASSERT(this->dataSize == rhs.dataSize);
    }
    return *this;
}


/*
 * the::blob::operator =
 */
the::blob& the::blob::operator =(blob&& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        this->dataPtr = rhs.dataPtr;
        this->dataSize = rhs.dataSize;
        rhs.dataPtr = nullptr;
        rhs.dataSize = 0;
    }
    return *this;
}


/*
 * the::blob::operator ==
 */
bool the::blob::operator ==(const blob& rhs) const {
    THE_STACK_TRACE;
    if (this != &rhs) {
        return (::memcmp(this->dataPtr, rhs.dataPtr, this->dataSize) == 0);
    } else {
        return true;
    }
}
