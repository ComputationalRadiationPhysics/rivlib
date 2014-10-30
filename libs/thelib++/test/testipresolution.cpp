/*
 * testipresolution.cpp
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

#include "the/system/net/dns.h"
#include "the/system/net/socket.h"

#include "the/text/string_converter.h"
#include "the/text/string_utility.h"


template<class T> void TestIpResolutionImpl(void) {
    using namespace the::system::net;
    using the::text::string_converter;
    using the::text::string_utility;

    ip_address::shared_pointer_type ipPtr;
    ip_host_entry<std::basic_string<T> > hostEntry;
    std::basic_string<T> addrStr;

    string_converter::convert(addrStr, "127.0.0.1");
    ipPtr = ip_address::parse(addrStr);
    ::AssertTrue("Parsing IPv4 loopback address using ip_address", ipPtr->is_loopback());

    string_converter::convert(addrStr, "10.0.0.1");
    ipPtr = ip_address::parse(addrStr);
    ::AssertEqual("Parsing \"10.0.0.1\", byte 0", (*ipPtr)[0], byte(10));
    ::AssertEqual("Parsing \"10.0.0.1\", byte 1", (*ipPtr)[1], byte(0));
    ::AssertEqual("Parsing \"10.0.0.1\", byte 2", (*ipPtr)[2], byte(0));
    ::AssertEqual("Parsing \"10.0.0.1\", byte 3", (*ipPtr)[3], byte(1));

    string_converter::convert(addrStr, "[::1]");
    ipPtr = ip_address::parse(addrStr);
    ::AssertTrue("Parsing IPv6 loopback address using ip_address", ipPtr->is_loopback());

    string_converter::convert(addrStr, "::1");
    ipPtr = ip_address::parse(addrStr);
    ::AssertTrue("Parsing IPv6 loopback address using ip_address", ipPtr->is_loopback());

    string_converter::convert(addrStr, "fe80::b19a:1bc0:4634:bed8");
    ipPtr = ip_address::parse(addrStr);
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 0", (*ipPtr)[0], byte(0xfe));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 1", (*ipPtr)[1], byte(0x80));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 2", (*ipPtr)[2], byte(0x0));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 3", (*ipPtr)[3], byte(0x0));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 4", (*ipPtr)[4], byte(0x0));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 5", (*ipPtr)[5], byte(0x0));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 6", (*ipPtr)[6], byte(0x0));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 7", (*ipPtr)[7], byte(0x0));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 8", (*ipPtr)[8], byte(0xb1));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 9", (*ipPtr)[9], byte(0x9a));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 10", (*ipPtr)[10], byte(0x1b));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 11", (*ipPtr)[11], byte(0xc0));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 12", (*ipPtr)[12], byte(0x46));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 13", (*ipPtr)[13], byte(0x34));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 14", (*ipPtr)[14], byte(0xbe));
    ::AssertEqual("Parsing \"fe80::b19a:1bc0:4634:bed8%20\", byte 15", (*ipPtr)[15], byte(0xd8));

    string_converter::convert(addrStr, "bing.com");
    AssertNoException("Can resolve bing.com", dns::get_host_entry(hostEntry, addrStr));
    ::AssertTrue("Correct name resolved.", string_utility::contains(hostEntry.get_host_name(), addrStr, 0, false));
    ::AssertTrue("At least one address.", hostEntry.get_addresses().size() >= 1);

    string_converter::convert(addrStr, "bing.com");
    AssertNoException("Can resolve bing.com", dns::get_host_entry(hostEntry, addrStr));
    ::AssertTrue("Correct name resolved.", string_utility::contains(hostEntry.get_host_name(), addrStr, 0, false));
    ::AssertTrue("At least one address.", hostEntry.get_addresses().size() >= 1);
}


/*
 * ::TestIpResolution
 */
void TestIpResolution(void) {
    the::system::net::socket::startup();
    ::TestIpResolutionImpl<char>();
    ::TestIpResolutionImpl<wchar_t>();
    the::system::net::socket::cleanup();
}
