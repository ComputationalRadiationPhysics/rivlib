/*
 * ip_communicator_impl.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "api_impl/ip_communicator_impl.h"
#include "ip_connection.h"
#include "the/assert.h"
#include "the/memory.h"
#include "the/math/functions.h"
#include "the/system/system_information.h"
#include "the/text/string_builder.h"
#include "vislib/IPCommEndPoint.h"
#include "vislib/SocketException.h"
#include "vislib/PeerDisconnectedException.h"
#include "vislib/Trace.h"
#include "vislib/NetworkInformation.h"

using namespace eu_vicci::rivlib;


/*
 * ip_communicator_impl::ip_communicator_impl
 */
ip_communicator_impl::ip_communicator_impl(unsigned short port)
        : ip_communicator(), element_node(), runnable(), port(port), comm(),
        worker(nullptr), public_uri_cache_src(), public_uri_cache(),
        public_uri_cache_lock() {
    vislib::net::Socket::Startup();
    this->worker = new the::system::threading::thread(this);
}


/*
 * ip_communicator_impl::~ip_communicator_impl
 */
ip_communicator_impl::~ip_communicator_impl(void) {
    this->public_uri_cache_src = nullptr; // do not delete
    THE_ASSERT(this->worker != nullptr);
    if (this->worker->is_running()) {
        this->worker->terminate(true);
    }
    the::safe_delete(this->worker);
    vislib::net::Socket::Cleanup();
}


/*
 * ip_communicator_impl::on_core_discovered
 */
void ip_communicator_impl::on_core_discovered(void) {
    element_node::on_core_discovered();
    if (this->worker->is_running()) {
        this->worker->terminate(true);
    }
    this->worker->start();
}


/*
 * ip_communicator_impl::on_core_lost
 */
void ip_communicator_impl::on_core_lost(void) {
    element_node::on_core_lost();
    if (this->worker->is_running()) {
        this->worker->terminate(true);
    }
}


/*
 * ip_communicator_impl::on_thread_terminating
 */
the::system::threading::thread::termination_behaviour
ip_communicator_impl::on_thread_terminating(void) throw() {
    try {
        if (!this->comm.IsNull()) {
            this->comm->Close();
            return the::system::threading::thread::termination_behaviour::graceful;
        }
    } catch(...) {
    }
    return the::system::threading::thread::termination_behaviour::forceful;
}


/*
 * ip_communicator_impl::run
 */
