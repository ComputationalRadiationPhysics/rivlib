/*
 * image_stream_connection_impl.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "api_impl/image_stream_connection_impl.h"
#include "the/system/threading/auto_lock.h"
#include "the/text/string_builder.h"
#include "the/system/performance_counter.h"
#include "vislib/SimpleMessage.h"
#include "message_image_request.h"
#include "data/buffer.h"
#include "data/image_buffer_metadata.h"
#include "data/buffer_type.h"
#include <sstream>
#include "encoder/image_encoder_rgb_zip.h"
#include "encoder/image_encoder_rgb_mjpeg.h"

using namespace eu_vicci::rivlib;
using namespace the::system::threading;


/*
 * image_stream_connection_impl::self_impl::self_impl
 */
image_stream_connection_impl::self_impl::self_impl(void) : connection_base_impl<image_stream_connection_impl>() {
    // intentionally empty
}


/*
 * image_stream_connection_impl::self_impl::~self_impl
 */
image_stream_connection_impl::self_impl::~self_impl(void) {
    // intentionally empty
}


/*
 * image_stream_connection_impl::self_impl::check_uri
 */
void image_stream_connection_impl::self_impl::check_uri(std::string &scheme,
        std::string& username, std::string& host, bool &is_host_v6,
        unsigned short& host_port, std::string& path, std::string& query,
        std::string& fragment) {
    if (scheme != "RIV") throw the::exception("Scheme must be 'RIV'", __FILE__, __LINE__);

    bool has_n = false;
    bool has_t = false;
    bool has_s = false;

    std::stringstream stream(query);
    std::string q;

    while (std::getline(stream, q, '&')) {
        if (the::text::string_utility::starts_with(q, "n=")) has_n = true;
        else if (the::text::string_utility::starts_with(q, "t=")) has_t = true;
        else if (the::text::string_utility::starts_with(q, "s=")) has_s = true;
    }

    if (!has_n || !has_t || !has_s) {
        throw the::exception("Query incomplete", __FILE__, __LINE__);
    }

    if (!fragment.empty()) throw the::exception("Fragments are not allowed for image_stream_connections", __FILE__, __LINE__);
}


/*
 * image_stream_connection_impl::self_impl::evaluate_answer
 */
