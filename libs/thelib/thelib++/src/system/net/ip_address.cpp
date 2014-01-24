/*
 * src\system\net\ip_address.cpp
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

#include "the/system/net/ip_address.h"

#include "the/argument_exception.h"
#include "the/format_exception.h"
#include "the/memory.h"

#include "the/text/string_converter.h"

#include "the/system/net/debug.h"
#include "the/system/net/dns.h"
#include "the/system/net/ip_host_entry.h"


/*
 * the::system::net::ip_address::any
 */
the::system::net::ip_address::shared_pointer_type
the::system::net::ip_address::any(
        const the::system::net::address_family& addressFamily) {
    THE_STACK_TRACE;
    switch (addressFamily) {
        case the::system::net::address_family::inter_network4:
            return shared_pointer_type(new ip_address4(ip_address4::any));

        case the::system::net::address_family::inter_network6:
            return shared_pointer_type(new ip_address6(ip_address6::any));

        default:
            THROW_THE_EXCEPTION(argument_exception, "addressFamily");
    }
}


/*
 * the::system::net::ip_address::parse
 */
the::system::net::ip_address::shared_pointer_type
the::system::net::ip_address::parse(
        const char *hostNameOrAddress,
        const the::system::net::address_family preferredFamily) {
    THE_STACK_TRACE;

    ip_address *addr = nullptr;
    the::system::net::address_family families[2];

    /* Determine the order in which we try to parse the address string. */
    switch (preferredFamily) {
        case the::system::net::address_family::inter_network4:
            families[0] = the::system::net::address_family::inter_network4;
            families[1] = the::system::net::address_family::inter_network6;
            break;

        case the::system::net::address_family::inter_network6:
            families[0] = the::system::net::address_family::inter_network4;
            families[1] = the::system::net::address_family::inter_network6;
            break;

        default:
            THE_ASSERT(addr == NULL);
            THE_NETWORKING_TRACE_ERROR("The address familiy %d is not "
                "supported for parsing IP addresses.\n", preferredFamily);
            THROW_THE_EXCEPTION(argument_exception, "preferredFamily");
    }

    /* 
     * As a first try, interpret input as address bytes in order defined by
     * the preference for a specific address family.
     */
    for (size_t i = 0; (i < sizeof(families) / sizeof(*families))
            && (addr == nullptr); ++i) {
        addr = ip_address::parseOnly(hostNameOrAddress, families[i]);
    }

    if (addr != nullptr) {
        /* The string was a dotted or hexadecimal IP address, return it. */
        return shared_pointer_type(addr);

    } else {
        /* 
         * Failed to parse address bytes from the string, so try to lookup the
         * host name in DNS now.
         */
        aip_host_entry hostEntry;
        aip_host_entry::address_list_type addrs;

        THE_NETWORKING_TRACE_DEBUG("Resolving \"%s\" using DNS...\n",
            hostNameOrAddress);
        dns::get_host_entry(hostEntry, hostNameOrAddress);
        addrs = hostEntry.get_addresses();

        for (size_t i = 0; i < addrs.size(); ++i) {
            if (addrs[i]->address_family() == preferredFamily) {
                return addrs[i];
            }
        }
        /* No preferred family found, so use the first address. */
        
        return addrs[0];
    }
}


/*
 * the::system::net::ip_address::parse
 */
the::system::net::ip_address::shared_pointer_type
the::system::net::ip_address::parse(
        const wchar_t *hostNameOrAddress,
        const the::system::net::address_family preferredFamily) {
    THE_STACK_TRACE;
    return ip_address::parse(THE_W2A(hostNameOrAddress), preferredFamily);
}


/*
 * the::system::net::ip_address::~ip_address
 */
