/*
 * provider_impl.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "api_impl/provider_impl.h"
#include "the/text/string_converter.h"
#include "the/assert.h"
#include "the/text/string_builder.h"
#include "raw_image_data_binding_impl.h"
#include "ip_connection.h"

using namespace eu_vicci::rivlib;


/*
 * provider_impl::provider_impl
 */
provider_impl::provider_impl(const the::wstring& name) : provider(), element_node(),
        name_astr(), name_wstr(name), on_user_msg() {
    the::text::string_converter::convert(this->name_astr, this->name_wstr);
}


/*
 * provider_impl::~provider_impl
 */
provider_impl::~provider_impl(void) {
    this->disconnect_all();
}


/*
 * provider_impl::provider_impl
 */
const char* provider_impl::get_name_astr(void) const {
    return this->name_astr.c_str();
}


/*
 * provider_impl::provider_impl
 */
const wchar_t* provider_impl::get_name_wstr(void) const {
    return this->name_wstr.c_str();
}


/*
 * provider_impl::provider_impl
 */
void provider_impl::add_data_binding(data_binding::ptr binding) {
    this->connect(binding);
    // do not handle connection here. Instead us the 'connecting' and 'connected' events.
}


/*
 * provider_impl::provider_impl
 */
void provider_impl::remove_data_binding(data_binding::ptr binding) {
    // do not handle connection here. Instead us the 'disconnecting' and 'disconnected' events.
    this->disconnect(binding);
}


/*
 * provider_impl::provider_impl
 */
void provider_impl::remove_all_data_bindings(void) {
    // do not handle connection here. Instead us the 'disconnecting' and 'disconnected' events.
    std::vector<api_ptr_base> peers = this->select<data_binding>();
    size_t peer_cnt = peers.size();
    for (size_t i = 0; i < peer_cnt; ++i) {
        this->disconnect(peers[i]);
    }
}


/*
 * provider_impl::provider_impl
 */
void provider_impl::enumerate_data_bindings(data_binding_enumerator enumerator, void *ctxt) {
    std::vector<api_ptr_base> peers = this->select<data_binding>();
    size_t peer_cnt = peers.size();
    for (size_t i = 0; i < peer_cnt; ++i) {
        data_binding::ptr* p = dynamic_cast<data_binding::ptr*>(&peers[i]);
        if (p == nullptr) continue;
        if (!enumerator(*p, ctxt)) break;
    }
}


/*
 * provider_impl::shutdown
 */
void provider_impl::shutdown(void) {
    core::ptr core = this->get_core();
    if (core) {
        this->disconnect(core);
    }
    this->disconnect_all_recursively();
}


/*
 * provider_impl::add_user_message_callback
 */
void provider_impl::add_user_message_callback(user_message_callback_delegate func, void *ctxt) {
    this->on_user_msg += the::delegate<void, unsigned int, unsigned int, const char*>(func, ctxt);
}


/*
 * provider_impl::remove_user_message_callback
 */
void provider_impl::remove_user_message_callback(user_message_callback_delegate func, void *ctxt) {
    this->on_user_msg -= the::delegate<void, unsigned int, unsigned int, const char*>(func, ctxt);
}


/*
 * provider_impl::broadcast_message
 */
void provider_impl::broadcast_message(unsigned int id, unsigned int size, const char *data) {
    std::vector<api_ptr_base> peers = this->select<rivlib::ip_connection>();
    size_t peer_cnt = peers.size();
    for (size_t i = 0; i < peer_cnt; ++i) {
        ip_connection* p = dynamic_cast<ip_connection*>(peers[i].get());
        if (p == nullptr) continue;
        p->send_message(id, size, data);
    }

}


/*
 * provider_impl::query_channels
 */
std::vector<data_channel_info> provider_impl::query_channels(void) {
    std::vector<data_channel_info> rv;

    std::vector<api_ptr_base> dbs = this->select<data_binding>();
    size_t cnt = dbs.size();
    for (size_t i = 0; i < cnt; i++) {
        data_binding *db = dynamic_cast<data_binding*>(dbs[i].get());
        THE_ASSERT(db != nullptr);

        raw_image_data_binding_impl* ridbi = dynamic_cast<raw_image_data_binding_impl*>(db);
        if (ridbi != nullptr) {
            // TODO: Enumerate through available image data encoders

            //
            // Note:
            //  The quality numbers will controll which encoder wil be used.
            //  Usually the encoder with the highest number will be used.
            //

            data_channel_info dci;
            uintptr_t ptr = reinterpret_cast<uintptr_t>(dbs[i].get());
            unsigned char buf[sizeof(uintptr_t)];
            ::memcpy(buf, &ptr, sizeof(uintptr_t));
            dci.name = the::text::string_utility::to_hex_astring(buf, sizeof(uintptr_t));

            dci.type = data_channel_type::image_stream;
            dci.subtype.image_stream = data_channel_image_stream_subtype::rgb_raw;
            dci.quality = 8; // not very good

            rv.push_back(dci);

            // replaced by ffmpeg implementation
            /*
            ptr = reinterpret_cast<uintptr_t>(dbs[i].get());
            ::memcpy(buf, &ptr, sizeof(uintptr_t));
            dci.name = the::text::string_utility::to_hex_astring(buf, sizeof(uintptr_t));

            dci.type = data_channel_type::image_stream;
            dci.subtype.image_stream = data_channel_image_stream_subtype::rgb_zip;
            dci.quality = 16; // not good; compressed, but high latency

            rv.push_back(dci);
            */

#if(USE_MJPEG == 1)
            ptr = reinterpret_cast<uintptr_t>(dbs[i].get());
            ::memcpy(buf, &ptr, sizeof(uintptr_t));
            dci.name = the::text::string_utility::to_hex_astring(buf, sizeof(uintptr_t));

            dci.type = data_channel_type::image_stream;
            dci.subtype.image_stream = data_channel_image_stream_subtype::rgb_mjpeg;
            dci.quality = 24; // ok; compressed, some latency

            rv.push_back(dci);
#endif

            // now uses ffmpeg
            ptr = reinterpret_cast<uintptr_t>(dbs[i].get());
            ::memcpy(buf, &ptr, sizeof(uintptr_t));
            dci.name = the::text::string_utility::to_hex_astring(buf, sizeof(uintptr_t));

            dci.type = data_channel_type::image_stream;
            dci.subtype.image_stream = data_channel_image_stream_subtype::rgb_zip;
            dci.quality = 32;

            rv.push_back(dci);
        }

    }
    return rv;
}


/*
 * provider_impl::on_user_message_received
 */
void provider_impl::on_user_message_received(unsigned int id, unsigned int size, const char *data) {
    this->on_user_msg(id, size, data);
}
