/*
 * rivlib
 * encoder/image_encoder_base.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "encoder/image_encoder_base.h"
#include "api_impl/raw_image_data_binding_impl.h"
#include "data/buffer_type.h"
#include "data/image_buffer_metadata.h"

using namespace eu_vicci::rivlib;


/*
 * encoder::image_encoder_base::runnable::runnable
 */
encoder::image_encoder_base::runnable::runnable(void) : run_func(),
        terminate_e(nullptr), terminate_b(nullptr) {
    // intentionally empty
}


/*
 * encoder::image_encoder_base::runnable::~runnable
 */
encoder::image_encoder_base::runnable::~runnable(void) {
    // intentionally empty
}


/*
 * encoder::image_encoder_base::runnable::run
 */
int encoder::image_encoder_base::runnable::run(void) {
    return this->run_func();
}


/*
 * encoder::image_encoder_base::runnable::on_thread_terminating
 */
the::system::threading::thread::termination_behaviour
encoder::image_encoder_base::runnable::on_thread_terminating(void) throw() {
    THE_ASSERT(this->terminate_e != nullptr);
    THE_ASSERT(this->terminate_b != nullptr);
    *this->terminate_b = true;
    this->terminate_e->set();
    return the::system::threading::thread::termination_behaviour::graceful;
}


/*
 * encoder::image_encoder_base::image_encoder_base
 */
encoder::image_encoder_base::image_encoder_base(void) : element_node(),
        input_worker(), input_new_data_event(), input_data_lock(), input_worker_abort(false), input_worker_running(false), raw_input(),
        encoder_worker(), encoder_new_data_event(), encoder_terminate(false), encoded_data(),
        output_worker(), output_update_event(), out_reqs() {

    this->input_worker.set_run(the::delegate<int>(*this, &image_encoder_base::run_input_collector));
    this->input_worker.set_terminate_event(&this->input_new_data_event);

    this->raw_input.on_update() += data::slot::update_delegate(*this, &image_encoder_base::on_new_input_data);

    this->encoder_worker.set_run(the::delegate<int>(*this, &image_encoder_base::run_encoder));
    this->encoder_worker.set_terminate_event(&this->encoder_new_data_event);

    this->encoded_data.on_update() += data::slot::update_delegate(*this, &image_encoder_base::on_new_encoded_data);

    this->output_worker.set_run(the::delegate<int>(*this, &image_encoder_base::run_output));
    this->output_worker.set_terminate_event(&this->output_update_event);
}


/*
 * encoder::image_encoder_base::~image_encoder_base
 */
encoder::image_encoder_base::~image_encoder_base(void) {
    if (this->input_worker.is_running()) {
        this->input_worker_abort = true;
        this->input_worker.terminate(true);
    }
    if (this->encoder_worker.is_running()) {
        this->encoder_worker.terminate(true);
    }
    if (this->output_worker.is_running()) {
        this->output_worker.terminate(true);
    }
}


/*
 * encoder::image_encoder_base::start_new_input_encoding
 */
void encoder::image_encoder_base::start_new_input_encoding(void) {
    if (!this->input_worker.is_running()) {
        this->input_worker.start();
    }
    if (!this->encoder_worker.is_running()) {
        this->encoder_worker.start();
    }
    if (!this->output_worker.is_running()) {
        this->output_worker.start();
    }
    if (this->input_worker_running) {
        throw new the::invalid_operation_exception("encoder cannot be started when already running", __FILE__, __LINE__);
    }
    this->input_worker_abort = false;
    this->input_new_data_event.set();
}


/*
 * encoder::image_encoder_base::wait_input_encoding
 */
void encoder::image_encoder_base::wait_input_encoding(bool abort) {
    this->input_worker_abort |= abort;
    this->input_data_lock.lock();
    this->input_data_lock.unlock();
    THE_ASSERT(this->input_worker_running == false);
}


/*
 * encoder::image_encoder_base::is_input_encoding_running
 */
bool encoder::image_encoder_base::is_input_encoding_running(void) {
    return this->input_worker_running;
}


/*
 * encoder::image_encoder_base::request_output
 */
void encoder::image_encoder_base::request_output(image_request::ptr req) {
    this->out_reqs.add(req);
    this->output_update_event.set();
}


/*
 * encoder::image_encoder_base::remove_pending_requests
 */
void encoder::image_encoder_base::remove_pending_requests(image_request::output_callback cb, void* ctxt) {
    the::system::threading::auto_lock<the::collections::fast_forward_list<image_request::ptr, the::system::threading::critical_section> > lock(this->out_reqs);
    the::collections::fast_forward_list<image_request::ptr, the::system::threading::critical_section>::enumerator i = this->out_reqs.get_enumerator();
    std::vector<image_request::ptr> delete_these;

    while (i.has_next()) {
        image_request::ptr p = i.next();
        if (p->equals(cb, ctxt)) {
            delete_these.push_back(p);    
        }
    }

    std::vector<image_request::ptr>::iterator dt_end = delete_these.end();
    for (std::vector<image_request::ptr>::iterator i = delete_these.begin(); i != dt_end; i++) {
        this->out_reqs.remove(*i);
    }

    delete_these.clear();
}


