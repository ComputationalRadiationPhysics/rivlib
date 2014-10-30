/*
 * the/system/system_message.cpp
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
 * SystemMessage.cpp  15.08.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#include "the/system/system_message.h"

#ifndef THE_WINDOWS
#include <wchar.h>
#endif /* THE_WINDOWS */

#include "the/memory.h"
#include "the/stack_trace.h"
#include "the/system/system_exception.h"


/*
 * the::system::system_message::system_message
 */
the::system::system_message::system_message(const native_error_type error)
        : error(error), msg(NULL) {
    THE_STACK_TRACE;
}


/*
 * the::system::system_message::system_message
 */
the::system::system_message::system_message(const system_error& error)
        : error(error), msg(NULL) {
    THE_STACK_TRACE;
}

/*
 * the::system::system_message::system_message
 */
the::system::system_message::system_message(const system_message& rhs)
        : error(rhs.error), msg(NULL) {
    THE_STACK_TRACE;
}


/*
 * the::system::system_message::~system_message
 */
the::system::system_message::~system_message(void) {
    THE_STACK_TRACE;

#ifdef THE_WINDOWS
    safe_local_free(this->msg);
#else /* THE_WINDOWS */
    safe_array_delete(this->msg);
#endif /* THE_WINDOWS */
}


/*
 * the::system::system_message::operator =
 */
the::system::system_message& the::system::system_message::operator =(
        const system_message& rhs) {
    THE_STACK_TRACE;

    if (this != &rhs) {
        this->error = rhs.error;
#ifdef THE_WINDOWS
        safe_local_free(this->msg);
#else /* THE_WINDOWS */
        safe_array_delete(this->msg);
#endif /* THE_WINDOWS */
    }

    return *this;
}


/*
 * the::system::system_message::operator const system_char *
 */
the::system::system_message::operator const the::system::system_char *(
        void) const {
    THE_STACK_TRACE;
    this->assert_msg();
    return this->msg;
}


/*
 * the::system::system_message::operator astring
 */
the::system::system_message::operator the::astring(void) const {
    THE_STACK_TRACE;
    return the::astring(THE_S2A(static_cast<const system_char *>(*this)));
}


/*
 * the::system::system_message::operator wstring
 */
the::system::system_message::operator the::wstring(void) const {
    THE_STACK_TRACE;
    return the::wstring(THE_S2W(static_cast<const system_char *>(*this)));
}


/*
 * the::system::system_message::assert_msg
 */
void the::system::system_message::assert_msg(void) const {
    THE_STACK_TRACE;

    if (this->msg == NULL) {
#if defined(THE_WINDOWS)
        if (::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER 
                | FORMAT_MESSAGE_FROM_SYSTEM, NULL, this->error.native_error(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                reinterpret_cast<wchar_t *>(&this->msg), 0, NULL) == 0) {
            throw system_exception(__FILE__, __LINE__);
        }

#elif defined(THE_LINUX)
        size_t bufLen = 128;
        char *buf = new char[bufLen];
        char *msg = NULL;

#ifdef _GNU_SOURCE
        msg = ::strerror_r(this->error.native_error(), buf, bufLen);

        // Ensure end of string in case 'buf' was used and too short.
        buf[bufLen - 1] = 0;

#else /* _GNU_SOURCE */
        while (::strerror_r(this->error.native_error(), buf, bufLen) == -1) {
            if (::GetLastError() != ERANGE) {
                buf[0] = 0;
                break;
            }
            delete[] buf;
            bufLen += bufLen / 2;
            buf = new char[bufLen];
        }     
        msg = buf;
#endif /* _GNU_SOURCE */

        bufLen = ::strlen(msg) + 1;
        this->msg = new char[bufLen];
        ::memcpy(this->msg, msg, bufLen * sizeof(char));

        if (msg == buf) {
            // Assume that we only have to free memory that we have
            // allocated ourselves, but only our own buffer.
            // Manpages do not contain sufficient information for doing
            // something more intelligent.
            safe_array_delete(buf);
        }

#else /* defined(THE_WINDOWS) */
#error "the::system::system_message::assert_msg must be implemented!"
#endif /* defined(THE_WINDOWS) */
    } /* end if (this->msg == NULL) */
}
