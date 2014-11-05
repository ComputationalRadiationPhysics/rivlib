/*
 * rivlib
 * ip_connection.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "ip_connection.h"
#include "message_image_request.h"
#include "rivlib/ip_utilities.h"
#include "rivlib/image_data_binding.h"
#include "api_impl/provider_impl.h"
#include "encoder/image_encoder_rgb_raw.h"
#include "encoder/image_encoder_rgb_zip.h"
#include "encoder/image_request.h"
#include "thread_scrubber.h"
#include "the/assert.h"
#include "the/blob.h"
#include "the/math/functions.h"
#include "the/text/string_builder.h"
#include "vislib/PeerDisconnectedException.h"
#include "vislib/SocketException.h"
#include "vislib/SimpleMessage.h"
#include "uri_utility.h"
#include <string>
#include <sstream>
#include <climits>

using namespace eu_vicci::rivlib;
using namespace the::system::threading;


/*
 * ip_connection::ip_connection
 */
ip_connection::ip_connection(comm_channel_type comm) : element_node(),
        runnable(), comm(comm), worker_thread(nullptr), is_terminating(false) {
    this->worker_thread = new thread(this);
    vislib::net::Socket::Startup();
}


/*
 * ip_connection::~ip_connection
 */
ip_connection::~ip_connection(void) {
    THE_ASSERT(!this->worker_thread->is_running());
    //if (this->worker_thread->is_running()) {
    //    this->worker_thread->terminate(true);
    //}
    THE_ASSERT(this->comm.IsNull());
    the::safe_delete(this->worker_thread);

    vislib::net::Socket::Cleanup();
}


/*
 * ip_connection::on_thread_terminating
 */
thread::termination_behaviour ip_connection::on_thread_terminating(void) throw() {
    try {
        if (!this->comm.IsNull()) {
            this->comm->Close();
            return thread::termination_behaviour::graceful;
        }
    } catch(...) {
    }
    return thread::termination_behaviour::forceful;
}


/*
 * ip_connection::on_thread_terminated
 */
void ip_connection::on_thread_terminated(const thread::exit_reason reason) throw() {
    this->is_terminating = true;
    std::shared_ptr<node_base> self = this->get_api_ptr();
    this->disconnect_all();
    thread_scrubber::cleanup_object<ip_connection>(self);
}


/*
 * ip_connection::run
 */
