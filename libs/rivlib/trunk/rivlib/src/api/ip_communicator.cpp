/*
 * ip_communicator.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "rivlib/ip_communicator.h"
#include "api_impl/ip_communicator_impl.h"

using namespace eu_vicci::rivlib;


/*
 * ip_communicator::DEFAULT_PORT
 */
const unsigned short ip_communicator::DEFAULT_PORT = 52000;


/*
 * ip_communicator::create
 */
communicator::ptr ip_communicator::create(unsigned short port) {
    return new ip_communicator_impl(port);
}


/*
 * ip_communicator::~ip_communicator
 */
ip_communicator::~ip_communicator(void) {
    // intentionally empty
}


/*
 * ip_communicator::ip_communicator
 */
ip_communicator::ip_communicator(void) {
    // intentionally empty
}


///*
// * ip_communicator::public_uris
// */
//void ip_communicator::public_uris(const provider *pro,
//        std::vector<std::string>& out_uris) {
//    THE_ASSERT(pro != nullptr);
//    // names always as global uris with port
//    std::string safe_name(the::text::string_utility::url_encode(pro->get_name()));
//
//    // 1: network name
//    std::string comp_name;
//    the::system::system_information::computer_name(comp_name);
//    out_uris.push_back(the::text::astring_builder::format("riv://%s:%u/%s", comp_name.c_str(), this->impl->get_port(), safe_name.c_str()));
//
//    // 2: IPv4
//    vislib::net::NetworkInformation::AdapterList ads;
//    vislib::net::NetworkInformation::GetAdaptersForType(ads, vislib::net::NetworkInformation::Adapter::Type::TYPE_ETHERNET);
//    vislib::net::NetworkInformation::AdapterList ads2;
//    vislib::net::NetworkInformation::GetAdaptersForType(ads2, vislib::net::NetworkInformation::Adapter::Type::TYPE_IEEE80211);
//    for (size_t i = 0; i < ads2.Count(); i++) ads.Add(ads2[i]);
//    // TYPE_LOOPBACK etc. does not make any sense here, because I want address how I can be found
//
//    for (size_t i = 0; i < ads.Count(); i++) {
//        const vislib::net::NetworkInformation::Adapter &a = ads[i];
//
//        vislib::net::NetworkInformation::Confidence conf;
//        vislib::net::NetworkInformation::Adapter::OperStatus stat = a.GetStatus(&conf);
//        if (conf != vislib::net::NetworkInformation::INVALID) {
//            if ((stat != vislib::net::NetworkInformation::Adapter::OperStatus::OPERSTATUS_UP)
//                    && (stat != vislib::net::NetworkInformation::Adapter::OperStatus::OPERSTATUS_UNKNOWN)) {
//                // skip adapters which are most likely down
//                continue;
//            }
//        }
//
//        try {
//            vislib::net::IPAgnosticAddress addr = a.GetUnicastAddress(vislib::net::IPAgnosticAddress::AddressFamily::FAMILY_INET);
//            vislib::StringA addrStr(addr.ToStringA());
//            if (!addrStr.IsEmpty()) {
//                out_uris.push_back(the::text::astring_builder::format("riv://%s:%u/%s", addrStr.PeekBuffer(), this->impl->get_port(), safe_name.c_str()));
//            }
//
//        } catch(...) {
//        }
//    }
//
//    // 3: IPv6
//    for (size_t i = 0; i < ads.Count(); i++) {
//        const vislib::net::NetworkInformation::Adapter &a = ads[i];
//
//        vislib::net::NetworkInformation::Confidence conf;
//        vislib::net::NetworkInformation::Adapter::OperStatus stat = a.GetStatus(&conf);
//        if (conf != vislib::net::NetworkInformation::INVALID) {
//            if ((stat != vislib::net::NetworkInformation::Adapter::OperStatus::OPERSTATUS_UP)
//                    && (stat != vislib::net::NetworkInformation::Adapter::OperStatus::OPERSTATUS_UNKNOWN)) {
//                // skip adapters which are most likely down
//                continue;
//            }
//        }
//
//        try {
//            vislib::net::IPAgnosticAddress addr = a.GetUnicastAddress(vislib::net::IPAgnosticAddress::AddressFamily::FAMILY_INET6);
//            vislib::StringA addrStr(addr.ToStringA());
//            if (!addrStr.IsEmpty()) {
//                out_uris.push_back(the::text::astring_builder::format("riv://[%s]:%u/%s", addrStr.PeekBuffer(), this->impl->get_port(), safe_name.c_str()));
//            }
//
//        } catch(...) {
//        }
//    }
//
//}