the::system::net::ip_address::~ip_address(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::ip_address::compare
 */
int the::system::net::ip_address::compare(const ip_address& rhs) const {
    THE_STACK_TRACE;
    int d = static_cast<int>(this->size()) - static_cast<int>(rhs.size());

    if (d == 0) {
        /* Addresses have the same length, so compare the content. */
        const byte *l = this->get_address_bytes();
        const byte *r = rhs.get_address_bytes();

        // Note: Address bytes are in network byte order, i.e. the most 
        // significant byte is at index 0.
        for (int i = static_cast<int>(this->size()) - 1; i >= 0; --i) {
            d = static_cast<int>(l[i]) - static_cast<int>(r[i]);
            if (d != 0) {
                break;
            }
        }
    }

    return d;
}


/*
 * the::system::net::ip_address::to_wstring
 */
the::wstring the::system::net::ip_address::to_wstring(void) const {
    THE_STACK_TRACE;
    the::wstring retval;
    return text::string_converter::convert(retval, this->to_astring());
}


/*
 * the::system::net::ip_address::operator []
 */
byte the::system::net::ip_address::operator [](const int i) const {
    THE_STACK_TRACE;
    if ((0 <= i) && (i < static_cast<int>(this->size()))) {
        return this->get_address_bytes()[i];
    } else {
        throw index_out_of_range_exception(0, i, 
            static_cast<int>(this->size() - 1), __FILE__, __LINE__);
    }
}


/*
 * the::system::net::ip_address::netmask_to_prefix
 */
uint32_t the::system::net::ip_address::netmask_to_prefix(
        const byte *netmask, const size_t lenNetmask) {
    THE_STACK_TRACE;
    const byte *mask = netmask;
    const byte *end = mask + lenNetmask;
    uint32_t retval = 0;

    if (netmask == NULL) {
        throw new argument_null_exception("netmask", __FILE__, __LINE__);
    }

    while ((mask < end) && (*mask == 0xFF)) {
        retval += 8;
        mask++;
    }

    if (mask < end) {
        byte tmp = *mask;
        while (tmp != 0) {
            retval++;
            tmp <<= 1;
        }
        mask++;
    }

    while (mask < end) {
        if (*mask != 0) {
            throw argument_exception("netmask", __FILE__, __LINE__);
        }
        mask++;
    }

    return retval;
}


/*
 * the::system::net::ip_address::prefix_to_netmask
 */
void the::system::net::ip_address::prefix_to_netmask(byte *outNetmask,
        const size_t lenNetmask, const uint32_t prefix) {
    THE_STACK_TRACE;
    byte *mask = outNetmask;
    int32_t remBits = static_cast<int32_t>(prefix);
    int32_t maxPrefix = 8 * static_cast<int32_t>(lenNetmask);

    if (outNetmask == NULL) {
        throw argument_null_exception("outNetmask", __FILE__, __LINE__);
    }

    if ((remBits < 0) || (remBits > maxPrefix)) {
        throw argument_exception("outNetmask", __FILE__, __LINE__);
    }

    zero_memory(mask, lenNetmask);
    while (remBits > 0) {
        if (remBits >= 8) {
            *mask++ = 0xFF;
            remBits -= 8;
            continue;
        }
        
        *mask |= 1 << (8 - remBits);
        remBits--;
    }
}


/*
 * the::system::net::ip_address::ip_address
 */
the::system::net::ip_address::ip_address(void) : base() {
    THE_STACK_TRACE;
}


/*
 * the::system::net::ip_address:apply_netmask
 */
void the::system::net::ip_address::apply_netmask(
        const ip_address& mask) {
    THE_STACK_TRACE;
    byte *address = this->get_address_bytes();

    if (this->size() != mask.size()) {
        throw argument_exception("mask", __FILE__, __LINE__);
    }

    for (size_t i = 0; i < this->size(); ++i) {
        int ii = static_cast<int>(i);
        address[ii] &= mask[ii];
    }
}


/*
 * the::system::net::ip_address:apply_netmask
 */
void the::system::net::ip_address::apply_netmask(
        const uint32_t prefixLength) {
    THE_STACK_TRACE;
    byte *address = this->get_address_bytes();
    byte *mask = static_cast<byte *>(::alloca(this->size()));
    
    ip_address::prefix_to_netmask(mask, this->size(), prefixLength);

    for (size_t i = 0; i < this->size(); ++i) {
        address[i] &= mask[i];
    }
}


/*
 * the::system::net::ip_address::parseOnly
 */
the::system::net::ip_address *the::system::net::ip_address::parseOnly(
        const char *hostNameOrAddress,
        const the::system::net::address_family family) {
    THE_STACK_TRACE;
    THE_ASSERT((family == the::system::net::address_family::inter_network4)
        || (family == the::system::net::address_family::inter_network6));

    ip_address *retval = nullptr;

    THE_NETWORKING_TRACE_DEBUG("Parsing \"%s\" without DNS lookup as address "
        "of family %d...\n", hostNameOrAddress, family);
    try {
        switch (family) {
            case the::system::net::address_family::inter_network4:
                retval = new ip_address4(ip_address4::from_dotted_string(
                    hostNameOrAddress));
                THE_NETWORKING_TRACE_DEBUG("Parsed \"%s\" using "
                    "ip_address4::from_dotted_string.\n", hostNameOrAddress);
                break;

            case the::system::net::address_family::inter_network6:
                retval = new ip_address6(ip_address6::from_hexadecimal_string(
                    hostNameOrAddress));
                THE_NETWORKING_TRACE_DEBUG("Parsed \"%s\" using "
                    "ip_address6::from_hexadecimal_string.\n", hostNameOrAddress);
                break;

            default:
                THE_ASSERT(false);
                break;
        }
    } catch (the::format_exception) {
        THE_ASSERT(retval == NULL);
        THE_NETWORKING_TRACE_WARN("Cannot parse \"%s\" as IP addressd, because "
            "the string is invalid.\n", hostNameOrAddress);
    }

    return retval;
}