int ip_connection::run(void) {
    THE_ASSERT(!this->comm.IsNull());

    thread_scrubber::register_object<ip_connection>(this->get_api_ptr());

    ip_handshake_id id;
    ::memcpy(id.id_str, "RIV\x13\x57\x9B\xDF\x00", 8);
    id.tst_dword = 0x12345678;
    id.tst_float = 2.71828175f;
    THE_ASSERT(sizeof(ip_handshake_id) == 16);

    try {

        this->comm->Send(&id, sizeof(ip_handshake_id));

        unsigned int requestLen = 0;
        if (this->comm->Receive(&requestLen, 4) != 4) {
            throw the::exception("Unable to receive request", __FILE__, __LINE__);
        }
        char *request = new char[requestLen + 1];
        ::memset(request, 0, requestLen + 1);
        try {

            if (this->comm->Receive(request, requestLen) != requestLen) {
                throw the::exception("Unable to receive request", __FILE__, __LINE__);
            }

        } catch(...) {
            delete[] request;
            throw;
        }
        std::string req(request);
        delete[] request;

        this->log().info("ip_connection: request to \"%s\"", req.c_str());

        // avoid seqfaults on empty req strings
        if( req.empty() )
            return 1;

        std::string scheme, req_user, host, req_path, req_query, req_fragment;
        bool is_host_v6, is_host_port_set;
        unsigned short host_port;
        uri_utility::parse_uri(req, scheme, req_user, host, is_host_v6, host_port, is_host_port_set, req_path, req_query, req_fragment);

        //req_user = the::text::string_utility::url_decode<std::string>(req_user); not used ATM
        req_path = the::text::string_utility::url_decode<std::string>(req_path);
        req_query = the::text::string_utility::url_decode<std::string>(req_query);
        req_fragment = the::text::string_utility::url_decode<std::string>(req_fragment);

        if (req_query.empty() && req_fragment.empty()) {
            // direct request for providers
            this->ctrl_chan_receiver(req_path);

        } else if (req_fragment.empty()) {
            // direct request with query for providers
            this->data_chan_receiver(req_path, req_query);

        } else {
            // now conditions met
            unsigned short answer = 500;
            this->comm->Send(&answer, 2);

            this->log().error("Request \"%s\" could not be fulfilled\n", req.c_str());
        }

    } catch(vislib::net::PeerDisconnectedException ex) {
        // expected behaviour when server is closed

    } catch(vislib::net::SocketException ex) {
#ifdef _WIN32
        if ((ex.GetErrorCode() == 10004) || (ex.GetErrorCode() == 10038)) {
#else /* _WIN32 */
        if (false) {
#endif /* _WIN32 */
            // expected behaviour when server is closed
        } else {
            this->log().error("ip_connection closed: %s (%s, %d)\n",
                ex.GetMsgA(), ex.GetFile(), ex.GetLine());

        }
    } catch(vislib::Exception ex) {
        this->log().error("ip_connection closed: %s (%s, %d)\n",
            ex.GetMsgA(), ex.GetFile(), ex.GetLine());

    } catch(the::exception ex) {
        this->log().error("ip_connection closed: %s (%s, %d)\n",
            ex.get_msg_astr(), ex.get_file(), ex.get_line());

    } catch(...) {
        this->log().error("ip_connection closed: unexpected exception\n");

    }

    this->log().info("ip_connection: closing");

    try {
        this->comm->Close();
        this->comm.Release();
    } catch(...) {
    }

    return 0;
}


/*
 * ip_connection::send_message
 */
void ip_connection::send_message(unsigned int id, unsigned int size, const char* data) {
    // HAZARD: Possible synchronization probleme here, because this could be called from several different threads ...
    using namespace vislib::net;

    SimpleMessageHeader h;

    h.SetMessageID(id);
    h.SetBodySize(static_cast<SimpleMessageSize>(size));

    SIZE_T sent = this->comm->Send(h.PeekData(), h.GetHeaderSize());
    if (sent != h.GetHeaderSize()) {
        fprintf(stderr, "Failed to send package header\n");
        return;
    }
    if (h.GetBodySize() > 0) {
        sent = this->comm->Send(data, size);
        if (sent != size) {
            fprintf(stderr, "Failed to send package body\n");
            return;
        }
    }
}


/*
 * ip_connection::on_core_discovered
 */
void ip_connection::on_core_discovered(void) {
    element_node::on_core_discovered();
    if (!this->worker_thread->is_running() && !this->comm.IsNull()) {
        this->worker_thread->start();
    }
}


/*
 * ip_connection::on_core_lost
 */
void ip_connection::on_core_lost(void) {
    element_node::on_core_lost();
    if (!this->is_terminating && this->worker_thread->is_running()) {
        // do not wait because the object will be cleaned up asynchroniously
        this->worker_thread->terminate(false);
    }
}


/*
 * ip_connection::send_image_data
 */
void ip_connection::send_image_data(const data::buffer::shared_ptr data, void *ctxt) {
    using namespace vislib::net;
    ip_connection *that = static_cast<ip_connection*>(ctxt);
    SimpleMessageHeader h;

    h.SetMessageID(static_cast<uint32_t>(message_id::image_data_blob));
    h.SetBodySize(static_cast<SimpleMessageSize>(2 * sizeof(uint32_t) + data->metadata().size() + data->data().size()));

    the::system::threading::thread::sleep(1);

    // HAZARD: Sending image data but comm has been closed (and deleted) already!

    SIZE_T sent = that->comm->Send(h.PeekData(), h.GetHeaderSize());
    if (sent != h.GetHeaderSize()) {
        fprintf(stderr, "Failed to send package header\n");
        return;
    }
    if (h.GetBodySize() > 0) {
        uint32_t bytes[2];
        bytes[0] = static_cast<uint32_t>(data->type());
        bytes[1] = data->time_code();

        sent = that->comm->Send(bytes, 2 * sizeof(uint32_t));
        if (sent != 2 * sizeof(uint32_t)) {
            fprintf(stderr, "Failed to send package body (1/3)\n");
            return;
        }
        sent = that->comm->Send(data->metadata(), data->metadata().size());
        if (sent != data->metadata().size()) {
            fprintf(stderr, "Failed to send package body (2/3)\n");
            return;
        }
        sent = that->comm->Send(data->data(), data->data().size());
        if (sent != data->data().size()) {
            fprintf(stderr, "Failed to send package body (3/3)\n");
            return;
        }
    }
}


/*
 * ip_connection::find_provider
 */
api_ptr_base ip_connection::find_provider(const std::string& path) {
    std::stringstream stream(path);
    std::string name;
    api_ptr_base obj = this->get_core();

    while (obj && std::getline(stream, name, '/')) {
        if (name.empty()) continue;
        std::vector<api_ptr_base> peers = dynamic_cast<node*>(obj.get())->select<node>();
        size_t cnt = peers.size();
        obj.reset();

        for (size_t i = 0; i < cnt; ++i) {
            provider *p = dynamic_cast<provider *>(peers[i].get());
            if (p != nullptr) {
                if (name.compare(p->get_name_astr()) == 0) {
                    obj = peers[i];
                    break;
                }
            }
        }
    }

    return obj;
}


/*
 * ip_connection::ctrl_chan_receiver
 */
void ip_connection::ctrl_chan_receiver(const std::string& path) {
    using namespace vislib::net;
    SimpleMessage msg;
    size_t rec = 1;
    unsigned int err_cnt = 0;
    bool answer;
    SimpleMessageHeader answer_header;
    the::blob answer_data;

    api_ptr_base prov_ptr = this->find_provider(path);

    if (prov_ptr) {
        this->connect(prov_ptr);

        unsigned short answer = 200; // OK
        this->comm->Send(&answer, 2);

    } else {
        unsigned short answer = 404; // not found
        this->comm->Send(&answer, 2);

        throw the::exception("Request service not found", __FILE__, __LINE__);
    }

    provider_impl* prov = dynamic_cast<provider_impl*>(prov_ptr.get());

    while (rec != 0) {
        answer = false;
        rec = this->comm->Receive(msg.GetHeader().PeekData(), msg.GetHeader().GetHeaderSize());
        if (rec == msg.GetHeader().GetHeaderSize()) {

            if (msg.GetHeader().GetBodySize() > 0) {
                msg.AssertBodySize();
                rec = this->comm->Receive(msg.GetBody(), msg.GetHeader().GetBodySize());
                if (rec != msg.GetHeader().GetBodySize()) {
                    if (rec == 0) break;
                    throw the::exception("Incomplete message body", __FILE__, __LINE__);
                }
            }

            // message complete
            if (msg.GetHeader().GetMessageID() >= RIVLIB_USERMSG) {
                // user message
                prov->on_user_message_received(msg.GetHeader().GetMessageID(),
                    msg.GetHeader().GetBodySize(), static_cast<char*>(msg.GetBody()));

            } else {
                // library message

                switch (msg.GetHeader().GetMessageID()) {
                case static_cast<unsigned int>(message_id::query_data_channels):
                    err_cnt = 0; // message to query available data channels
                    {
                        std::vector<data_channel_info> dci = prov->query_channels();
                        size_t cnt = dci.size();
                        size_t size = sizeof(uint32_t);

                        for (size_t i = 0; i < cnt; ++i) {
                            size += sizeof(uint16_t)
                                + the::math::minimum<size_t>(dci[i].name.size(), UINT16_MAX)
                                + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint8_t);
                        }

                        answer_data.assert_size(size);
                        *answer_data.as_at<uint32_t>(0) = static_cast<uint32_t>(cnt);
                        size = sizeof(uint32_t);
                        for (size_t i = 0; i < cnt; ++i) {
                            size_t namelen = the::math::minimum<size_t>(dci[i].name.size(), UINT16_MAX);

                            *answer_data.as_at<uint16_t>(size) = static_cast<uint16_t>(namelen);
                            size += sizeof(uint16_t);

                            ::memcpy(answer_data.at(size), dci[i].name.c_str(), namelen);
                            size += namelen;

                            *answer_data.as_at<uint16_t>(size) = static_cast<uint16_t>(dci[i].type);
                            size += sizeof(uint16_t);

                            *answer_data.as_at<uint16_t>(size) = dci[i].subtype.uint;
                            size += sizeof(uint16_t);

                            *answer_data.as_at<uint8_t>(size) = dci[i].quality;
                            size += sizeof(uint8_t);
                        }
    
                        answer_header.SetMessageID(static_cast<uint32_t>(message_id::data_channels));
                        answer_header.SetBodySize(static_cast<SimpleMessageSize>(size));
                        answer = true;
                    }
                    break;
                default:
                    // unexpected message
                    ++err_cnt;
                    if (err_cnt >= 10) {
                        throw the::exception("Channel seems broken", __FILE__, __LINE__);
                    } else {
                        this->log().error("Unsupported message (%u) received\n",
                            msg.GetHeader().GetMessageID());
                    }
                    break;
                }

            }

            if (answer) {
                this->comm->Send(answer_header.PeekData(), answer_header.GetHeaderSize());
                if (answer_header.GetBodySize() > 0) {
                    this->comm->Send(answer_data, answer_header.GetBodySize());
                }
            }

        } else if (rec != 0) {
            throw the::exception("Incomplete message header", __FILE__, __LINE__);
        }
    }

}


