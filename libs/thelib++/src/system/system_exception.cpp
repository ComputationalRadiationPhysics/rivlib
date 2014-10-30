/*
 * the/system/system_exception.cpp
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
 * SystemException.cpp  09.08.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#include "the/system/system_exception.h"

#include "the/stack_trace.h"


/*
 * the::system::system_exception::system_exception
 */
the::system::system_exception::system_exception(
        const native_error_type errorCode, const char *file, const int line)
    : base(static_cast<const char *>(NULL), file, line), sysMsg(errorCode) {
    THE_STACK_TRACE;
}


/*
 * the::system::system_exception::system_exception
 */
the::system::system_exception::system_exception(const system_error& errorCode,
        const char *file, const int line)
    : base(static_cast<const char *>(NULL), file, line), sysMsg(errorCode) {
    THE_STACK_TRACE;
}


/*
 * the::system::system_exception::system_exception
 */
the::system::system_exception::system_exception(const char *file, const int line)
        : base(static_cast<const char *>(NULL), file, line),
        sysMsg(system_error::last()) {
    THE_STACK_TRACE;
}


/*
 * the::system::system_exception::system_exception
 */
the::system::system_exception::system_exception(const system_exception& rhs) 
        : base(rhs), sysMsg(rhs.sysMsg) {
    THE_STACK_TRACE;
}


/*
 * the::system::system_exception::~system_exception
 */
the::system::system_exception::~system_exception(void) throw() {
    THE_STACK_TRACE;
}


/*
 * the::sys::SystemException::GetMsgA
 */
const char *the::system::system_exception::get_msg_astr(void) const {
    THE_STACK_TRACE;
    this->set_msg(THE_S2A(static_cast<const system_char *>(this->sysMsg)));
    return base::get_msg_astr();
}


/*
 * the::sys::SystemException::GetMsgW
 */
const wchar_t *the::system::system_exception::get_msg_wstr(void) const {
    THE_STACK_TRACE;
    this->set_msg(THE_S2W(static_cast<const system_char *>(this->sysMsg)));
    return base::get_msg_wstr();
}


/*
 * the::system::system_exception::operator =
 */
the::system::system_exception& the::system::system_exception::operator =(
        const system_exception& rhs) {
    THE_STACK_TRACE;

    if (this != &rhs) {
        base::operator =(rhs);
        this->sysMsg = rhs.sysMsg;
    }

    return *this;
}
