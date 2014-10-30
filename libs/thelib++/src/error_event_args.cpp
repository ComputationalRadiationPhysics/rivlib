/*
 * src\error_event_args.cpp
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

#include "the/error_event_args.h"



/*
 * the::error_event_args::error_event_args
 */
the::error_event_args::error_event_args(void) : base() {
    THE_STACK_TRACE;
}


/*
 * the::error_event_args::error_event_args
 */
the::error_event_args::error_event_args(const std::exception_ptr& exPtr)
        : base(), exPtr(exPtr) {
    THE_STACK_TRACE;
}


/*
 * the::error_event_args::error_event_args
 */
the::error_event_args::error_event_args(const error_event_args& rhs)
        : base(rhs), exPtr(rhs.exPtr) {
    THE_STACK_TRACE;
}


/*
 * the::error_event_args::~error_event_args
 */
the::error_event_args::~error_event_args(void) {
    THE_STACK_TRACE;
}


/*
 * the::error_event_args::is_error
 */
bool the::error_event_args::is_error(void) const {
    THE_STACK_TRACE;
    return (this->exPtr != error_event_args::EMPTY_EXCEPTION);
}


/*
 * the::error_event_args::rethrow
 */
void the::error_event_args::rethrow(void) const {
    THE_STACK_TRACE;
    if (this->is_error()) {
        std::rethrow_exception(this->exPtr);
    }
}


/*
 * the::error_event_args::operator =
 */
the::error_event_args& the::error_event_args::operator =(
        const error_event_args& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        base::operator =(rhs);
        this->exPtr = rhs.exPtr;
    }
    return *this;
}


/*
 * the::error_event_args::EMPTY_EXCEPTION
 */
const std::exception_ptr the::error_event_args::EMPTY_EXCEPTION;
