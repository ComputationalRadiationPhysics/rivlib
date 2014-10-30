/*
 * testipaddress.cpp
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

#include "stdafx.h"
#include "testhelper.h"

#include "the/system/net/ip_address.h"
#include "the/system/net/ip_address4.h"
#include "the/system/net/ip_address6.h"
#include "the/system/net/socket.h"



template<class T> void CommonTestIpAddress(void) {
    typedef T ip_address_type;

    ip_address_type addr1;
    std::cout << addr1.to_astring().c_str() << std::endl;
    std::cout << addr1 << std::endl;
    ::AssertEqual("Default ctor creates any address", addr1, ip_address_type::any);

    addr1 = ip_address_type::loopback;
    std::cout << addr1.to_astring().c_str() << std::endl;
    std::cout << addr1 << std::endl;
    ::AssertEqual("Assignment operator", addr1, ip_address_type::loopback);

    ip_address_type addr2(ip_address_type::loopback);
    std::cout << addr2.to_astring().c_str() << std::endl;
    std::cout << addr2 << std::endl;
    ::AssertEqual("Copy ctor", addr2, ip_address_type::loopback);

    ::AssertTrue("ip_address_type::any.is_any()", ip_address_type::any.is_any());
    ::AssertTrue("ip_address_type::loopback.is_loopback()", ip_address_type::loopback.is_loopback());
}


void TestIpAddress4(void) {
    using the::system::net::address_family;
    using the::system::net::ip_address4;

    ::AssertEqual("ip_address4::loopback[0]", ip_address4::loopback[0], byte(127));
    ::AssertEqual("ip_address4::loopback[1]", ip_address4::loopback[1], byte(0));
    ::AssertEqual("ip_address4::loopback[2]", ip_address4::loopback[2], byte(0));
    ::AssertEqual("ip_address4::loopback[3]", ip_address4::loopback[3], byte(1));

    ip_address4 addr1(127, 0, 0, 1);
    ::AssertTrue("Four byte ctor", addr1.is_loopback());

    ip_address4 addr2(127, 0, 0, 0);
    ::AssertEqual("Four byte ctor [0]", addr2[0], byte(127));
    ::AssertEqual("Four byte ctor [1]", addr2[1], byte(0));
    ::AssertEqual("Four byte ctor [2]", addr2[2], byte(0));
    ::AssertEqual("Four byte ctor [3]", addr2[3], byte(0));

    ::AssertEqual("Size of ip_address4", addr1.size(), size_t(4));
    ::AssertEqual("Family of ip_address4", addr1.address_family(), address_family::inter_network4);
    ::AssertTrue("ip_address4::compare (gt)", addr1.compare(addr2) > 0);
    ::AssertTrue("ip_address4::compare (lt)", addr2.compare(addr1) < 0);
    ::AssertTrue("ip_address4::compare (eq)", addr1.compare(addr1) == 0);  

    ::AssertTrue("ip_address4::is_any (true)", ip_address4::any.is_any());
    ::AssertFalse("ip_address4::is_any (false)", addr1.is_any());

    ::AssertTrue("ip_address4::is_loopback (true)", ip_address4::loopback.is_loopback());
    ::AssertFalse("ip_address4::is_loopback (false)", addr2.is_loopback());

    ::AssertEqual("ip_address4::to_string", addr2.to_astring(), std::string("127.0.0.0"));

    ip_address4 addr3(129, 69, 205, 36);
    ::AssertEqual("Four byte ctor [0]", addr3[0], byte(129));
    ::AssertEqual("Four byte ctor [1]", addr3[1], byte(69));
    ::AssertEqual("Four byte ctor [2]", addr3[2], byte(205));
    ::AssertEqual("Four byte ctor [3]", addr3[3], byte(36));

    ip_address4 mask1(255, 255, 255, 0);
    ::AssertEqual("Four byte ctor [0]", mask1[0], byte(255));
    ::AssertEqual("Four byte ctor [1]", mask1[1], byte(255));
    ::AssertEqual("Four byte ctor [2]", mask1[2], byte(255));
    ::AssertEqual("Four byte ctor [3]", mask1[3], byte(0));

    addr3 &= mask1;
    ::AssertEqual("Apply netmask [0]", addr3[0], byte(129));
    ::AssertEqual("Apply netmask [1]", addr3[1], byte(69));
    ::AssertEqual("Apply netmask [2]", addr3[2], byte(205));
    ::AssertEqual("Apply netmask [3]", addr3[3], byte(0));

    ip_address4 mask2(255, 0, 255, 0);
    ::AssertEqual("Four byte ctor [0]", mask2[0], byte(255));
    ::AssertEqual("Four byte ctor [1]", mask2[1], byte(0));
    ::AssertEqual("Four byte ctor [2]", mask2[2], byte(255));
    ::AssertEqual("Four byte ctor [3]", mask2[3], byte(0));

    addr3 &= mask2;
    ::AssertEqual("Apply nonsense netmask [0]", addr3[0], byte(129));
    ::AssertEqual("Apply nonsense netmask [1]", addr3[1], byte(0));
    ::AssertEqual("Apply nonsense netmask [2]", addr3[2], byte(205));
    ::AssertEqual("Apply nonsense netmask [3]", addr3[3], byte(0));

    addr3 = ip_address4(129, 69, 205, 36);
    addr3 &= 16;
    ::AssertEqual("Apply prefix mask [0]", addr3[0], byte(129));
    ::AssertEqual("Apply prefix mask [1]", addr3[1], byte(69));
    ::AssertEqual("Apply prefix mask [2]", addr3[2], byte(0));
    ::AssertEqual("Apply prefix mask [3]", addr3[3], byte(0));    

    addr3 = ip_address4::from_dotted_string("129.69.205.36");
    ::AssertEqual("ip_address4::from_dotted_string [0]", addr3[0], byte(129));
    ::AssertEqual("ip_address4::from_dotted_string [1]", addr3[1], byte(69));
    ::AssertEqual("ip_address4::from_dotted_string [2]", addr3[2], byte(205));
    ::AssertEqual("ip_address4::from_dotted_string [3]", addr3[3], byte(36));

    addr3 = ip_address4::from_dotted_string(L"129.69.205.36");
    ::AssertEqual("ip_address4::from_dotted_string [0]", addr3[0], byte(129));
    ::AssertEqual("ip_address4::from_dotted_string [1]", addr3[1], byte(69));
    ::AssertEqual("ip_address4::from_dotted_string [2]", addr3[2], byte(205));
    ::AssertEqual("ip_address4::from_dotted_string [3]", addr3[3], byte(36));

    addr3 = ip_address4::from_dotted_string(the::astring("129.69.205.36"));
    ::AssertEqual("ip_address4::from_dotted_string [0]", addr3[0], byte(129));
    ::AssertEqual("ip_address4::from_dotted_string [1]", addr3[1], byte(69));
    ::AssertEqual("ip_address4::from_dotted_string [2]", addr3[2], byte(205));
    ::AssertEqual("ip_address4::from_dotted_string [3]", addr3[3], byte(36));

    addr3 = ip_address4::from_dotted_string(the::wstring(L"129.69.205.36"));
    ::AssertEqual("ip_address4::from_dotted_string [0]", addr3[0], byte(129));
    ::AssertEqual("ip_address4::from_dotted_string [1]", addr3[1], byte(69));
    ::AssertEqual("ip_address4::from_dotted_string [2]", addr3[2], byte(205));
    ::AssertEqual("ip_address4::from_dotted_string [3]", addr3[3], byte(36));

    AssertException("Invalid dotted string", ip_address4::from_dotted_string("horst"), the::format_exception); 
    AssertException("Invalid dotted string", ip_address4::from_dotted_string(L"hugo"), the::format_exception);
    AssertException("Invalid dotted string", ip_address4::from_dotted_string(the::astring("gernot")), the::format_exception);
    AssertException("Invalid dotted string", ip_address4::from_dotted_string(the::wstring(L"gunther")), the::format_exception);
}


void TestIpAddress6(void) {
}

void TestIpAgnosticAddress(void) {
    using the::system::net::address_family;
    using the::system::net::ip_address;

    ip_address::shared_pointer_type addr1;
    ::AssertTrue("ip_address ctor creates NULL", addr1 == NULL);

    //addr1 = ip_address::parse("127.0.0.1");
}


void TestIpAddress(void) {
    the::system::net::socket::startup();
    ::CommonTestIpAddress<the::system::net::ip_address4>();
    ::TestIpAddress4();
    ::CommonTestIpAddress<the::system::net::ip_address6>();
    ::TestIpAddress6();
    ::TestIpAgnosticAddress();
    the::system::net::socket::cleanup();
}