void image_stream_connection_impl::self_impl::evaluate_answer(unsigned short code) {
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


//static unsigned int imgcnt = 0;


/*
 * image_stream_connection_impl::self_impl::communication_core
 */
void image_stream_connection_impl::self_impl::communication_core(void) {
    vislib::net::SimpleMessage msg;
    size_t rec = 1;
    the::system::performance_counter timer;
    size_t dat_cnt = 0;
    size_t frm_cnt = 0;
    timer.start();

    message_image_request req_message;
    req_message.req.id = 1;
    req_message.req.time_code = 0x12345678;
    THE_ASSERT(req_message.bytes[0] = 0x01);
    THE_ASSERT(req_message.bytes[1] = 0x78);
    THE_ASSERT(req_message.bytes[2] = 0x56);
    THE_ASSERT(req_message.bytes[3] = 0x34);
    THE_ASSERT(req_message.bytes[4] = 0x12);

    this->send(&req_message.bytes, 5);

    while (rec != 0) {

        rec = this->receive(msg.GetHeader().PeekData(), msg.GetHeader().GetHeaderSize());

        if (rec == msg.GetHeader().GetHeaderSize()) {

            dat_cnt += msg.GetHeader().GetHeaderSize();
            frm_cnt++;

            if (msg.GetHeader().GetBodySize() > 0) {
                msg.AssertBodySize();
                rec = this->receive(msg.GetBody(), msg.GetHeader().GetBodySize());
                if (rec != msg.GetHeader().GetBodySize()) {
                    if (rec == 0) break;
                    throw the::exception("Incomplete message body", __FILE__, __LINE__);
                }
                // printf("Received %d bytes\n", (int)msg.GetHeader().GetBodySize());
                dat_cnt += msg.GetHeader().GetBodySize();
            }

            // message complete
            if (msg.GetHeader().GetMessageID() == static_cast<vislib::net::SimpleMessageID>(message_id::image_data_blob)) {
                if (msg.GetHeader().GetBodySize() < (2 * sizeof(uint32_t) + sizeof(data::image_buffer_metadata))) {
                    throw the::exception("Image data missing", __FILE__, __LINE__);
                }

                // could all be nicer using shallow simple message
                data::buffer::shared_ptr buf = data::buffer::create();
                buf->set_type(static_cast<data::buffer_type>(msg.GetBodyAs<uint32_t>()[0]));
                buf->set_time_code(msg.GetBodyAs<uint32_t>()[1]);
                buf->metadata().assert_size(sizeof(data::image_buffer_metadata));
                ::memcpy(buf->metadata(), msg.GetBodyAsAt<void>(sizeof(uint32_t) * 2), sizeof(data::image_buffer_metadata));
                size_t data_size = msg.GetHeader().GetBodySize() - (2 * sizeof(uint32_t) + sizeof(data::image_buffer_metadata));
                buf->data().assert_size(data_size);
                ::memcpy(buf->data(), msg.GetBodyAsAt<void>(2 * sizeof(uint32_t) + sizeof(data::image_buffer_metadata)), data_size);

                if (timer.elapsed_milliseconds() >= 1000.0) {
                    // transfer in bit/sec
                    double trans = (8.0 * static_cast<double>(dat_cnt)) / (static_cast<double>(timer.elapsed_milliseconds()) / 1000.0);
                    double fps = static_cast<double>(frm_cnt) / (static_cast<double>(timer.elapsed_milliseconds()) / 1000.0);
                    double compRatio = static_cast<double>(buf->data().size()) / static_cast<double>(
                        buf->metadata().as<data::image_buffer_metadata>()->width
                        * buf->metadata().as<data::image_buffer_metadata>()->height
                        * 3);
                    dat_cnt = 0;
                    frm_cnt = 0;
                    timer.start();
                    const char *ord = "";

                    if (trans > 1024 * 1024) {
                        ord = "M";
                        trans /= 1024.0 * 1024.0;
                    } else if (trans > 1024) {
                        ord = "K";
                        trans /= 1024.0;
                    }
                    //printf("    %.2f %sBit/s\n", trans, ord);
                    //printf("    %.2f dFPS\n", fps);
                    //printf("    %.2f%% compression\n", compRatio * 100.0);

                }


                if (buf->type() != data::buffer_type::raw_rgb_bytes) {
                    // decoding!
                    switch (buf->type()) {
                    case data::buffer_type::zip_rgb_bytes: {
                        static encoder::image_encoder_rgb_zip codec; // uck
                        buf = codec.decode(buf);

                    } break;
                    case data::buffer_type::mjpeg_rgb_bytes: {
                        static encoder::image_encoder_rgb_mjpeg codec; // uck
                        buf = codec.decode(buf);

                    } break;
                    default:
                        throw the::exception("Data conversion is strange", __FILE__, __LINE__);
                        break;
                    }
                }

                // buffer now completely interpreted
                // request next frame before decoding (even faster requesting would be nice)
                req_message.req.id = 2; // follow up frame
                req_message.req.time_code = buf->time_code();
                //printf("req(%u, %u)\n", req_message.req.id, req_message.req.time_code);
                this->send(&req_message.bytes, 5);

                THE_ASSERT(buf->type() == data::buffer_type::raw_rgb_bytes);
                {
                    auto_lock<self_impl> lock(*this);
                    size_t l_s = this->get_listeners().size();
                    for (size_t i = 0; i < l_s; ++i) {
                        this->get_listeners()[i]->on_image_data(this->get_owner(), 
                            buf->metadata().as<data::image_buffer_metadata>()->width,
                            buf->metadata().as<data::image_buffer_metadata>()->height,
                            buf->data());
                    }
                }

            }

        } else if (rec != 0) {
            throw the::exception("Incomplete message header", __FILE__, __LINE__);
        }
    }
}


/*
 * image_stream_connection_impl::image_stream_connection_impl
 */
image_stream_connection_impl::image_stream_connection_impl(void)
        : image_stream_connection(), node(), impl() {
    this->impl.set_owner(this);
}


/*
 * image_stream_connection_impl::~image_stream_connection_impl
 */
image_stream_connection_impl::~image_stream_connection_impl(void) {
    // intentionally empty
}


/*
 * image_stream_connection_impl::add_listener
 */
void image_stream_connection_impl::add_listener(listener* l) {
    this->impl.add_listener(l);
}


/*
 * image_stream_connection_impl::remove_listener
 */
void image_stream_connection_impl::remove_listener(listener* l) {
    this->impl.remove_listener(l);
}


/*
 * image_stream_connection_impl::remove_all_listeners
 */
void image_stream_connection_impl::remove_all_listeners(void) {
    this->impl.remove_all_listeners();
}


/*
 * image_stream_connection_impl::connect
 */
void image_stream_connection_impl::connect(const char *uri) {
    this->impl.connect(uri);
}


/*
 * image_stream_connection_impl::connect
 */
void image_stream_connection_impl::connect(const wchar_t *uri) {
    this->impl.connect(uri);
}


/*
 * image_stream_connection_impl::disconnect
 */
void image_stream_connection_impl::disconnect(bool wait) {
    this->impl.disconnect(wait);
}


/*
 * image_stream_connection_impl::get_status
 */
control_connection::status image_stream_connection_impl::get_status(void) const {
    return this->impl.get_status();
}


/*
 * image_stream_connection_impl::get_uri_astr
 */
const char *image_stream_connection_impl::get_uri_astr(void) const {
    return this->impl.get_uri_astr();
}


/*
 * image_stream_connection_impl::get_uri_wstr
 */
const wchar_t *image_stream_connection_impl::get_uri_wstr(void) const {
    return this->impl.get_uri_wstr();
}


/*
 * image_stream_connection_impl::lock
 */
void image_stream_connection_impl::lock(void) {
    this->impl.lock();
}


/*
 * image_stream_connection_impl::unlock
 */
void image_stream_connection_impl::unlock(void) {
    this->impl.unlock();
}


/*
 * image_stream_connection_impl::log
 */
error_log& image_stream_connection_impl::log(void) {
    return error_log::null_log;
}


/*
 * image_stream_connection_impl::is_supported
 */
bool image_stream_connection_impl::is_supported(data_channel_image_stream_subtype subtype) {
    return (subtype == data_channel_image_stream_subtype::rgb_raw)
        || (subtype == data_channel_image_stream_subtype::rgb_zip)
        || (subtype == data_channel_image_stream_subtype::rgb_mjpeg);
    // TODO: support more ...
}