/*
 * ip_connection::data_chan_receiver
 */
void ip_connection::data_chan_receiver(const std::string& path, const std::string& query) {

    // find provider
    api_ptr_base prov_ptr = this->find_provider(path);

    if (!prov_ptr) {
        unsigned short answer = 404; // not found
        this->comm->Send(&answer, 2);
        throw the::exception("Request service not found", __FILE__, __LINE__);
    }

    unsigned int found = 0;
    uintptr_t name;
    uint16_t type;
    uint16_t subtype;

    std::stringstream stream(query);
    std::string q;

    while (std::getline(stream, q, '&')) {
        if (the::text::string_utility::starts_with(q, "n=")) {
            unsigned char buf[sizeof(uintptr_t)];
            the::text::string_utility::from_hex_string(buf, sizeof(uintptr_t), q.substr(2));
            ::memcpy(&name, buf, sizeof(uintptr_t));
            found |= 1;
        } else if (the::text::string_utility::starts_with(q, "t=")) {
            type = static_cast<uint16_t>(the::text::string_utility::parse_int(q.c_str() + 2));
            found |= 2;
        } else if (the::text::string_utility::starts_with(q, "s=")) {
            subtype = static_cast<uint16_t>(the::text::string_utility::parse_int(q.c_str() + 2));
            found |= 4;
        }
    }

    if (found != 7) {
        unsigned short answer = 400; // bad request
        this->comm->Send(&answer, 2);
        throw the::exception("Request query incomplete", __FILE__, __LINE__);
    }

    // find data channel
    provider_impl* pi = dynamic_cast<provider_impl*>(prov_ptr.get());
    if (pi == nullptr) {
        unsigned short answer = 500; // Internal Server Error
        this->comm->Send(&answer, 2);
        throw the::exception("Internal Server Error when accessing provider", __FILE__, __LINE__);
    }

    if (type == static_cast<uint16_t>(data_channel_type::image_stream)) {
        api_ptr_base img_dat_binding;
        std::vector<api_ptr_base> dcs = pi->select<image_data_binding>();
        for (size_t i = 0, cnt = dcs.size(); i < cnt; ++i) {
            if (reinterpret_cast<uintptr_t>(dcs[i].get()) == name) {
                img_dat_binding = dcs[i];
                break;
            }
        }

        if (!img_dat_binding) {
            unsigned short answer = 404; // Not Found
            this->comm->Send(&answer, 2);
            throw the::exception("Requested data channel not found", __FILE__, __LINE__);
        }

        // TODO: Supported media types should be collected automatically
        if ((subtype != static_cast<uint16_t>(data_channel_image_stream_subtype::rgb_raw)) 
                && (subtype != static_cast<uint16_t>(data_channel_image_stream_subtype::rgb_zip)) ) {
            unsigned short answer = 415; // Unsupported Media Type
            this->comm->Send(&answer, 2);
            throw the::exception("Unsupported media subtype requested", __FILE__, __LINE__);
        }

        encoder::image_encoder_base *encoder = nullptr;
        switch (subtype) {
        case static_cast<uint16_t>(data_channel_image_stream_subtype::rgb_raw):
            encoder = new encoder::image_encoder_rgb_raw();
            break;
        case static_cast<uint16_t>(data_channel_image_stream_subtype::rgb_zip):
            encoder = new encoder::image_encoder_rgb_zip();
            break;
        default: {
            unsigned short answer = 415; // Unsupported Media Type
            this->comm->Send(&answer, 2);
            throw the::exception("Unsupported media subtype requested", __FILE__, __LINE__);
        }
        }

        api_ptr_base encoder_ptr(encoder);
        this->connect(encoder_ptr);
        encoder->connect(img_dat_binding);
        this->connect(this->get_core()); // quick-fix for shutdown assertion ... ugly

        unsigned short answer = 200; // OK
        this->comm->Send(&answer, 2);

        this->image_encoder_receiver(encoder);

    } else {
        unsigned short answer = 415; // Unsupported Media Type
        this->comm->Send(&answer, 2);
        throw the::exception("Unsupported media type requested", __FILE__, __LINE__);
    }
}


