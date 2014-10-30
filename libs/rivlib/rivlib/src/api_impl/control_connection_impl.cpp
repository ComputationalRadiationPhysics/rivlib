/*
 * control_connection_impl.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "api_impl/control_connection_impl.h"
#include "the/system/threading/auto_lock.h"
#include "the/text/string_builder.h"
#include "vislib/SimpleMessage.h"

using namespace eu_vicci::rivlib;
using namespace the::system::threading;


/*
 * control_connection_impl::self_impl::self_impl
 */
control_connection_impl::self_impl::self_impl(void) : connection_base_impl<control_connection_impl>() {
    // intentionally empty
}


/*
 * control_connection_impl::self_impl::~self_impl
 */
control_connection_impl::self_impl::~self_impl(void) {
    // intentionally empty
}


/*
 * control_connection_impl::self_impl::check_uri
 */
void control_connection_impl::self_impl::check_uri(std::string &scheme,
        std::string& username, std::string& host, bool &is_host_v6,
        unsigned short& host_port, std::string& path, std::string& query,
        std::string& fragment) {
    if (scheme != "RIV") throw the::exception("Scheme must be 'RIV'", __FILE__, __LINE__);
    if (!query.empty()) throw the::exception("Queries are not allowed for control_connections", __FILE__, __LINE__);
    if (!fragment.empty()) throw the::exception("Fragments are not allowed for control_connections", __FILE__, __LINE__);
}


/*
 * control_connection_impl::self_impl::evaluate_answer
 */
void control_connection_impl::self_impl::evaluate_answer(unsigned short code) {
    switch (code) {
    case 200: // accepted!
        break;
    case 404: // requested resource not found
        throw the::exception("Requested resource not found", __FILE__, __LINE__);
    default:
        throw the::exception(
            the::text::astring_builder::format("Unexpected server answer: %u\n", code).c_str(),
            __FILE__, __LINE__);
    }
}


/*
 * control_connection_impl::self_impl::communication_core
 */
void control_connection_impl::self_impl::communication_core(void) {
    vislib::net::SimpleMessage msg;
    size_t rec = 1;
    while (rec != 0) {
        rec = this->receive(msg.GetHeader().PeekData(), msg.GetHeader().GetHeaderSize());
        if (rec == msg.GetHeader().GetHeaderSize()) {

            if (msg.GetHeader().GetBodySize() > 0) {
                msg.AssertBodySize();
                rec = this->receive(msg.GetBody(), msg.GetHeader().GetBodySize());
                if (rec != msg.GetHeader().GetBodySize()) {
                    if (rec == 0) break;
                    throw the::exception("Incomplete message body", __FILE__, __LINE__);
                }
            }

            // message complete
            {
                auto_lock<self_impl>(*this);
                size_t l_s = this->get_listeners().size();
                for (size_t i = 0; i < l_s; ++i) {
                    this->get_listeners()[i]->on_msg(this->get_owner(),
                        msg.GetHeader().GetMessageID(),
                        msg.GetHeader().GetBodySize(),
                        msg.GetBody());
                }
            }

        } else if (rec != 0) {
            throw the::exception("Incomplete message header", __FILE__, __LINE__);
        }
    }
}


/*
 * control_connection_impl::self_impl::send_message
 */
void control_connection_impl::self_impl::send_message(unsigned int id, unsigned int size, const void *data) {
    auto_lock<self_impl>(*this);
    if (this->get_status() == connection_base::status::connected) {
        vislib::net::SimpleMessageHeader head;
        head.SetMessageID(id);
        head.SetBodySize(size);
        if (this->send(head.PeekData(), head.GetHeaderSize()) != head.GetHeaderSize()) {
            throw the::exception("failed to send header package", __FILE__, __LINE__);
        }
        if (size > 0) {
            if (this->send(data, size) != size) {
                throw the::exception("failed to send data package", __FILE__, __LINE__);
            }
        }
    }

}


/*
 * control_connection_impl::control_connection_impl
 */
