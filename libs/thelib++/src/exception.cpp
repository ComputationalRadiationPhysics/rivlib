/*
 * the/exception.cpp
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
 * Exception.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). Alle Rechte vorbehalten.
 */

#include "the/exception.h"

#include <cstring>
#include <cstdio>
#include <cstdarg>

#include "the/config.h"
#include "the/memory.h"
#include "the/stack_trace.h"
#include "the/string.h"
#include "the/text/string_converter.h"


/*
 * the::exception::exception
 */
the::exception::exception(const char *msg, const char *file, 
        const int line)
        : file(NULL), line(line), msg(NULL), stack(NULL) {
    THE_STACK_TRACE;
    this->set_file(file);
    this->set_msg(msg);
    this->fetch_stack();
}


/*
 * the::exception::exception
 */
the::exception::exception(const wchar_t *msg, const char *file, 
        const int line)
        : file(NULL), line(line), msg(NULL), stack(NULL) {
    THE_STACK_TRACE;
    this->set_file(file);
    this->set_msg(msg);
    this->fetch_stack();
}


/*
 * the::exception::exception
 */
the::exception::exception(const char *file, const int line) 
        : file(NULL), line(line), msg(NULL), stack(NULL) {
    THE_STACK_TRACE;
    this->set_file(file);
    this->fetch_stack();
}


/*
 * the::exception::exception
 */
the::exception::exception(const exception& rhs) 
        : file(NULL), line(rhs.line), msg(NULL), stack(NULL) {
    THE_STACK_TRACE;
    this->set_file(rhs.file);

    if (rhs.isMsgUnicode) {
        this->set_msg(static_cast<const wchar_t *>(rhs.get_msg_wstr()));
    } else {
        this->set_msg(static_cast<const char *>(rhs.get_msg_astr()));
    }

    if (rhs.stack != NULL) {
        size_t len = ::strlen(rhs.stack) + 1;
        this->stack = new char[len];
        ::memcpy(this->stack, rhs.stack, len);
    }
}


/* 
 * the::exception::~exception
 */
the::exception::~exception(void) throw() {
    THE_STACK_TRACE;
    safe_array_delete(this->file);
    safe_operator_delete(this->msg);
    safe_array_delete(this->stack);
}


/*
 * the::exception::get_msg_astr
 */
const char *the::exception::get_msg_astr(void) const {
    THE_STACK_TRACE;

    if (this->msg == NULL) {
        return "Exception";
    }

    if (this->isMsgUnicode) {
        try {
            the::astring dst;
            the::text::string_converter::convert(dst,
                static_cast<wchar_t *>(this->msg));
            this->set_msg(dst.c_str());
        } catch(...) {
            this->set_msg("[INTERNAL ERROR]");
        }
    }

    return static_cast<const char *>(this->msg);
}


/*
 * the::exception::get_msg_wstr
 */
const wchar_t *the::exception::get_msg_wstr(void) const {
    THE_STACK_TRACE;

    if (this->msg == NULL) {
        return L"Exception";
    }

    if (!this->isMsgUnicode) {
        try {
            the::wstring dst;
            the::text::string_converter::convert(dst,
                static_cast<char *>(this->msg));
            this->set_msg(dst.c_str());
        } catch(...) {
            this->set_msg(L"[INTERNAL ERROR]");
        }
    }

    return static_cast<const wchar_t *>(this->msg);
}


/*
 * the::exception::operator =
 */
the::exception& the::exception::operator =(const exception& rhs) {
    THE_STACK_TRACE;

    if (this != &rhs) {
        this->set_file(rhs.file);
        this->line = rhs.line;

        // using GetMsgX() because derived classes may use lazy initialisation
        if (rhs.isMsgUnicode) {
            this->set_msg(rhs.get_msg_wstr());
        } else {
            this->set_msg(rhs.get_msg_astr());
        }

        safe_array_delete(this->stack);
        if (rhs.stack != NULL) {
            size_t len = ::strlen(rhs.stack) + 1;
            this->stack = new char[len];
            ::memcpy(this->stack, rhs.stack, len);
        }
    }

    return *this;
}


/*
 * the::exception::fetch_stack
 */
