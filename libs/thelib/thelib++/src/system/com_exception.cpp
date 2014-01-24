/*
 * src\system\com_exception.cpp
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
 * COMException.cpp
 *
 * Copyright (C) 2006 - 2012 by Visualisierungsinstitut Universitaet Stuttgart.
 * Alle Rechte vorbehalten.
 */

#include "the/system/com_exception.h"

#include "the/stack_trace.h"
#include "the/text/string_builder.h"
#include "the/text/string_converter.h"


/*
 * the::system::com_exception::get_message
 */
the::astring& the::system::com_exception::get_message(astring& outMsg,
        native_error_type hr) {
    THE_STACK_TRACE;

#ifdef THE_WINDOWS
    _com_error ce(hr);
    outMsg = THE_T2A(ce.ErrorMessage());

#else /* THE_WINDOWS */
    text::astring_builder sb;
    sb.format("%d", hr);
    return sb.to_string(outMsg);
#endif /* THE_WINDOWS */
    return outMsg;
}


/*
 * the::system::com_exception::get_message
 */
the::wstring& the::system::com_exception::get_message(wstring& outMsg,
        native_error_type hr) {
    THE_STACK_TRACE;

#ifdef THE_WINDOWS
    _com_error ce(hr);
    outMsg = THE_T2W(ce.ErrorMessage());

#else /* THE_WINDOWS */
    text::wstring_builder sb;
    sb.format(L"%d", hr);
    return sb.to_string(outMsg);
#endif /* THE_WINDOWS */
    return outMsg;
}


/*
 * the::system::com_exception::com_exception
 */
the::system::com_exception::com_exception(const native_error_type hr,
        const char *file, const int line) : base(file, line), hr(hr) {
    THE_STACK_TRACE;
    wstring msg;
    this->set_msg(com_exception::get_message(msg, hr).c_str());
}


/*
 * the::system::com_exception::com_exception
 */
the::system::com_exception::com_exception(const native_error_type hr,
        const char *msg, const char *file, const int line) 
        : base(msg, file, line), hr(hr) {
    THE_STACK_TRACE;
}


/*
 * the::system::com_exception::com_exception
 */
the::system::com_exception::com_exception(const native_error_type hr,
        const wchar_t *msg, const char *file, const int line)
        : base(msg, file, line), hr(hr) {
    THE_STACK_TRACE;
}


/*
 * the::system::com_exception::com_exception
 */
the::system::com_exception::com_exception(const com_exception& rhs) 
        : base(rhs), hr(rhs.hr) {
    THE_STACK_TRACE;
}


/*
 * the::system::com_exception::~com_exception
 */
the::system::com_exception::~com_exception(void) throw() {
    THE_STACK_TRACE;
}


/*
 * the::system::com_exception::operator =
 */
the::system::com_exception& the::system::com_exception::operator =(
        const com_exception& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        base::operator =(rhs);
        this->hr = rhs.hr;
    }
    return *this;
}
