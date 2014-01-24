/*
 * src\system\net\simple_message_base.cpp
 *
 * Copyright (C) 2013 TheLib Team (http://www.thelib.org/license)
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

#include "the/system/net/simple_message_base.h"

#include <climits>

#include "the/index_out_of_range_exception.h"


/*
 * the::system::net::simple_message_base::~simple_message_base
 */
the::system::net::simple_message_base::~simple_message_base(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::simple_message_base::body_at
 */
const void *the::system::net::simple_message_base::body_at(
        const size_t offset) const {
    THE_STACK_TRACE;
    if (this->header()->body_size < offset) {
        return the::as_at<void>(this->body(), offset);
    } else {
        THROW_THE_EXCEPTION(index_out_of_range_exception,
            offset, static_cast<size_t>(0), this->body_size());
    }
}


/*
 * the::system::net::simple_message_base::body_at
 */
void *the::system::net::simple_message_base::body_at(const size_t offset) {
    THE_STACK_TRACE;
    if (this->header()->body_size < offset) {
        return the::as_at<void>(this->body(), offset);
    } else {
        THROW_THE_EXCEPTION(index_out_of_range_exception,
            offset, static_cast<size_t>(0), this->body_size());
    }
}


/*
 * the::system::net::simple_message_base::equals
 */
bool the::system::net::simple_message_base::equals(
        const simple_message_base& rhs) const {
    THE_STACK_TRACE;

    // Note: Order of tests ensures correct test range and performance
    if (::memcmp(this->header(), rhs.header(), this->header_size()) == 0) {
        return (::memcmp(this->body(), rhs.body(), this->body_size()) == 0);
    } else {
        return false;
    }
}


/*
 * the::system::net::simple_message_base::set_body
 */
void the::system::net::simple_message_base::set_body(const void *body, 
        const body_size_type bodySize) {
    THE_STACK_TRACE;
    // If that asserts, the child class probably does not initialise correctly.
    THE_ASSERT(this->header() != nullptr);

    if (body != nullptr) {
        if (bodySize != 0) {
            this->header()->body_size = bodySize;
        } 

        this->assert_body_size();

        THE_ASSERT(this->header() != nullptr);
        THE_ASSERT(this->body() != nullptr);
        THE_ASSERT(this->body_size() == bodySize);
        ::memcpy(this->body(), body, this->body_size());

    } else {
        /* No body was passed, force size to zero bytes. */
        this->header()->body_size = 0;
    }
}


/*
 * the::system::net::simple_message_base::operator =
 */
the::system::net::simple_message_base&
the::system::net::simple_message_base::operator =(
        const simple_message_base& rhs) {
    THE_STACK_TRACE;

    if ((this != &rhs) && (this->header() != rhs.header())) {
        void *data = this->assert_storage(rhs.body_size());
        ::memcpy(data, rhs.header(), rhs.size());
    }

    return *this;
}


/*
 * the::system::net::simple_message_base::simple_message_base
 */
the::system::net::simple_message_base::simple_message_base(void) : base() {
    THE_STACK_TRACE;
}


/*
 * the::system::net::simple_message_base::assert_storage
 */
void *the::system::net::simple_message_base::assert_storage(
        const size_t bodySize) {
    THE_STACK_TRACE;
    THE_ASSERT(bodySize <= UINT_MAX);

    header_value_type oldHeader;
    void *retval = nullptr;

    if (this->header() != nullptr) {
        ::memcpy(&oldHeader, this->header(), this->header_size());
    }
    
    if (this->assert_storage(retval, this->header_size() +  bodySize)) {
        ::memcpy(this->header(), &oldHeader, this->header_size());
    }

    THE_ASSERT(retval != nullptr);
    return retval;
}
