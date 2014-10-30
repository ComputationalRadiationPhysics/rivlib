/*
 * src\system\net\shallow_simple_message.cpp
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

#include "the/system/net/shallow_simple_message.h"

#include "the/argument_exception.h"
#include "the/argument_null_exception.h"
#include "the/invalid_operation_exception.h"
#include "the/stack_trace.h"



/*
 * the::system::net::shallow_simple_message::~shallow_simple_message
 */
the::system::net::shallow_simple_message::~shallow_simple_message(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::shallow_simple_message::set_storage
 */
void the::system::net::shallow_simple_message::set_storage(void *storage,
        const size_t cntStorage) {
    THE_STACK_TRACE;
    if (storage == nullptr) {
        THROW_THE_EXCEPTION(argument_null_exception, "storage");
    }
    if ((cntStorage != 0) && (cntStorage < this->header_size())) {
        THROW_THE_EXCEPTION(argument_exception, "cntStorage");
    }

    // Note: Order of assignments is important!
    this->storage = storage;
    this->cntStorage = (cntStorage > 0) ? cntStorage : this->size();
}


/*
 * the::system::net::shallow_simple_message::assert_storage
 */
bool the::system::net::shallow_simple_message::assert_storage(
        void *& outStorage, const size_t size) {
    THE_STACK_TRACE;
    if (size > this->cntStorage) {
        THROW_THE_INVALID_OPERATION_EXCEPTION;
    }
    return false;
}


/*
 * the::system::net::shallow_simple_message::get_storage
 */
const void *the::system::net::shallow_simple_message::get_storage(
        void) const throw() {
    THE_STACK_TRACE;
    return this->storage;
}


/*
 * the::system::net::shallow_simple_message::get_storage
 */
void *the::system::net::shallow_simple_message::get_storage(void) throw() {
    THE_STACK_TRACE;
    return this->storage;
}
