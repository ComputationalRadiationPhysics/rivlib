/*
 * src\system\net\simple_message.cpp
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

#include "the/system/net/simple_message.h"



/*
 * the::system::net::simple_message::simple_message
 */
the::system::net::simple_message::simple_message(const body_size_type bodySize)
        : base() {
    THE_STACK_TRACE;
    // This will force the superclass to (i) allocate memory for the message
    // header and the body itself and (ii) to update the message header pointer.
    base::assert_storage(bodySize);

    // Initialise the message ID and set the body size.
    this->header()->id = 0;
    this->header()->body_size = bodySize;
}


/*
 * the::system::net::simple_message::simple_message
 */
the::system::net::simple_message::simple_message(const message_id_type id,
        const void *body, const body_size_type bodySize) : base() {
    THE_STACK_TRACE;
    // Force the superclass to allocate memory for the header regardless of the
    // 'body' provided by the user.
    base::assert_storage(bodySize);

    // Set the message ID.
    this->header()->id = id;

    // Set the body. This will update the body, size, too.
    this->set_body(body, bodySize);
    THE_ASSERT(this->body_size() == bodySize);
}


/*
 * the::system::net::simple_message::~simple_message
 */
the::system::net::simple_message::~simple_message(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::simple_message::operator =
 */
the::system::net::simple_message& the::system::net::simple_message::operator =(
        simple_message&& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        this->storage = std::move(rhs.storage);
        rhs.storage = std::move(the::blob(this->header_size()));
        the::zero_memory(this->get_storage(), this->header_size());
    }
    return *this;
}


/*
 * the::system::net::simple_message::assert_storage
 */
bool the::system::net::simple_message::assert_storage(void *& outStorage,
        const size_t size) {
    THE_STACK_TRACE;
    bool retval = this->storage.assert_size(size, false);
    outStorage = static_cast<void *>(this->storage);
    return retval;
}


/*
 * the::system::net::simple_message::get_storage
 */
const void *the::system::net::simple_message::get_storage(void) const throw() {
    THE_STACK_TRACE;
    return static_cast<const void *>(this->storage);
}


/*
 * the::system::net::simple_message::get_storage
 */
void *the::system::net::simple_message::get_storage(void) throw() {
    THE_STACK_TRACE;
    return static_cast<void *>(this->storage);
}