control_connection_impl::control_connection_impl(void) : control_connection(),
        node(), impl() {
    this->impl.set_owner(this);
}


/*
 * control_connection_impl::~control_connection_impl
 */
control_connection_impl::~control_connection_impl(void) {
    // intentionally empty
}


/*
 * control_connection_impl::add_listener
 */
void control_connection_impl::add_listener(listener* l) {
    this->impl.add_listener(l);
}


/*
 * control_connection_impl::remove_listener
 */
void control_connection_impl::remove_listener(listener* l) {
    this->impl.remove_listener(l);
}


/*
 * control_connection_impl::remove_all_listeners
 */
void control_connection_impl::remove_all_listeners(void) {
    this->impl.remove_all_listeners();
}


/*
 * control_connection_impl::connect
 */
void control_connection_impl::connect(const char *uri) {
    this->impl.connect(uri);
}


/*
 * control_connection_impl::connect
 */
void control_connection_impl::connect(const wchar_t *uri) {
    this->impl.connect(uri);
}


/*
 * control_connection_impl::disconnect
 */
void control_connection_impl::disconnect(bool wait) {
    this->impl.disconnect(wait);
}


/*
 * control_connection_impl::get_status
 */
control_connection::status control_connection_impl::get_status(void) const {
    return this->impl.get_status();
}


/*
 * control_connection_impl::get_uri_astr
 */
const char *control_connection_impl::get_uri_astr(void) const {
    return this->impl.get_uri_astr();
}


/*
 * control_connection_impl::get_uri_wstr
 */
const wchar_t *control_connection_impl::get_uri_wstr(void) const {
    return this->impl.get_uri_wstr();
}


/*
 * control_connection_impl::send
 */
void control_connection_impl::send(unsigned int id, unsigned int size, const void *data) {
    this->impl.send_message(id, size, data);
}


/*
 * control_connection_impl::lock
 */
void control_connection_impl::lock(void) {
    this->impl.lock();
}


/*
 * control_connection_impl::unlock
 */
void control_connection_impl::unlock(void) {
    this->impl.unlock();
}


/*
 * control_connection_impl::log
 */
error_log& control_connection_impl::log(void) {
    return error_log::null_log;
}


/*
 * control_connection_impl::make_data_channel_uri
 */
size_t control_connection_impl::make_data_channel_uri(const char *name, uint16_t type, uint16_t subtype, char *uri, size_t uri_size) {

    std::string uri_scheme;
    std::string uri_username;
    std::string uri_host;
    bool uri_is_host_v6 = false;
    unsigned short uri_host_port;
    std::string uri_path;
    std::string uri_query;
    std::string uri_fragment;
    bool uri_is_port_set = false;

    uri_utility::parse_uri(this->get_uri_astr(), uri_scheme, uri_username, uri_host, uri_is_host_v6, uri_host_port, uri_is_port_set, uri_path, uri_query, uri_fragment);

    if (!uri_query.empty()) {
        throw the::exception("Uri of control_connection must not include 'query'", __FILE__, __LINE__);
    }
    if (!uri_fragment.empty()) {
        throw the::exception("Uri of control_connection must not include 'fragment'", __FILE__, __LINE__);
    }

    if (uri_scheme.empty()) {
        uri_scheme = "RIV";
    }
    if (uri_host.empty()) {
        uri_host = "localhost";
    }

    the::text::astring_builder u(uri_host);
    if (!uri_username.empty()) u.prepend(uri_username);
    u.prepend_formatted("%s://", uri_scheme.c_str());
    if (uri_is_port_set) {
        u.append_formatted(":%u", uri_host_port);
    }
    u.append_formatted("%s?n=%s&t=%u&s=%u", uri_path.c_str(), name,
        static_cast<unsigned int>(type), static_cast<unsigned int>(subtype));

    if (uri != nullptr) {
        ::memcpy(uri, u.to_string().c_str(), the::math::minimum<size_t>(uri_size, u.length()));
    }

    return u.length();
}
