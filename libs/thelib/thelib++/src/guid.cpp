/*
 * src\guid.cpp
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
/*
 * GUID.cpp
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#include "the/guid.h"

#include "the/argument_null_exception.h"
#include "the/assert.h"
#include "the/memory.h"
#include "the/invalid_operation_exception.h"
#include "the/not_implemented_exception.h"
#include "the/stack_trace.h"

#include "the/text/string_converter.h"


/*
 * the::guid::create
 */
the::guid& the::guid::create(guid& outGuid, const bool doNotUseMac) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    RPC_STATUS status;

    if (doNotUseMac) {
        status = ::UuidCreate(&outGuid.id);
    } else {
        status = ::UuidCreateSequential(&outGuid.id);
    }

    if ((status != RPC_S_OK) && (status != RPC_S_UUID_LOCAL_ONLY)) {
        THROW_THE_INVALID_OPERATION_EXCEPTION;
    }

#elif defined(THE_LINUX)
    if (doNotUseMac) {
        ::uuid_generate_random(outGuid.id);
    } else {
        ::uuid_generate_time(outGuid.id);
    }

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */

    return outGuid;
}


/*
 * the::guid::parse
 */
the::guid& the::guid::parse(guid& outGuid, const char *str) {
    THE_STACK_TRACE;
    if (str == nullptr) {
        THROW_THE_EXCEPTION(argument_null_exception, "str");
    }

#if defined(THE_WINDOWS)
    if (::UuidFromStringA(reinterpret_cast<RPC_CSTR>(const_cast<char *>(
            str)), &outGuid.id) != RPC_S_OK) {
#elif defined(THE_LINUX)
    if (::uuid_parse(str, outGuid.id) == -1) {
#else /* defined(THE_WINDOWS) */
#error "the::guid::parse() must be implemented."
#endif /* defined(THE_WINDOWS) */
        THROW_THE_EXCEPTION(format_exception, "The given string does not "
            "represent a valid GUID.");
    }

    return outGuid;
}


/*
 * the::guid::parse
 */
the::guid& the::guid::parse(guid& outGuid, const wchar_t *str) {
    THE_STACK_TRACE;
    if (str == nullptr) {
        THROW_THE_EXCEPTION(argument_null_exception, "str");
    }

#if defined(THE_WINDOWS)
    if (::UuidFromStringW(reinterpret_cast<RPC_WSTR>(const_cast<wchar_t *>(
            str)), &outGuid.id) != RPC_S_OK) {
#elif defined(THE_LINUX)
    if (::uuid_parse(THE_W2A(str), outGuid.id) == -1) {
#else /* defined(THE_WINDOWS) */
#error "the::guid::parse() must be implemented."
#endif /* defined(THE_WINDOWS) */
        THROW_THE_EXCEPTION(format_exception, "The given string does not "
            "represent a valid GUID.");
    }

    return outGuid;
}


/*
 * the::guid::guid
 */
the::guid::guid(void) {
    THE_STACK_TRACE;
    this->clear();
}


/*
 * the::guid::guid
 */
the::guid::guid(const byte b[16]) {
    THE_STACK_TRACE;
    THE_ASSERT(sizeof(this->id) == 16 * sizeof(byte));

    if (b == nullptr) {
        THROW_THE_EXCEPTION(argument_null_exception, "b");
    }

    ::memcpy(guid::data(this->id), b, sizeof(this->id));
}


/*
 * the::guid::guid
 */
the::guid::guid(const byte b1, const byte b2, const byte b3, const byte b4,
        const byte b5, const byte b6, const byte b7, const byte b8,
        const byte b9, const byte b10, const byte b11, const byte b12,
        const byte b13, const byte b14, const byte b15, const byte b16) {
    THE_STACK_TRACE;
    byte *g = guid::data(this->id);
#define ASSIGN_BYTE(i) g[i - 1] = b##i
    ASSIGN_BYTE(1);
    ASSIGN_BYTE(2);
    ASSIGN_BYTE(3);
    ASSIGN_BYTE(4);
    ASSIGN_BYTE(5);
    ASSIGN_BYTE(6);
    ASSIGN_BYTE(7);
    ASSIGN_BYTE(8);
    ASSIGN_BYTE(9);
    ASSIGN_BYTE(10);
    ASSIGN_BYTE(11);
    ASSIGN_BYTE(12);
    ASSIGN_BYTE(13);
    ASSIGN_BYTE(14);
    ASSIGN_BYTE(15);
    ASSIGN_BYTE(16);
#undef ASSIGN_BYTE
}


/*
 * the::guid::guid
 */
the::guid::guid(const uint32_t i, const uint16_t s1, const uint16_t s2,
        const byte b1, const byte b2, const byte b3, const byte b4,
        const byte b5, const byte b6, const byte b7, const byte b8) {
    THE_STACK_TRACE;
    byte *g = guid::data(this->id);
    const byte *in = nullptr;

    in = reinterpret_cast<const byte *>(&i);
    g[0] = in[0];
    g[1] = in[1];
    g[2] = in[2];
    g[3] = in[3];

    in = reinterpret_cast<const byte *>(&s1);
    g[4] = in[0];
    g[5] = in[1];

    in = reinterpret_cast<const byte *>(&s2);
    g[6] = in[0];
    g[7] = in[1];

    g[8] = b1;
    g[9] = b2;
    g[10] = b3;
    g[11] = b4;
    g[12] = b5;
    g[13] = b6;
    g[14] = b7;
    g[15] = b8;
}




/*
 * the::guid::guid
 */
the::guid::guid(const uint32_t i, const uint16_t s1, const uint16_t s2,
        const byte b[8]) {
    THE_STACK_TRACE;
    byte *g = guid::data(this->id);
    const byte *in = nullptr;

    if (b == nullptr) {
        THROW_THE_EXCEPTION(argument_null_exception, "b");
    }

    in = reinterpret_cast<const byte *>(&i);
    g[0] = in[0];
    g[1] = in[1];
    g[2] = in[2];
    g[3] = in[3];

    in = reinterpret_cast<const byte *>(&s1);
    g[4] = in[0];
    g[5] = in[1];

    in = reinterpret_cast<const byte *>(&s2);
    g[6] = in[0];
    g[7] = in[1];

    ::memcpy(g + 8, b, 8 * sizeof(byte));
}



/*
 * the::guid::~guid
 */
the::guid::~guid(void) {
    THE_STACK_TRACE;
}


/*
 * the::guid::clear
 */
void the::guid::clear(void) {
    THE_STACK_TRACE;
    the::zero_memory(guid::data(this->id));
}


/*
 * the::guid::compare
 */
int the::guid::compare(const guid& rhs) const {
    THE_STACK_TRACE;
    return ::memcmp(guid::data(this->id), guid::data(rhs.id), sizeof(this->id));
}


/*
 * the::guid::equals
 */
bool the::guid::equals(const guid& rhs) const {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    return (::InlineIsEqualGUID(this->id, rhs.id) != 0);
#elif defined(THE_LINUX)
    return (::uuid_compare(this->id, rhs.id) == 0);
#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::guid::hash_code
 */
the::guid::hash_type the::guid::hash_code(void) const {
    THE_STACK_TRACE;
    const byte *str = guid::data(this->id);
    hash_type hash = 0;

    // DJB2 hash function
    for (size_t i = 0; i < sizeof(this->id); ++i) {
        hash = ((hash << 5) + hash) + static_cast<hash_type>(str[i]);
    }

    return hash;
}


/*
 * the::guid::to_astring
 */
the::astring the::guid::to_astring(void) const {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    RPC_CSTR str;
    if (::UuidToStringA(const_cast<::GUID *>(&this->id), &str) == RPC_S_OK) {
        the::astring retval(reinterpret_cast<char *>(str));
        ::RpcStringFreeA(&str);
        return retval;
    } else {
        throw std::bad_alloc();
    }

#elif defined(THE_LINUX)
    char retval[36 + 1];
    ::uuid_unparse(this->id, retval);
    return retval;

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::guid::to_wstring
 */
the::wstring the::guid::to_wstring(void) const {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    RPC_WSTR str;
    if (::UuidToStringW(const_cast<::GUID *>(&this->id), &str) == RPC_S_OK) {
        the::wstring retval(reinterpret_cast<wchar_t *>(str));
        ::RpcStringFreeW(&str);
        return retval;
    } else {
        throw std::bad_alloc();
    }

#elif defined(THE_LINUX)
    the::wstring retval;
    return text::string_converter::convert(retval, this->to_astring());

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::guid::zero
 */
bool the::guid::zero(void) const {
    THE_STACK_TRACE;
    const byte *g = guid::data(this->id);

    for (size_t i = 0; i < sizeof(this->id); ++i) {
        if (g[i] != 0) {
            return false;
        }
    }

    return true;
}


/*
 * the::guid::operator =
 */
the::guid& the::guid::operator =(const guid& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        ::memcpy(guid::data(this->id), guid::data(rhs.id), sizeof(this->id));
    }
    return *this;
}


/*
 * the::guid::operator =
 */
the::guid& the::guid::operator =(const native_guid_type& rhs) {
    THE_STACK_TRACE;
    byte *g = guid::data(this->id);
    const byte *h = guid::data(rhs);
    if (g != h) {
        ::memcpy(g, h, sizeof(this->id));
    }
    return *this;
}


/*
 * the::guid::operator =
 */
the::guid& the::guid::operator =(const native_guid_type *rhs) {
    THE_STACK_TRACE;
    if (rhs == nullptr) {
        THROW_THE_EXCEPTION(argument_null_exception, "rhs");
    }

    byte *g = guid::data(this->id);
    const byte *h = guid::data(*rhs);
    if (g != h) {
        ::memcpy(g, h, sizeof(this->id));
    }
    return *this;
}

