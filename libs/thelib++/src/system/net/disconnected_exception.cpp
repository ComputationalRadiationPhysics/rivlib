/*
 * src\system\net\disconnected_exception.cpp
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

#include "the/system/net/disconnected_exception.h"

#include "the/not_implemented_exception.h"
#include "the/stack_trace.h"


/*
 * the::system::net::disconnected_exception::disconnected_exception
 */
the::system::net::disconnected_exception::disconnected_exception(
        const char *file, const int line) : base(file, line) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::disconnected_exception::disconnected_exception
 */
the::system::net::disconnected_exception::disconnected_exception(
        const disconnected_exception& rhs) : base(rhs) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::disconnected_exception::~disconnected_exception
 */
the::system::net::disconnected_exception::~disconnected_exception(
        void) throw() {
    THE_STACK_TRACE;
}


/*
 * the::system::net::disconnected_exception::operator =
 */
the::system::net::disconnected_exception&
the::system::net::disconnected_exception::operator =(
        const disconnected_exception& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        base::operator =(rhs);
    }
    return *this;
}
