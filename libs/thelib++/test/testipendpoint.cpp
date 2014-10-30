/*
 * testipendpoint.cpp
 *
 * Copyright (c) 2013, TheLib Team (http://www.thelib.org/license)
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

#include "stdafx.h"
#include "testhelper.h"

#include "the/string.h"

#include "the/system/net/ip_end_point.h"
#include "the/system/net/socket.h"

#include "the/text/string_converter.h"
#include "the/text/string_utility.h"


template<class T> void TestEpParsing(void) {
    using namespace the::system::net;
    ip_end_point endPoint;
    const T *input = nullptr;

    input = "127.0.0.1:1234";
    endPoint = ip_end_point::parse(input);
    ::AssertTrue("Parsed IPv4 loopback", endPoint.ip_address()->is_loopback());
    ::AssertEqual("Parsed port 1234", endPoint.port(), ip_end_point::port_type(1234));

    input = "[::1]:1234";
    endPoint = ip_end_point::parse(input);
    ::AssertTrue("Parsed IPv6 loopback", endPoint.ip_address()->is_loopback());
    ::AssertEqual("Parsed port 1234", endPoint.port(), ip_end_point::port_type(1234));

    input = "[::1]";
    AssertException("Missing port", ip_end_point::parse(input), the::format_exception);

    input = "[::1]:";
    AssertException("Empty port", ip_end_point::parse(input), the::format_exception);

    input = "::1:1234";
    AssertException("Invalid IPv6", ip_end_point::parse(input), the::format_exception);
}

/*
 * ::TestIpEndPoint
 */
void TestIpEndPoint(void) {
    the::system::net::socket::startup();
    ::TestEpParsing<char>();
    //::TestEpParsing<wchar_t>();
    the::system::net::socket::cleanup();
}