/*
 * ip_connection::image_encoder_receiver
 */
void ip_connection::image_encoder_receiver(encoder::image_encoder_base* encoder) {
    size_t rec = 1;
    uint16_t code = 0;
    message_image_request req_message;

    try {

        while (rec != 0) {
            rec = this->comm->Receive(&req_message.bytes, 5);
            if (rec == 5) {
                switch (req_message.req.id) {
                case 0: // close
                    return;
                    break;
                case 1: // restart image stream
                    if (req_message.req.time_code != 0x12345678) {
                        if (req_message.req.time_code == 0x78563412) {
                            throw the::exception("Byte order switch of network image request message is not supported", __FILE__, __LINE__);
                        } else {
                            // printf("%.2x%.2x%.2x%.2x%.2x\n", req_message.bytes[0], req_message.bytes[1], req_message.bytes[2], req_message.bytes[3], req_message.bytes[4]);
                            throw the::exception("Image stream init message broken", __FILE__, __LINE__);
                        }
                    }
                    req_message.req.time_code = 0;
                    // fall though
                case 2: { // next image
                    std::vector<api_ptr_base> encs = this->select<encoder::image_encoder_base>();
                    if (encs.size() != 1) continue;
                    encoder::image_encoder_base *enc = dynamic_cast<encoder::image_encoder_base*>(encs[0].get());

                    encoder::image_request::ptr ir(new encoder::image_request(
                        &ip_connection::send_image_data, this, req_message.req.time_code));
                    //printf("req(%u, %u)\n", req_message.req.id, req_message.req.time_code);

                    enc->request_output(ir);

                } break;
                default: // invalid code. Close!
                    throw the::exception(the::text::astring_builder::format("Invalid image request code received: %d", static_cast<int>(req_message.req.id)).c_str(), __FILE__, __LINE__);
                }
            } else if (rec != 0) {
                throw the::exception("Incomplete message", __FILE__, __LINE__);
            }
        }

    } catch(...) {
        try {
            // remove all pending requests
            std::vector<api_ptr_base> encs = this->select<encoder::image_encoder_base>();
            std::vector<api_ptr_base>::iterator end = encs.end();
            for (std::vector<api_ptr_base>::iterator i = encs.begin(); i < end; i++) {
                encoder::image_encoder_base *enc = dynamic_cast<encoder::image_encoder_base*>(i->get());
                if (enc == nullptr) continue;
                enc->remove_pending_requests(&ip_connection::send_image_data, this);
            }

        } catch(...) {
        }
        throw;
    }
}