void the::exception::fetch_stack(void) {
    THE_STACK_TRACE;

    unsigned int size;
    size = 0;
    the::stack_trace::get_stack_string((char*)NULL, size);
    if (size <= 1) {
        safe_array_delete(this->stack);
        this->stack = NULL;
        return;
    }

    this->stack = new char[size];
    the::stack_trace::get_stack_string(this->stack, size);
}


/*
 * the::exception::format_msg
 */
void the::exception::format_msg(const char *fmt, ...) {
    THE_STACK_TRACE;
    const float bufGrowFactor = 1.5f;

    va_list arglist;

    if (fmt != NULL) {
        this->isMsgUnicode = false;
        int bufLen = static_cast<int>(::strlen(fmt) + 1);

        va_start(arglist, fmt);
        do {
            va_end(arglist);
            va_start(arglist, fmt);
            safe_operator_delete(this->msg);
            bufLen = static_cast<int>(bufGrowFactor * bufLen);
            this->msg = ::operator new(bufLen * sizeof(char));
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
        } while (::_vsnprintf_s(static_cast<char *>(this->msg), bufLen, 
                _TRUNCATE, fmt, arglist) < 0);
#elif THE_WINDOWS 
        } while (::_vsnprintf(static_cast<char *>(this->msg), bufLen,
                fmt, arglist) < 0);
#else /* defined(_MSC_VER) && (_MSC_VER >= 1400) */
        } while (::vsnprintf(static_cast<char *>(this->msg), bufLen, 
                fmt, arglist) < 0);
#endif /* defined(_MSC_VER) && (_MSC_VER >= 1400) */
        va_end(arglist);

    } else {
        /* Delete old message and set pointer NULL. */
        safe_operator_delete(this->msg);
    }
}


/*
 * the::exception::format_msg
 */
void the::exception::format_msg(const wchar_t *fmt, ...) {
    THE_STACK_TRACE;
    const float bufGrowFactor = 1.5f;

    va_list arglist;

    if (fmt != NULL) {
        this->isMsgUnicode = true;
        int bufLen = static_cast<int>(::wcslen(fmt) + 1);

        va_start(arglist, fmt);
        do {
            va_end(arglist);
            va_start(arglist, fmt);
            safe_operator_delete(this->msg);
            bufLen = static_cast<int>(bufGrowFactor * bufLen);
            this->msg = ::operator new(bufLen * sizeof(wchar_t));
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
        } while (::_vsnwprintf_s(static_cast<wchar_t *>(this->msg), bufLen, 
                _TRUNCATE, fmt, arglist) < 0);
#elif THE_WINDOWS 
        } while (::_vsnwprintf(static_cast<wchar_t *>(this->msg), bufLen,
                fmt, arglist) < 0);
#else /* defined(_MSC_VER) && (_MSC_VER >= 1400) */
        } while (::vswprintf(static_cast<wchar_t *>(this->msg), bufLen, 
                fmt, arglist) < 0);
#endif /* defined(_MSC_VER) && (_MSC_VER >= 1400) */
        va_end(arglist);

    } else {
        /* Delete old message and set pointer NULL. */
        safe_operator_delete(this->msg);
    }
}


/*
 * the::exception::set_file
 */
void the::exception::set_file(const char *file) {
    THE_STACK_TRACE;
    safe_array_delete(this->file);

    if (file != NULL) {
        size_t bufLen = ::strlen(file) + 1;
        this->file = new char[bufLen];
        ::memcpy(this->file, file, bufLen * sizeof(char));
    }
}


/*
 * the::exception::set_msg
 */
void the::exception::set_msg(const char *msg) const {
    THE_STACK_TRACE;
    safe_operator_delete(this->msg);

    if (msg != NULL) {
        this->isMsgUnicode = false;
        size_t bufLen = ::strlen(msg) + 1;
        this->msg = ::operator new(bufLen * sizeof(char));
        ::memcpy(this->msg, msg, bufLen * sizeof(char));
    }
}


/*
 * the::exception::set_msg
 */
void the::exception::set_msg(const wchar_t *msg) const {
    THE_STACK_TRACE;
    safe_operator_delete(this->msg);

    if (msg != NULL) {
        this->isMsgUnicode = true;
        size_t bufLen = ::wcslen(msg) + 1;
        this->msg = ::operator new(bufLen * sizeof(wchar_t));
        ::memcpy(this->msg, msg, bufLen * sizeof(wchar_t));
    }
}
