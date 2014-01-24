/*
 * the/system/net/socket_exception.cpp
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
/*
 * SocketException.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#include "the/system/net/socket_exception.h"



/*
 * the::system::net::socket_exception::error_timeout
 */
const the::system::net::socket_exception::native_error_type
the::system::net::socket_exception::error_timeout
#ifdef _WIN32
    = WSAETIMEDOUT;
#else /* _WIN32 */
    = ETIMEDOUT;
#endif /* _WIN32 */


/*
 * the::system::net::socket_exception::socket_exception
 */
the::system::net::socket_exception::socket_exception(
        const native_error_type errorCode, const char *file, const int line)
        : base(errorCode, file, line) { 
    THE_STACK_TRACE;
}


/*
 * the::system::net::socket_exception::socket_exception
 */
the::system::net::socket_exception::socket_exception(
        const native_error_type errorCode, const char *msg, const char *file, 
        const int line) : base(errorCode, file, line) {
    THE_STACK_TRACE;
    if (msg != NULL) {
        this->set_msg(msg);
    }
}


/*
 * the::system::net::socket_exception::socket_exception
 */
the::system::net::socket_exception::socket_exception(
        const native_error_type errorCode, const wchar_t *msg, const char *file,
        const int line) : base(errorCode, file, line) {
    THE_STACK_TRACE;
    if (msg != NULL) {
        this->set_msg(msg);
    }
}


/*
 * the::system::net::socket_exception::socket_exception
 */
the::system::net::socket_exception::socket_exception(const char *file, 
        const int line)
#if defined(THE_WINDOWS)
        : base(::WSAGetLastError(), file, line) {
#elif defined(THE_LINUX)
        : base(errno, file, line) {
#else /* defined(THE_WINDOWS) */
#error "Implementation missing for this platform."
#endif /* defined(THE_WINDOWS) */
    THE_STACK_TRACE;
}


/*
 * the::system::net::socket_exception::socket_exception
 */
the::system::net::socket_exception::socket_exception(
        const socket_exception& rhs) : base(rhs) { 
    THE_STACK_TRACE;
}


/*
 * the::system::net::socket_exception::~socket_exception
 */
the::system::net::socket_exception::~socket_exception(void) throw() {
    THE_STACK_TRACE;
}


/*
 * the::system::net::socket_exception::operator =
 */
the::system::net::socket_exception&
the::system::net::socket_exception::operator =(const socket_exception& rhs) {
    THE_STACK_TRACE;
    base::operator =(rhs);
    return *this;
}