/*
 * encoder::image_encoder_base::run_input_collector
 */
int encoder::image_encoder_base::run_input_collector(void) {
    bool terminate = false;
    unsigned int frame_cnt = 0;
    std::vector<api_ptr_base> src = this->select<image_data_binding>();
    if (src.size() != 1) return -1;
    raw_image_data_binding_impl *ridbi = dynamic_cast<raw_image_data_binding_impl*>(src[0].get());
    if (ridbi == nullptr) return -2;

    if ((ridbi->get_colour_type() != image_colour_type::rgb)
        && (ridbi->get_colour_type() != image_colour_type::bgr)) return -3; // unsupported colour type
    if (ridbi->get_data_type() != image_data_type::byte) return -4; // unsupported data type
    if ((ridbi->get_orientation() != image_orientation::bottom_up)
        && (ridbi->get_orientation() != image_orientation::top_down)) return -5; // unsupported orientation type

    bool y_flip = ridbi->get_orientation() == image_orientation::bottom_up;

    this->input_worker.set_terminate_flag(&terminate);

    while (!terminate) {
        this->input_new_data_event.wait();
        if (terminate) break;

        data::buffer::shared_ptr buf;

        this->input_data_lock.lock();
        this->input_worker_running = true;

        frame_cnt++;

        try {
            buf = data::buffer::create();

            buf->set_type(
                (ridbi->get_colour_type() == image_colour_type::rgb)
                ? data::buffer_type::raw_rgb_bytes
                : data::buffer_type::raw_bgr_bytes);
            buf->set_time_code(frame_cnt);

            buf->metadata().assert_size(sizeof(data::image_buffer_metadata));
            unsigned int w, h;
            buf->metadata().as<data::image_buffer_metadata>()->width = w = ridbi->get_width();
            buf->metadata().as<data::image_buffer_metadata>()->height = h = ridbi->get_height();

            size_t scan_width = ridbi->get_scan_width();
            buf->data().assert_size(w * (h - 1) * 3 + scan_width);

            for (unsigned int y = 0; y < h; y++) {
                size_t pos = (y_flip ? (h - (y + 1)) : (y)) * w * 3;
                ::memcpy(buf->data().at(pos), ridbi->as_at<void>(y * scan_width), w * 3);
            }

            this->raw_input.set_buffer(buf);

        } catch(...) {
        }

        this->input_worker_running = false;
        this->input_data_lock.unlock();

    }

    return 0;
}


/*
 * encoder::image_encoder_base::run_encoder
 */
int encoder::image_encoder_base::run_encoder(void) {
    this->encoder_terminate = false;
    this->encoder_worker.set_terminate_flag(&this->encoder_terminate);

    while (!this->encoder_terminate) {
        this->encoder_new_data_event.wait();
        if (this->encoder_terminate) break;

        data::buffer::shared_ptr buf = this->raw_input.get_buffer();
        this->raw_input.set_buffer(nullptr);

        // here, the actual encoding takes place!
        buf = this->encode(buf);

        this->encoded_data.set_buffer(buf);
        this->output_update_event.set();
    }

    return 0;
}


/*
 * encoder::image_encoder_base::run_output
 */
int encoder::image_encoder_base::run_output(void) {
    bool terminate = false;
    this->output_worker.set_terminate_flag(&terminate);
    the::collections::fast_forward_list<image_request::ptr> pending_requests;

    while (!terminate) {
        this->output_update_event.wait();
        if (terminate) break;

        data::buffer::shared_ptr buf = this->encoded_data.get_buffer();
        if (!buf) continue; // no data, so there is nothing to output
        //printf("pre-out: %u\n", buf->time_code());

        THE_ASSERT(pending_requests.is_empty());

        this->out_reqs.lock();

        // data is available
        // now check if there are output requests which can be fulfilled
        while (!this->out_reqs.is_empty()) {

            // no need to sync these, because only this thread removes items and other threads only add items
            image_request::ptr rq = this->out_reqs.first();
            this->out_reqs.remove_first();

            // overflow will occure on 1.5 months update. ... meh
            if (rq->last_time() < buf->time_code()) {
                //printf("out: %u\n", buf->time_code());
                // request fulfillable
                rq->call(buf);

            } else {
                // request cannot be fulfilled at the moment. Keep for later
                pending_requests.add(rq);
            }
        }

        while (!pending_requests.is_empty()) {
            this->out_reqs.append(pending_requests.first());
            pending_requests.remove_first();
        }

        this->out_reqs.unlock();
    }

    return 0;
}