int ip_communicator_impl::run(void) {
    using namespace vislib::net;

    this->log().info("ip_communicator: started");

#if defined(DEBUG) || defined(_DEBUG)
    vislib::Trace::GetInstance().SetLevel(vislib::Trace::LEVEL_NONE);
#endif /* defined(DEBUG) || defined(_DEBUG) */

    this->comm = TcpCommChannel::Create(TcpCommChannel::FLAG_NODELAY);
    vislib::SmartRef<TcpCommChannel> client;

    try {
        this->comm->Bind(IPCommEndPoint::Create(IPCommEndPoint::IPV4, this->port));

        while(true) {
            this->comm->Listen();
            client = nullptr;

            try {
                client = this->comm->Accept().DynamicCast<TcpCommChannel>();
                this->log().info("ip_communicator: incoming connection request");

                {
                    api_ptr_base conn(new ip_connection(client));
                    this->connect(conn);
                }
                client.Release();

            } catch(...) {
                try {
                    if (!client.IsNull()) client->Close();
                } catch(...) {
                }
                client.Release();
            }
            try {
                if (!client.IsNull()) client->Close();
            } catch(...) {
                client.Release();
            }
            try {
                client.Release();
            } catch(...) {
            }
        }

    } catch(PeerDisconnectedException ex) {
        // expected behaviour when server is closed

    } catch(SocketException ex) {
#ifdef _WIN32
        if ((ex.GetErrorCode() == 10004) || (ex.GetErrorCode() == 10038)) {
#else /* _WIN32 */
        if (false) {
#endif /* _WIN32 */
            // expected behaviour when server is closed
        } else {
            this->log().error("ip_communicator server closed: %s (%s, %d)\n",
                ex.GetMsgA(), ex.GetFile(), ex.GetLine());

        }
    } catch(vislib::Exception ex) {
        this->log().error("ip_communicator server closed: %s (%s, %d)\n",
            ex.GetMsgA(), ex.GetFile(), ex.GetLine());

    } catch(...) {
        this->log().error("ip_communicator server closed: unexpected exception\n");

    }

    try {
        this->comm->Close();
    } catch(...) {
    }
    try {
        this->comm.Release();
    } catch(...) {
    }

    this->log().info("ip_communicator: stopped");

    return 0;
}


/*
 * ip_communicator_impl::count_public_uris
 */
size_t ip_communicator_impl::count_public_uris(provider::ptr prov) {
    the::system::threading::auto_lock<the::system::threading::critical_section>(this->public_uri_cache_lock);
    this->assert_public_uri_cache(prov);
    return this->public_uri_cache.size();
}


/*
 * ip_communicator_impl::public_uri
 */
size_t ip_communicator_impl::public_uri(provider::ptr prov, size_t idx, char *buf, size_t buflen) {
    the::system::threading::auto_lock<the::system::threading::critical_section>(this->public_uri_cache_lock);
    this->assert_public_uri_cache(prov);

    if (idx >= this->public_uri_cache.size()) return 0;
    std::string& s = this->public_uri_cache[idx];
    if (buf == nullptr) return s.size();

    size_t l = the::math::minimum<size_t>(s.size(), buflen);
    ::memcpy(buf, s.c_str(), l);

    return l;
}


/*
 * ip_communicator_impl::assert_public_uri_cache
 */
void ip_communicator_impl::assert_public_uri_cache(provider::ptr prov) {
    THE_ASSERT(prov.get() != nullptr);
    if (this->public_uri_cache_src == prov.get()) return;
    this->public_uri_cache_src = prov.get();
    this->public_uri_cache.clear();

    // names always as global uris with port
    std::string safe_name(the::text::string_utility::url_encode(prov->get_name_wstr()));

    // 1: network name
    std::string comp_name;
    the::system::system_information::computer_name(comp_name);
    this->public_uri_cache.push_back(the::text::astring_builder::format("riv://%s:%u/%s", comp_name.c_str(), this->port, safe_name.c_str()));

    // 2: IPv4
    vislib::net::NetworkInformation::AdapterList ads;
    vislib::net::NetworkInformation::GetAdaptersForType(ads, vislib::net::NetworkInformation::Adapter::Type::TYPE_ETHERNET);
    vislib::net::NetworkInformation::AdapterList ads2;
    vislib::net::NetworkInformation::GetAdaptersForType(ads2, vislib::net::NetworkInformation::Adapter::Type::TYPE_IEEE80211);
    for (size_t i = 0; i < ads2.Count(); i++) ads.Add(ads2[i]);
    // TYPE_LOOPBACK etc. does not make any sense here, because I want address how I can be found

    for (size_t i = 0; i < ads.Count(); i++) {
        const vislib::net::NetworkInformation::Adapter &a = ads[i];

        vislib::net::NetworkInformation::Confidence conf;
        vislib::net::NetworkInformation::Adapter::OperStatus stat = a.GetStatus(&conf);
        if (conf != vislib::net::NetworkInformation::INVALID) {
            if ((stat != vislib::net::NetworkInformation::Adapter::OperStatus::OPERSTATUS_UP)
                    && (stat != vislib::net::NetworkInformation::Adapter::OperStatus::OPERSTATUS_UNKNOWN)) {
                // skip adapters which are most likely down
                continue;
            }
        }

        try {
            vislib::net::IPAgnosticAddress addr = a.GetUnicastAddress(vislib::net::IPAgnosticAddress::AddressFamily::FAMILY_INET);
            if (!addr.IsV4()) continue;
            vislib::StringA addrStr(addr.ToStringA());
            if (!addrStr.IsEmpty()) {
                this->public_uri_cache.push_back(the::text::astring_builder::format("riv://%s:%u/%s", addrStr.PeekBuffer(), this->port, safe_name.c_str()));
            }

        } catch(...) {
        }
    }

    // 3: IPv6
    for (size_t i = 0; i < ads.Count(); i++) {
        const vislib::net::NetworkInformation::Adapter &a = ads[i];

        vislib::net::NetworkInformation::Confidence conf;
        vislib::net::NetworkInformation::Adapter::OperStatus stat = a.GetStatus(&conf);
        if (conf != vislib::net::NetworkInformation::INVALID) {
            if ((stat != vislib::net::NetworkInformation::Adapter::OperStatus::OPERSTATUS_UP)
                    && (stat != vislib::net::NetworkInformation::Adapter::OperStatus::OPERSTATUS_UNKNOWN)) {
                // skip adapters which are most likely down
                continue;
            }
        }

        try {
            vislib::net::IPAgnosticAddress addr = a.GetUnicastAddress(vislib::net::IPAgnosticAddress::AddressFamily::FAMILY_INET6);
            if (!addr.IsV6()) continue;
            vislib::StringA addrStr(addr.ToStringA());
            if (!addrStr.IsEmpty()) {
                this->public_uri_cache.push_back(the::text::astring_builder::format("riv://[%s]:%u/%s", addrStr.PeekBuffer(), this->port, safe_name.c_str()));
            }

        } catch(...) {
        }
    }

}
